#include <iostream>
#include <memory>
#include <string>
#include <algorithm>

#include "Renderer.h"
#include "CanvasRenderer.h"
#include "ColorUtils.h"
#include "../file/file.h"
#include "../Resources/binary_icons.h"
#include "../core/structs.h"

extern "C" {
#include "microui.h"
}

using namespace std;

// --- CONFIGURATION & BUFFERS ---
struct ProjectConfig {
    std::string name = "Untitled";
    int width = 800;
    int height = 600;
};

static char name_buf[64] = "";
static char height_buf[64] = "";
static char width_buf[64] = "";

const size_t MAX_PALETTE_SIZE = 60;
static ProjectConfig current_config;

// --- REUSABLE UI COMPONENTS ---

int mu_button_icon(mu_Context* ctx, int icon_id) {
    mu_Rect r = mu_layout_next(ctx);
    mu_Id id = mu_get_id(ctx, &icon_id, sizeof(icon_id));
    mu_update_control(ctx, id, r, 0);

    int clicked = 0;
    if (ctx->focus == id && (ctx->mouse_pressed & MU_MOUSE_LEFT)) {
        clicked = 1;
    }

    mu_Color bg = ctx->style->colors[MU_COLOR_BUTTON];
    if (ctx->focus == id) { bg = ctx->style->colors[MU_COLOR_BUTTONFOCUS]; }
    if (ctx->hover == id) { bg = ctx->style->colors[MU_COLOR_BUTTONHOVER]; }

    mu_draw_rect(ctx, r, bg);
    mu_draw_icon(ctx, icon_id, r, ctx->style->colors[MU_COLOR_TEXT]);

    return clicked;
}

// --- INTERNAL HELPERS ---

static void HandleGradientInput(mu_Context* ctx, mu_Id id, mu_Rect rect) {
    if (ctx->focus == id && (ctx->mouse_down & MU_MOUSE_LEFT)) {
        // Calculate normalized mouse positions inside the gradient box
        float x_pct = (float)(ctx->mouse_pos.x - rect.x) / rect.w;
        float y_pct = (float)(ctx->mouse_pos.y - rect.y) / rect.h;

        x_pct = std::clamp(x_pct, 0.0f, 1.0f);
        y_pct = std::clamp(y_pct, 0.0f, 1.0f);

        // Map normalized coordinates to HSV spectrum (X -> Saturation, Y -> Value)
        Render::currentHSV.s = static_cast<uint8_t>(x_pct * 255.0f);
        Render::currentHSV.v = static_cast<uint8_t>((1.0f - y_pct) * 255.0f);
        Render::currentColor = ColorUtils::HSVToRGB(Render::currentHSV, Render::currentHSV.s, 255 - Render::currentHSV.v);
    }
}

static void RenderDynamicPalette(mu_Context* ctx) {
    if (!Render::window.CurrentFile) return;

    auto& palette = Render::window.CurrentFile->dynamic_palette;
    mu_layout_row(ctx, 10, Render::uiConfig.paletteColWidths, Render::uiConfig.paletteRowHeight);

    for (size_t i = 0; i < palette.size(); ++i) {
        mu_Id id = mu_get_id(ctx, &palette[i], sizeof(void*));
        mu_Rect r = mu_layout_next(ctx);
        mu_update_control(ctx, id, r, 0);

        if (ctx->focus == id && (ctx->mouse_pressed & MU_MOUSE_LEFT)) {
            Render::currentColor = palette[i];
            HSV raw_hsv = ColorUtils::RGBToHSV(Render::currentColor);
            Render::currentHSV.h = raw_hsv.h;
            Render::currentHSV.s = raw_hsv.s;
            Render::currentHSV.v = raw_hsv.v;
            Render::UpdateGradientSquare();
        }

        mu_Color mu_c = mu_color(palette[i].r, palette[i].g, palette[i].b, 255);
        mu_draw_rect(ctx, r, mu_c);
    }

    // Render palette append button if capacity limits are not exceeded
    if (palette.size() < MAX_PALETTE_SIZE) {
        mu_Rect plus_rect = mu_layout_next(ctx);
        mu_Id plus_id = mu_get_id(ctx, "add_color_btn", 13);
        mu_update_control(ctx, plus_id, plus_rect, 0);

        mu_Color bg = ctx->style->colors[MU_COLOR_BUTTON];
        if (ctx->focus == plus_id) { bg = ctx->style->colors[MU_COLOR_BUTTONFOCUS]; }
        if (ctx->hover == plus_id) { bg = ctx->style->colors[MU_COLOR_BUTTONHOVER]; }

        mu_draw_rect(ctx, plus_rect, bg);
        mu_draw_icon(ctx, ICON_PBM_SMALL_PLUS, plus_rect, ctx->style->colors[MU_COLOR_TEXT]);

        if (ctx->focus == plus_id && (ctx->mouse_pressed & MU_MOUSE_LEFT)) {
            palette.push_back(Render::currentColor);
        }
    }
}

