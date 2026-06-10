//
// Created by izakr on 01/06/2026.
//

#include "Renderer.h"
#include <iostream>
#include <ostream>
#include <SDL3_ttf/SDL_ttf.h>
#include <string.h>

namespace Render {
    WindowContext window;

    SDL_Texture* CreatePaletteTexture() {
        // TO DO: steaming for modification
        SDL_Texture* texture = SDL_CreateTexture(window.renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, 256, 256);
        if (!texture) {
            SDL_Log("Failed to create palette texture: %s", SDL_GetError());
        }
        return texture;
    }

    void UpdateGradientSquare() {
        if (!window.paletteTexture) return;

        Color Palette[256][256];

        UpdatePalette(Palette);

        SDL_UpdateTexture(window.paletteTexture, nullptr, Palette, 256 * sizeof(uint32_t));
    }

    int text_width(mu_Font font, const char *text, int len) {
        if (len == -1) {
            len = (int)strlen(text);
        }
        return len * 8;
    }
    int text_height(mu_Font font) {
        return 16;
    }

    void Process_Events() {
        if (!window.mu_ctx) {
            std::cout << "[CRITICAL] Process_Events widzi mu_ctx jako NULL!" << std::endl;
            return;
        }
        while (SDL_PollEvent(&window.event)) {
            switch (window.event.type) {
                case SDL_EVENT_QUIT:
                    window.shouldQuit = true;
                    break;
                case SDL_EVENT_MOUSE_MOTION:
                    mu_input_mousemove(window.mu_ctx, (int)window.event.motion.x, (int)window.event.motion.y);
                    break;
                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                    // Zmapowane na lewy przycisk myszy
                    mu_input_mousedown(window.mu_ctx, (int)window.event.button.x, (int)window.event.button.y, MU_MOUSE_LEFT);
                    break;
                case SDL_EVENT_MOUSE_BUTTON_UP:
                    mu_input_mouseup(window.mu_ctx, (int)window.event.button.x, (int)window.event.button.y, MU_MOUSE_LEFT);
                    break;
                case SDL_EVENT_KEY_DOWN:
                    if (window.event.key.key == SDLK_BACKSPACE) {
                        mu_input_keydown(window.mu_ctx, MU_KEY_BACKSPACE);
                    } else if (window.event.key.key == SDLK_RETURN || window.event.key.key == SDLK_KP_ENTER) {
                        mu_input_keydown(window.mu_ctx, MU_KEY_RETURN);
                    }
                    break;
                case SDL_EVENT_KEY_UP:
                    if (window.event.key.key == SDLK_BACKSPACE) {
                        mu_input_keyup(window.mu_ctx, MU_KEY_BACKSPACE);
                    } else if (window.event.key.key == SDLK_RETURN || window.event.key.key == SDLK_KP_ENTER) {
                        mu_input_keyup(window.mu_ctx, MU_KEY_RETURN);
                    }
                    break;
                case SDL_EVENT_TEXT_INPUT:
                    // W SDL3 event.text.text to wskaźnik na string UTF-8
                    if (window.event.text.text && window.event.text.text[0] != '\0') {
                        mu_input_text(window.mu_ctx, window.event.text.text);
                    }
                    break;
            }
        }
    }
    void Init() {
        window = {};
        window.window = SDL_CreateWindow("Pixel Editor", WIN_W, WIN_H, SDL_WINDOW_MAXIMIZED );
        window.renderer = SDL_CreateRenderer(window.window, NULL);
        window.mu_ctx = new mu_Context();
        mu_init(window.mu_ctx);
        window.paletteTexture = CreatePaletteTexture();
        window.font = TTF_OpenFont("Pix32.ttf", 12);
        if (window.font == NULL) {
            std::cout<< SDL_GetError() << std::endl;
        }
        if (window.font == NULL) SDL_GetError();
        UpdateGradientSquare();
        window.mu_ctx->text_width = text_width;
        window.mu_ctx->text_height = text_height;
        window.mu_ctx->style->colors[MU_COLOR_WINDOWBG] = mu_color(30, 30, 35, 255);
        SDL_StartTextInput(window.window);
    }

   void RenderMicroUI() {
    mu_Command* local_cmd = nullptr;

    while (mu_next_command(window.mu_ctx, &local_cmd)) {
        switch (local_cmd->type) {
            case MU_COMMAND_RECT: {
                SDL_FRect rect = {
                    (float)local_cmd->rect.rect.x,
                    (float)local_cmd->rect.rect.y,
                    (float)local_cmd->rect.rect.w,
                    (float)local_cmd->rect.rect.h
                };
                SDL_SetRenderDrawColor(window.renderer, local_cmd->rect.color.r, local_cmd->rect.color.g, local_cmd->rect.color.b, local_cmd->rect.color.a);
                SDL_RenderFillRect(window.renderer, &rect);
                break;
            }
            case MU_COMMAND_TEXT: {
                if (window.font) {
                    SDL_Color color = {
                        local_cmd->text.color.r,
                        local_cmd->text.color.g,
                        local_cmd->text.color.b,
                        local_cmd->text.color.a
                    };

                    SDL_Surface* surface = TTF_RenderText_Blended_Wrapped(window.font, local_cmd->text.str, 0, color, 0);

                    if (surface) {
                        SDL_Texture* texture = SDL_CreateTextureFromSurface(window.renderer, surface);

                        if (texture) {
                            float w, h;
                            SDL_GetTextureSize(texture, &w, &h);

                            SDL_FRect dst_rect = {
                                (float)local_cmd->text.pos.x,
                                (float)local_cmd->text.pos.y,
                                w,
                                h
                            };
                            SDL_RenderTexture(window.renderer, texture, nullptr, &dst_rect);
                            SDL_DestroyTexture(texture);
                        }
                        SDL_DestroySurface(surface);
                    }
                }
                break;
            }
            case MU_COMMAND_CLIP: {
                SDL_Rect clip = {
                    local_cmd->clip.rect.x,
                    local_cmd->clip.rect.y,
                    local_cmd->clip.rect.w,
                    local_cmd->clip.rect.h
                };
                SDL_SetRenderClipRect(window.renderer, &clip);
                break;
            }
            case MU_COMMAND_ICON: {
                if (local_cmd->icon.id == MU_ICON_MAX) {
                    SDL_FRect dstRect = {
                        static_cast<float>(local_cmd->icon.rect.x),
                        static_cast<float>(local_cmd->icon.rect.y),
                        static_cast<float>(local_cmd->icon.rect.w),
                        static_cast<float>(local_cmd->icon.rect.h)
                    };

                    SDL_RenderTexture(window.renderer, window.paletteTexture, nullptr, &dstRect);
                }
                break;
            }
        }
    }
    SDL_SetRenderClipRect(window.renderer, nullptr);
}
}