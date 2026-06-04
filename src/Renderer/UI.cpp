// Created by izakr on 27/03/2026.
#include <iostream>
#include <memory>
#include <ostream>
#include <SDL3_ttf/SDL_ttf.h>
#include <shobjidl.h>
#include <vector>
#include <windows.h>
#include "Renderer.h"
#include "ColorPicker.h"
#include "View.h"
#include "../file/file.h"

using namespace std;

static char export_filename[64] = "my_artwork.ppe";


void process_start_window(mu_Context* ctx, Document* &CurrentFile) {
     {
        int widths[1] = {-1};
        mu_layout_row(ctx, 1, widths, 0);
        mu_layout_row(ctx, 1, widths, 30);

        if (mu_button(ctx, "New Project")) {
            if (CurrentFile) delete CurrentFile;

            CurrentFile = new Document(100, 100, "New Artwork", std::vector<Color>());
            CurrentFile->addLayer("Background", {1, 1, 1, 1});
        }

        if (mu_button(ctx, "Open Project")) {
            std::string selected_path = open_file_dialog_windows();

            if (!selected_path.empty()) {
                u32 loaded_w = 0, loaded_h = 0;
                std::vector<Color> pixels = ReadFileType(selected_path.c_str(), loaded_w, loaded_h);

                if (pixels.empty()) {
                    std::cout << "error, pixels array empty or file missing" << std::endl;
                } else {
                    if (CurrentFile) delete CurrentFile;
                    CurrentFile = new Document(loaded_w, loaded_h, "Loaded Project", pixels);
                }
            } else {
                std::cout << "Uzytkownik anulowal wybor pliku." << std::endl;
            }
        }
        mu_end_window(ctx);
    }
}


void MainLoop(mu_Context* ctx, Document* &CurrentFile) {
    process_start_window(ctx, CurrentFile);
}


void process_microui(mu_Context* ctx, Document& doc, std::unique_ptr<ITool>& currentTool, UIConfig &uiConfig) {
    char charvalue[32];
    int widths[1] = {-1};

    if (mu_begin_window(ctx, "Toolbar", mu_rect(10, 10, 160, 200))) {
        mu_layout_row(ctx, 1, widths, 0);

        if (mu_button(ctx, "Pencil")) {
            currentTool = std::make_unique<Pencil>();
        }
        if (mu_button(ctx, "Eraser")) {
            currentTool = std::make_unique<Eraser>();
        }

        mu_layout_row(ctx, 1, widths, 0);
        mu_label(ctx, "Pixel Size:");
        if (mu_slider_ex(ctx, &uiConfig.PixelSize, 1, 100, 1, charvalue, sizeof(charvalue))) {
            doc.PixelSize = uiConfig.PixelSize;
        }

        mu_layout_row(ctx, 1, widths, 0);
        mu_label(ctx, "Scale:");
        float value = uiConfig.scale;
        if (mu_slider_ex(ctx, &value, 0.1f, 20.0f, 0.1f, charvalue, sizeof(charvalue))) {
            uiConfig.scale = value;
        }

        mu_layout_row(ctx, 1, widths, 0);
        mu_label(ctx, "R:");
        mu_Real R = uiConfig.R;
        if (mu_slider_ex(ctx, &R, 0.0f, 255.0f, 1.0f, charvalue, sizeof(charvalue))) {
            uiConfig.R = static_cast<RGBA255>(R + 0.5f);
        }

        mu_layout_row(ctx, 1, widths, 0);
        mu_label(ctx, "G:");
        mu_Real G = uiConfig.G;
        if (mu_slider_ex(ctx, &G, 0.0f, 255.0f, 1.0f, charvalue, sizeof(charvalue))) {
            uiConfig.G = static_cast<RGBA255>(G + 0.5f);
        }

        mu_layout_row(ctx, 1, widths, 0);
        mu_label(ctx, "B:");
        mu_Real B = uiConfig.B;
        if (mu_slider_ex(ctx, &B, 0.0f, 255.0f, 1.0f, charvalue, sizeof(charvalue))) {
            uiConfig.B = static_cast<RGBA255>(B + 0.5f);
        }

        mu_layout_row(ctx, 1, widths, 0);
        mu_label(ctx, "A:");
        mu_Real A = uiConfig.A;
        if (mu_slider_ex(ctx, &A, 0.0f, 255.0f, 1.0f, charvalue, sizeof(charvalue))) {
            uiConfig.A = static_cast<RGBA255>(A + 0.5f);
        }

        mu_layout_row(ctx, 1, widths, 50);
        mu_draw_rect(ctx, mu_rect(10, 10, 50, 50), mu_color(uiConfig.R, uiConfig.G, uiConfig.B, uiConfig.A));

        mu_label(ctx, "Layers:");
        for (size_t i = 0; i < doc.layers.size(); i++) {
            mu_text(ctx, doc.layers[i].name.c_str());
        }

        mu_end_window(ctx);
    }

    if (mu_begin_window(ctx, "File Manager", mu_rect(10, 470, 160, 150))) {
        mu_layout_row(ctx, 1, widths, 0);
        mu_label(ctx, "Filename:");

        mu_textbox(ctx, export_filename, sizeof(export_filename));

        mu_layout_row(ctx, 1, widths, 0);
        if (mu_button(ctx, "Save (.ppe)")) {
            WriteFileType(export_filename, doc.height, doc.width, doc.composite());
        }

        if (mu_button(ctx, "Load (.ppe)")) {
            u32 loaded_w = 0, loaded_h = 0;
            std::vector<Color> pixels = ReadFileType(export_filename, loaded_w, loaded_h);

            if (pixels.empty()) {
                cout << "error, pixels array empty or file missing" << endl;
            } else {
                if (loaded_w == doc.width && loaded_h == doc.height) {
                    doc.activeLayer().pixels = pixels;
                    cout << "Success: Loaded file data into active layer!" << endl;
                } else {
                    cout << "error: Loaded image size doesn't match canvas size!" << endl;
                }
            }
        }
        mu_end_window(ctx);
    }
}

