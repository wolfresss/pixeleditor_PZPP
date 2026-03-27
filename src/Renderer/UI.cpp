//
// Created by izakr on 27/03/2026.
//
#include "View.h"
void process_microui(mu_Context* ctx, Document& doc, std::unique_ptr<ITool>& currentTool, UIConfig &uiConfig) {
    mu_begin(ctx);

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

        // PixelSize slider
        mu_layout_row(ctx, 1, widths, 0);
        mu_label(ctx, "Pixel Size:");
        if (mu_slider_ex(ctx, &uiConfig.PixelSize, 1, 100, 1, charvalue, sizeof(charvalue))) {
            doc.PixelSize = uiConfig.PixelSize;
        }

        // Scale slider
        mu_layout_row(ctx, 1, widths, 0);
        mu_label(ctx, "Scale:");
        float value = uiConfig.scale;
        if (mu_slider_ex(ctx, &value, 0.1f, 20.0f, 0.1f, charvalue, sizeof(charvalue))) {
            uiConfig.scale = value;
        }

        // RGB sliders
        mu_layout_row(ctx, 1, widths, 0);
        mu_label(ctx, "R:");
        mu_Real R = uiConfig.R;
        if (mu_slider_ex(ctx, &R, 0.0f, 255.0f, 1.0f, charvalue, sizeof(charvalue))) {
            uiConfig.R = static_cast<RGB255>(R + 0.5f);
        }

        mu_layout_row(ctx, 1, widths, 0);
        mu_label(ctx, "G:");
        mu_Real G = uiConfig.G;
        if (mu_slider_ex(ctx, &G, 0.0f, 255.0f, 1.0f, charvalue, sizeof(charvalue))) {
            uiConfig.G = static_cast<RGB255>(G + 0.5f);
        }

        mu_layout_row(ctx, 1, widths, 0);
        mu_label(ctx, "B:");
        mu_Real B = uiConfig.B;
        if (mu_slider_ex(ctx, &B, 0.0f, 255.0f, 1.0f, charvalue, sizeof(charvalue))) {
            uiConfig.B = static_cast<RGB255>(B + 0.5f);
        }

        mu_layout_row(ctx, 1, widths, 0);
        mu_label(ctx, "A:");
        mu_Real A = uiConfig.A;
        if (mu_slider_ex(ctx, &A, 0.0f, 255.0f, 1.0f, charvalue, sizeof(charvalue))) {
            uiConfig.A = static_cast<RGB255>(A + 0.5f);
        }

        mu_layout_row(ctx, 1, widths, 50); // wysokość prostokąta 50px
        mu_draw_rect(ctx, mu_rect(10, 10, 50, 50), mu_color(uiConfig.R, uiConfig.G, uiConfig.B, uiConfig.A));

        mu_label(ctx, "Layers:");
        for (size_t i = 0; i < doc.layers.size(); i++) {
            mu_text(ctx, doc.layers[i].name.c_str());
        }

        mu_end_window(ctx);
    }

    mu_end(ctx);
}

int text_width(mu_Font font, const char *text, int len) {
    if (len == -1) len = (int)strlen(text);
    return len * 8; // Tymczasowo 8px/znak
}
int text_height(mu_Font font) {
    return 16; // 16px wysokości lini
}

void render_microui(SDL_Renderer* renderer, mu_Context* ctx) {
    mu_Command* cmd = nullptr;
    while (mu_next_command(ctx, &cmd)) {
        switch (cmd->type) {
            case MU_COMMAND_RECT: {
                SDL_FRect rect = {
                    (float)cmd->rect.rect.x,
                    (float)cmd->rect.rect.y,
                    (float)cmd->rect.rect.w,
                    (float)cmd->rect.rect.h
                };
                SDL_SetRenderDrawColor(renderer, cmd->rect.color.r, cmd->rect.color.g, cmd->rect.color.b, cmd->rect.color.a);
                SDL_RenderFillRect(renderer, &rect);
                break;
            }
            case MU_COMMAND_TEXT: {
                // Na razie ignorujemy tekst
                break;
            }
            case MU_COMMAND_CLIP: {
                SDL_Rect clip = {
                    cmd->clip.rect.x,
                    cmd->clip.rect.y,
                    cmd->clip.rect.w,
                    cmd->clip.rect.h
                };
                SDL_SetRenderClipRect(renderer, &clip);
                break;
            }
        }
    }
}