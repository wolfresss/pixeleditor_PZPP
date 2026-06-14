#include "Renderer.h"
#include "../core/structs.h"
#include "ColorUtils.h"
#include "CanvasTools.h"
#include "../Resources/binary_icons.h"

#include <algorithm>
#include <iostream>
#include <SDL3_ttf/SDL_ttf.h>
#include <string.h>

bool hold = false;
float lastMouseX = 0.0f;
float lastMouseY = 0.0f;

namespace Render {
    WindowContext window;
    UIConfig uiConfig;
    HSV currentHSV{0, 255, 255};
    Color currentColor;

    int offsetX = 100;
    int offsetY = 100;
    float zoomScale = 8.0f;

    bool hold = false;
    float lastMouseX = 0.0f;
    float lastMouseY = 0.0f;

    SDL_Texture* CreatePaletteTexture() {
        SDL_Texture* texture = SDL_CreateTexture(window.renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, 256, 256);
        if (!texture) {
            SDL_Log("Failed to create palette texture: %s", SDL_GetError());
        }
        return texture;
    }

    void UpdateGradientSquare() {
        if (!window.paletteTexture) return;

        Color Palette[256][256];
        ColorUtils::UpdatePalette(currentHSV, Palette);

        SDL_UpdateTexture(window.paletteTexture, nullptr, Palette, 256 * sizeof(uint32_t));

        currentColor = ColorUtils::HSVToRGB(currentHSV, currentHSV.s, 255 - currentHSV.v);
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

    inline void InitAppIcons(SDL_Renderer* renderer) {
        int total_icons = sizeof(generated_icons_meta) / sizeof(PBM_SourceMeta);
        for (int i = 0; i < total_icons; i++) {
            my_ui_icons[i].icon_id = generated_icons_meta[i].id;
            my_ui_icons[i].texture = LoadIconFrom64BitArray(
                renderer,
                generated_icons_meta[i].bits,
                generated_icons_meta[i].w,
                generated_icons_meta[i].h
            );
        }
    }

    void FreeAppIcons() {
        for (int i = 0; i < MY_ICONS_SIZE; i++) {
            if (my_ui_icons[i].texture) {
                SDL_DestroyTexture(my_ui_icons[i].texture);
                my_ui_icons[i].texture = NULL;
            }
        }
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
                    if (hold && window.CurrentFile && window.mu_ctx->hover_root == nullptr) {

                        float brushSize = static_cast<float>(uiConfig.PixelSize);
                        float halfBrush = brushSize / 2.0f;

                        if (uiConfig.useInterpolation) {

                            float x1 = lastMouseX;
                            float y1 = lastMouseY;
                            float x2 = window.event.motion.x;
                            float y2 = window.event.motion.y;

                            float dx = x2 - x1;
                            float dy = y2 - y1;
                            float steps = std::max(std::abs(dx), std::abs(dy));

                            for (int i = 0; i <= steps; i++) {
                                float t = (steps == 0) ? 0.0f : (float)i / steps;
                                float currX = x1 + dx * t;
                                float currY = y1 + dy * t;


                                CanvasTools::ProcessTool(uiConfig.selectedTool, *window.CurrentFile, currX - halfBrush, currY - halfBrush, false, currentColor, offsetX, offsetY, zoomScale);
                            }
                        } else {
                            CanvasTools::ProcessTool(uiConfig.selectedTool, *window.CurrentFile, window.event.motion.x - halfBrush, window.event.motion.y - halfBrush, false, currentColor, offsetX, offsetY, zoomScale);
                        }

                        lastMouseX = window.event.motion.x;
                        lastMouseY = window.event.motion.y;
                    }
                    mu_input_mousemove(window.mu_ctx, (int)window.event.motion.x, (int)window.event.motion.y);
                    break;

                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                    if (window.event.button.button == SDL_BUTTON_LEFT) {
                        hold = true;
                        lastMouseX = window.event.button.x;
                        lastMouseY = window.event.button.y;

                        if (window.CurrentFile && window.mu_ctx->hover_root == nullptr) {

                            float halfBrush = static_cast<float>(uiConfig.PixelSize) / 2.0f;
                            CanvasTools::ProcessTool(uiConfig.selectedTool, *window.CurrentFile, window.event.button.x - halfBrush, window.event.button.y - halfBrush, true, currentColor, offsetX, offsetY, zoomScale);
                        }
                    }
                    mu_input_mousedown(window.mu_ctx, (int)window.event.button.x, (int)window.event.button.y, MU_MOUSE_LEFT);
                    break;

                case SDL_EVENT_MOUSE_BUTTON_UP:
                    hold = false;
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
                    if (window.event.text.text && window.event.text.text[0] != '\0') {
                        mu_input_text(window.mu_ctx, window.event.text.text);
                    }
                    break;
                case SDL_EVENT_MOUSE_WHEEL:
                    float wheel_delta = window.event.wheel.y;
                    if (wheel_delta > 0.0f) {
                        uiConfig.scale *= 1.1f;
                    } else if (wheel_delta < 0.0f) {
                        uiConfig.scale /= 1.1f;
                    }

                    if (uiConfig.scale < 1.0f)  uiConfig.scale = 1.0f;
                    if (uiConfig.scale > 40.0f) uiConfig.scale = 40.0f;

                    zoomScale = uiConfig.scale;
                    break;
            }
        }
    }

    void Init() {
        window = {};
        uiConfig = {};
        window.window = SDL_CreateWindow("Pixel Editor", WIN_W, WIN_H, SDL_WINDOW_MAXIMIZED);
        window.renderer = SDL_CreateRenderer(window.window, NULL);
        Render::InitAppIcons(window.renderer);
        window.mu_ctx = new mu_Context();
        mu_init(window.mu_ctx);

        uiConfig.leftPanelWidth = WIN_W / 8;
        uiConfig.rightPanelWidth = WIN_W / 8;
        window.paletteTexture = CreatePaletteTexture();

        window.font = TTF_OpenFont("Pix32.ttf", 12);
        if (window.font == NULL) {
            std::cout << SDL_GetError() << std::endl;
        }
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
                                SDL_FRect dst_rect = { (float)local_cmd->text.pos.x, (float)local_cmd->text.pos.y, w, h };
                                SDL_RenderTexture(window.renderer, texture, nullptr, &dst_rect);
                                SDL_DestroyTexture(texture);
                            }
                            SDL_DestroySurface(surface);
                        }
                    }
                    break;
                }
                case MU_COMMAND_CLIP: {
                    SDL_Rect clip = { local_cmd->clip.rect.x, local_cmd->clip.rect.y, local_cmd->clip.rect.w, local_cmd->clip.rect.h };
                    SDL_SetRenderClipRect(window.renderer, &clip);
                    break;
                }
                case MU_COMMAND_ICON: {
                    if (local_cmd->icon.id == MU_ICON_MAX) {
                        SDL_FRect dstRect = { (float)local_cmd->icon.rect.x, (float)local_cmd->icon.rect.y, (float)local_cmd->icon.rect.w, (float)local_cmd->icon.rect.h };
                        SDL_RenderTexture(window.renderer, window.paletteTexture, nullptr, &dstRect);

                        float dotX = dstRect.x + (static_cast<float>(currentHSV.s) / 255.0f) * dstRect.w;
                        float dotY = dstRect.y + (1.0f - (static_cast<float>(currentHSV.v) / 255.0f)) * dstRect.h;


                        SDL_FRect outerDot = { dotX - 4.0f, dotY - 4.0f, 8.0f, 8.0f };
                        SDL_SetRenderDrawColor(window.renderer, 0, 0, 0, 255); // czarny
                        SDL_RenderFillRect(window.renderer, &outerDot);

                        SDL_FRect innerDot = { dotX - 2.0f, dotY - 2.0f, 4.0f, 4.0f };
                        SDL_SetRenderDrawColor(window.renderer, 255, 255, 255, 255); // biały
                        SDL_RenderFillRect(window.renderer, &innerDot);
                    }
                    else if (local_cmd->icon.id > MU_ICON_MAX) {
                        SDL_Texture* custom_icon = GetTextureByIconID(local_cmd->icon.id);
                        if (custom_icon) {
                            float icon_w = 50.0f;
                            float icon_h = 50.0f;
                            float dst_x = local_cmd->icon.rect.x + (local_cmd->icon.rect.w - icon_w) / 2.0f;
                            float dst_y = local_cmd->icon.rect.y + (local_cmd->icon.rect.h - icon_h) / 2.0f;
                            SDL_FRect dstRect = { dst_x, dst_y, icon_w, icon_h };
                            SDL_RenderTexture(window.renderer, custom_icon, nullptr, &dstRect);
                        }
                    }
                    break;
                }
            }
        }
        SDL_SetRenderClipRect(window.renderer, nullptr);
    }
}