// --- INTERFACE PANELS ---

void ProcessMainMenu(mu_Context* ctx) {
    if (mu_begin_window(ctx, "Advanced Color Selector", mu_rect(0, 0, Render::WIN_W / 8, Render::WIN_H))) {

        // 1. Color Picker Segment (Gradient & Color Preview)
        mu_layout_row(ctx, 1, Render::uiConfig.layoutWidths, 180);
        mu_Rect square_rect = mu_layout_next(ctx);
        mu_Rect square_rect2 = mu_layout_next(ctx);
        mu_Id square_id = mu_get_id(ctx, "gradient_box", 12);
        mu_update_control(ctx, square_id, square_rect, 0);

        HandleGradientInput(ctx, square_id, square_rect);

        mu_Color color = ColorUtils::HSVTo_mu_Color(Render::currentHSV, Render::currentHSV.s, 255 - Render::currentHSV.v);
        mu_draw_rect(ctx, square_rect2, color);
        mu_draw_icon(ctx, MU_ICON_MAX, square_rect, mu_color(255, 255, 255, 255));

        // 2. Control Sliders (HUE & Brush Engine Parameterization)
        char charvalue[32];
        mu_layout_row(ctx, 1, Render::uiConfig.layoutWidths, 0);

        mu_label(ctx, "HUE:");
        mu_Real Hue = Render::currentHSV.h;
        if (mu_slider_ex(ctx, &Hue, 0.0f, 255.0f, 1.0f, charvalue, sizeof(charvalue))) {
            Render::currentHSV.h = static_cast<uint16_t>(Hue);
            Render::UpdateGradientSquare();
        }

        mu_label(ctx, "Brush Size:");
        if (mu_slider_ex(ctx, &Render::uiConfig.PixelSize, 0.0f, 255.0f, 1.0f, charvalue, sizeof(charvalue))) {
            pixelSize = static_cast<uint16_t>(Render::uiConfig.PixelSize);
            Render::UpdateGradientSquare();
        }

        // 3. Toolset & Vector Interpolation Toggle
        mu_label(ctx, "Tools:");
        std::string check_text = Render::uiConfig.useInterpolation ? "[X] Interpolate Line" : "[ ] Interpolate Line";
        if (mu_button(ctx, check_text.c_str())) {
            Render::uiConfig.useInterpolation = !Render::uiConfig.useInterpolation;
        }

        mu_layout_row(ctx, 2, Render::uiConfig.toolWidths, Render::uiConfig.toolRowHeight);
        if (mu_button_icon(ctx, ICON_PBM_PENCIL)) {
            Render::uiConfig.selectedTool = PENCIL;
        }
        if (mu_button_icon(ctx, ICON_PBM_RUBBER)) {
            Render::uiConfig.selectedTool = RUBBER;
        }

        // 4. Color Swatch Management
        mu_layout_row(ctx, 1, Render::uiConfig.layoutWidths, 0);
        mu_label(ctx, "Palette:");
        RenderDynamicPalette(ctx);
    }
    mu_end_window(ctx);
}

