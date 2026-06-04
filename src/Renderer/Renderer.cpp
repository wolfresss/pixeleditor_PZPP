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
    mu_Context* mu_ctx = nullptr;
    mu_Command* cmd = nullptr;
    UIConfig uiConfig;
    Document* CurrentFile = nullptr;
    void InitRenderSDL() {

        if (!SDL_Init(SDL_INIT_VIDEO)) {     SDL_SetRenderDrawColor(renderer, 35, 35, 38, 255); // tło edytora
        SDL_RenderClear(renderer);
            std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        }

        if (!TTF_Init()) {
            std::cerr << "TTF_Init Error: " << SDL_GetError() << std::endl;
            SDL_Quit();
        }

        window = SDL_CreateWindow("Pixel Editor", WIN_W, WIN_H, 0);
        renderer = SDL_CreateRenderer(window, nullptr);

        g_font = TTF_OpenFont("Pix32.ttf", 16);
        if (!g_font) {
            std::cerr << "TTF_OpenFont Error: " << SDL_GetError() << std::endl;
        }
        SDL_RenderClear(renderer);

    }

    void DestroyRenderSDL() {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);

        if (g_font) {
            TTF_CloseFont(g_font);
        }
        TTF_Quit();

        if (CurrentFile) {
            delete CurrentFile;
        }
        SDL_Quit();
    }

    void Process_Events() {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_EVENT_QUIT:
                    quit = true;
                    break;
                case SDL_EVENT_MOUSE_MOTION:
                    mu_input_mousemove(mu_ctx, (int)event.motion.x, (int)event.motion.y);
                    break;
                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        mu_input_mousedown(mu_ctx, (int)event.button.x, (int)event.button.y, MU_MOUSE_LEFT);
                    }
                    break;
                case SDL_EVENT_MOUSE_BUTTON_UP:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        mu_input_mouseup(mu_ctx, (int)event.button.x, (int)event.button.y, MU_MOUSE_LEFT);
                    }
                    break;
                case SDL_EVENT_MOUSE_WHEEL:
                    mu_input_scroll(mu_ctx, (int)event.wheel.x * 30, (int)event.wheel.y * -30);
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
        mu_ctx = new mu_Context();
        mu_init(mu_ctx);
        mu_ctx->text_width = text_width;
        mu_ctx->text_height = text_height;
        }




    void RenderMicroUI() {
    while (mu_next_command(mu_ctx, &cmd)) {
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
                if (g_font) {
                    SDL_Color color = {
                        cmd->text.color.r,
                        cmd->text.color.g,
                        cmd->text.color.b,
                        cmd->text.color.a
                    };

                    SDL_Surface* surface = TTF_RenderText_Blended_Wrapped(g_font, cmd->text.str, 0, color, 0);

                    if (surface) {
                        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

                        if (texture) {
                            float w, h;
                            SDL_GetTextureSize(texture, &w, &h);

                            SDL_FRect dst_rect = {
                                (float)cmd->text.pos.x,
                                (float)cmd->text.pos.y,
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
}