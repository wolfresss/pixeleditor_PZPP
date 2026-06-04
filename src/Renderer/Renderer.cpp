//
// Created by izakr on 01/06/2026.
//

#include "Renderer.h"
#include <iostream>
#include <ostream>
#include <SDL3_ttf/SDL_ttf.h>
#include <string.h>

namespace Render {

    int CANVAS_SIZE = 100;
    TTF_Font* g_font = nullptr;
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Event event;
    bool quit = false;
    UIConfig uiConfig = {};
    mu_Context* mu_ctx = nullptr;
    Document* CurrentFile = nullptr;
    SDL_Texture* paletteTexture = nullptr;
    void InitRenderSDL() {
        if (!SDL_Init(SDL_INIT_VIDEO)) {
            std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
            quit = true;
            return;
        }

        if (!TTF_Init()) {
            std::cerr << "TTF_Init Error: " << SDL_GetError() << std::endl;
            SDL_Quit();
            quit = true;
            return;
        }

        window = SDL_CreateWindow("Pixel Editor", WIN_W, WIN_H, 0);
        if (!window) {
            std::cerr << "Window Error: " << SDL_GetError() << std::endl;
            quit = true;
            return;
        }
        renderer = SDL_CreateRenderer(window, nullptr);
        if (!renderer) {
            std::cerr << "Renderer Error: " << SDL_GetError() << std::endl;
            quit = true;
            return;
        }

        g_font = TTF_OpenFont("Pix32.ttf", 16);
        if (!g_font) {
            std::cerr << "TTF_OpenFont Warning: " << SDL_GetError() << " (Using layout fallbacks)" << std::endl;
        }
    }

    void DestroyRenderSDL() {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);

        if (mu_ctx) {
            delete mu_ctx;
            mu_ctx = nullptr;
        }

        if (g_font) {
            TTF_CloseFont(g_font);
        }
        TTF_Quit();

        if (CurrentFile) {
            delete CurrentFile;
            CurrentFile = nullptr;
        }
        SDL_Quit();
    }

    SDL_Texture* CreatePaletteTexture(SDL_Renderer* renderer) {
        // TO DO: steaming for modification
        SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, 256, 256);
        if (!texture) {
            SDL_Log("Failed to create palette texture: %s", SDL_GetError());
        }
        return texture;
    }

    void UpdateGradientSquare() {
        if (!paletteTexture) return;

        Color Palette[256][256];

        UpdatePalette(Palette);

        SDL_UpdateTexture(paletteTexture, nullptr, Palette, 256 * sizeof(uint32_t));
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

        if (!mu_ctx) {
            std::cout << "[CRITICAL] Process_Events widzi mu_ctx jako NULL!" << std::endl;
            return;
        }
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_EVENT_QUIT:
                    quit = true;
                    break;
                case SDL_EVENT_MOUSE_MOTION:
                    mu_input_mousemove(mu_ctx, (int)event.motion.x, (int)event.motion.y);
                    break;
                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                    mu_input_mousedown(mu_ctx, (int)event.button.x, (int)event.button.y, MU_MOUSE_LEFT);
                    break;
                case SDL_EVENT_MOUSE_BUTTON_UP:
                    mu_input_mouseup(mu_ctx, (int)event.button.x, (int)event.button.y, MU_MOUSE_LEFT);
                    break;
                case SDL_EVENT_KEY_DOWN:
                    if (event.key.key == SDLK_BACKSPACE) mu_input_keydown(mu_ctx, MU_KEY_BACKSPACE);
                    if (event.key.key == SDLK_RETURN)    mu_input_keydown(mu_ctx, MU_KEY_RETURN);
                    break;
                case SDL_EVENT_KEY_UP:
                    if (event.key.key == SDLK_BACKSPACE) mu_input_keyup(mu_ctx, MU_KEY_BACKSPACE);
                    if (event.key.key == SDLK_RETURN)    mu_input_keyup(mu_ctx, MU_KEY_RETURN);
                    break;
                case SDL_EVENT_TEXT_INPUT:
                    mu_input_text(mu_ctx, event.text.text);
                    break;
            }
        }
    }

    void InitMicroUI() {
        std::unique_ptr<ITool> currentTool = std::make_unique<Pencil>();
        mu_ctx = new mu_Context();
        mu_init(mu_ctx);
        paletteTexture = CreatePaletteTexture(renderer);
        UpdateGradientSquare();
        mu_ctx->text_width = text_width;
        mu_ctx->text_height = text_height;
        mu_ctx->style->colors[MU_COLOR_WINDOWBG] = mu_color(30, 30, 35, 255);
    }

   void RenderMicroUI() {
    mu_Command* local_cmd = nullptr;

    while (mu_next_command(mu_ctx, &local_cmd)) {
        switch (local_cmd->type) {
            case MU_COMMAND_RECT: {
                SDL_FRect rect = {
                    (float)local_cmd->rect.rect.x,
                    (float)local_cmd->rect.rect.y,
                    (float)local_cmd->rect.rect.w,
                    (float)local_cmd->rect.rect.h
                };
                SDL_SetRenderDrawColor(renderer, local_cmd->rect.color.r, local_cmd->rect.color.g, local_cmd->rect.color.b, local_cmd->rect.color.a);
                SDL_RenderFillRect(renderer, &rect);
                break;
            }
            case MU_COMMAND_TEXT: {
                if (g_font) {
                    SDL_Color color = {
                        local_cmd->text.color.r,
                        local_cmd->text.color.g,
                        local_cmd->text.color.b,
                        local_cmd->text.color.a
                    };

                    SDL_Surface* surface = TTF_RenderText_Blended_Wrapped(g_font, local_cmd->text.str, 0, color, 0);

                    if (surface) {
                        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

                        if (texture) {
                            float w, h;
                            SDL_GetTextureSize(texture, &w, &h);

                            SDL_FRect dst_rect = {
                                (float)local_cmd->text.pos.x,
                                (float)local_cmd->text.pos.y,
                                w,
                                h
                            };
                            SDL_RenderTexture(renderer, texture, nullptr, &dst_rect);
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
                SDL_SetRenderClipRect(renderer, &clip);
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

                    // Draw the 4-corner blended layout box
                    SDL_RenderTexture(renderer, Render::paletteTexture, nullptr, &dstRect);
                }
                break;
            }
        }
    }
    SDL_SetRenderClipRect(renderer, nullptr);
}
}