void UpdateRGBAPaletteTexture(SDL_Renderer* renderer, SDL_Texture* texture, uint8_t current_blue) {
    uint32_t pixels[256 * 256];

    for (int y = 0; y < 256; y++) {
        for (int x = 0; x < 256; x++) {
            uint8_t r = x;               // Red increases left-to-right (0 to 255)
            uint8_t g = 255 - y;         // Green increases bottom-to-top (0 to 255)
            uint8_t b = current_blue;    // Blue is locked to whatever the slider is at
            uint8_t a = 255;             // Fully opaque for the picker background

            // Pack pixels as RGBA8888
            pixels[y * 256 + x] = (r << 24) | (g << 16) | (b << 8) | a;
        }
    }

    SDL_UpdateTexture(texture, nullptr, pixels, 256 * sizeof(uint32_t));
}

void ProcessMainMenu(mu_Context* ctx) {
    if (mu_begin_window(ctx, "Advanced Color Selector", mu_rect(0, 0, Render::WIN_W/8, Render::WIN_H))) {
        int widths[1] = {-1};
        mu_layout_row(ctx, 1, widths, 180);
        mu_Rect square_rect = mu_layout_next(ctx);
        mu_Id square_id = mu_get_id(ctx, "gradient_box", 12);
        mu_update_control(ctx, square_id, square_rect, 0);

        if (ctx->focus == square_id && (ctx->mouse_down & MU_MOUSE_LEFT)) {
            float x_pct = (float)(ctx->mouse_pos.x - square_rect.x) / square_rect.w;
            float y_pct = (float)(ctx->mouse_pos.y - square_rect.y) / square_rect.h;

            if (x_pct < 0.0f) x_pct = 0.0f; if (x_pct > 1.0f) x_pct = 1.0f;
            if (y_pct < 0.0f) y_pct = 0.0f; if (y_pct > 1.0f) y_pct = 1.0f;


            currentHSV.s = static_cast<uint8_t>(x_pct * 255.0f);

            currentHSV.v = static_cast<uint8_t>((1.0f - y_pct) * 255.0f);
            Render::UpdateGradientSquare();
        }
        mu_draw_icon(ctx, MU_ICON_MAX, square_rect, mu_color(255, 255, 255, 255));

        mu_layout_row(ctx, 1, widths, 0);
        mu_end_window(ctx);
    }
}