void ProcessLeftBar(mu_Context* ctx) {
    if (mu_begin_window(ctx, "File", mu_rect(Render::WIN_W - Render::WIN_W / 8, 0, Render::WIN_W / 8, Render::WIN_H))) {

        mu_layout_row(ctx, 1, Render::uiConfig.layoutWidths, 0);

        // Project Metadata Input Buffers
        mu_label(ctx, "Name:");
        int res_name = mu_textbox(ctx, name_buf, sizeof(name_buf));
        if (res_name & MU_RES_SUBMIT || (ctx->focus == mu_get_id(ctx, &name_buf, sizeof(name_buf)))) {
            current_config.name = std::string(name_buf);
        }

        mu_label(ctx, "Height:");
        if (mu_textbox(ctx, height_buf, sizeof(height_buf))) {
            try { if (height_buf[0] != '\0') current_config.height = std::stoi(height_buf); } catch (...) {}
        }

        mu_label(ctx, "Width:");
        if (mu_textbox(ctx, width_buf, sizeof(width_buf))) {
            try { if (width_buf[0] != '\0') current_config.width = std::stoi(width_buf); } catch (...) {}
        }

        // File I/O Operations & Context Lifecycle Control
        mu_layout_row(ctx, 1, Render::uiConfig.layoutWidths, 30);
        if (mu_button(ctx, "New Project")) {
            current_config.name = name_buf[0] != '\0' ? name_buf : "NewProject";
            current_config.width = (width_buf[0] != '\0') ? std::stoi(width_buf) : 800;
            current_config.height = (height_buf[0] != '\0') ? std::stoi(height_buf) : 600;

            delete Render::window.CurrentFile;
            delete Render::window.canvasRenderer;

            Render::window.CurrentFile = new Document(current_config.width, current_config.height, current_config.name.c_str(), Color{255, 255, 255, 255});
            Render::window.canvasRenderer = new CanvasRenderer(Render::window.renderer, current_config.width, current_config.height);

            Render::currentColor = Color{0, 0, 0, 255};
            Render::uiConfig.selectedTool = PENCIL;
            ctx->focus = ctx->hover = ctx->updated_focus = 0;
        }

        if (mu_button(ctx, "Open Project")) {
            std::string path = open_file_dialog_windows();
            if (!path.empty()) {
                delete Render::window.CurrentFile;
                delete Render::window.canvasRenderer;

                Render::window.CurrentFile = LoadDocumentPPE(path.c_str());
                if (Render::window.CurrentFile) {
                    Render::window.canvasRenderer = new CanvasRenderer(Render::window.renderer, Render::window.CurrentFile->width, Render::window.CurrentFile->height);
                    snprintf(name_buf, sizeof(name_buf), "%s", Render::window.CurrentFile->name.c_str());
                    snprintf(width_buf, sizeof(width_buf), "%d", Render::window.CurrentFile->width);
                    snprintf(height_buf, sizeof(height_buf), "%d", Render::window.CurrentFile->height);
                }
            }
        }

        if (mu_button(ctx, "Save Project")) {
            if (Render::window.CurrentFile != nullptr) {
                std::string path = save_file_dialog_windows();
                if (!path.empty()) SaveDocumentPPE(path.c_str(), *Render::window.CurrentFile);
            }
        }

        // Raster Layer Composition Stack Management
        if (Render::window.CurrentFile != nullptr) {
            mu_layout_row(ctx, 1, Render::uiConfig.layoutWidths, 0);
            mu_draw_control_text(ctx, "-------------------", mu_rect(0, 0, 0, 0), MU_COLOR_TEXT, 0);

            int header_widths[] = { 100, 60 };
            mu_layout_row(ctx, 2, header_widths, 0);
            mu_label(ctx, "Layers:");

            if (mu_button(ctx, "+ Add")) {
                std::string new_layer_name = "Layer " + std::to_string(Render::window.CurrentFile->layers.size());
                Render::window.CurrentFile->addLayer(new_layer_name, Color{0, 0, 0, 0});
                Render::window.CurrentFile->activeLayerIndex = Render::window.CurrentFile->layers.size() - 1;
            }

            mu_layout_row(ctx, 1, Render::uiConfig.layoutWidths, 0);
            mu_draw_control_text(ctx, "-------------------", mu_rect(0, 0, 0, 0), MU_COLOR_TEXT, 0);

            Document* doc = Render::window.CurrentFile;
            mu_Color backup_color = ctx->style->colors[MU_COLOR_BUTTON];

            for (int i = (int)doc->layers.size() - 1; i >= 0; i--) {
                auto& layer = doc->layers[i];
                int row_widths[] = { 35, -1 };
                mu_layout_row(ctx, 2, row_widths, 35);

                // Layer visibility visibility toggle (Eye glyph representation)
                int eye_icon = layer.visible ? ICON_PBM_EYE_OPEN : ICON_PBM_EYE_CLOSED;
                mu_push_id(ctx, &i, sizeof(int));
                mu_push_id(ctx, "eye", 3);
                if (mu_button_icon(ctx, eye_icon)) {
                    layer.visible = !layer.visible;
                }
                mu_pop_id(ctx);
                mu_pop_id(ctx);

                // Highlight UI focus for the currently active render layer target
                ctx->style->colors[MU_COLOR_BUTTON] = (i == doc->activeLayerIndex) ? mu_color(60, 120, 180, 255) : backup_color;

                // Selection node binding for explicit layer focus swapping
                mu_push_id(ctx, &i, sizeof(int));
                mu_push_id(ctx, "name", 4);
                if (mu_button(ctx, layer.name.c_str())) {
                    doc->activeLayerIndex = i;
                }
                mu_pop_id(ctx);
                mu_pop_id(ctx);
            }
            ctx->style->colors[MU_COLOR_BUTTON] = backup_color;
        }
    }
    mu_end_window(ctx);
}