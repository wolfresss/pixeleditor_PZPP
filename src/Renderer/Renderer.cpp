//
// Created by izakr on 01/06/2026.
//

#include "Renderer.h"
#include "../core/structs.h"

#include <cmath>
#include <iostream>
#include <ostream>
#include <SDL3_ttf/SDL_ttf.h>
#include <string.h>
#include "../Resources/binary_icons.h"

bool hold = false;
float lastMouseX = 0.0f;
float lastMouseY = 0.0f;

namespace Render {
    WindowContext window;
    UIConfig uiConfig;
    HSV currentHSV{0, 255, 255};
    Color currentColor;
    int offsetX;
     int offsetY;
     float zoomScale;

    //TO DO: add line interpolation
    void executePencil(Document& doc, int x, int y, int PixelSize, Color drawColor) {
        auto& layer = doc.activeLayer();
        if (x >= 0 && x < layer.width && y >= 0 && y < layer.height) {
            layer.setPixels(x, y,drawColor);
        }
    }
    void executeRubber(Document& doc, int x, int y, int PixelSize) {
        auto& layer = doc.activeLayer();

        if (x >= 0 && x < layer.width && y >= 0 && y < layer.height) {
            // Gumka ustawia kolor na w pełni przezroczysty
            // TO DO: Kontrola przezroczystości gumki
            Color drawColor = {0, 0, 0, 0};
            layer.setPixels(x, y, drawColor);

        }
    }

    void drawLineOnCanvas(Document& doc, int x0, int y0, int x1, int y1, ToolType selectedTool) {
        int dx = std::abs(x1 - x0);
        int dy = std::abs(y1 - y0);
        int sx = (x0 < x1) ? 1 : -1;
        int sy = (y0 < y1) ? 1 : -1;
        int err = dx - dy;

        while (true) {
            switch (selectedTool) {
                case PENCIL:
                    executePencil(doc, x0, y0, 1, currentColor);
                    break;
                case RUBBER:
                    executeRubber(doc, x0, y0, 1);
                    break;
                default:
                    break;
            }

            if (x0 == x1 && y0 == y1) break;
            int e2 = 2 * err;
            if (e2 > -dy) {
                err -= dy;
                x0 += sx;
            }
            if (e2 < dx) {
                err += dx;
                y0 += sy;
            }
        }
    }


    void ProcessTool(ToolType selectedTool, Document& doc, float mouseX, float mouseY, bool isFirstClick)
    {
        float canvasLeft   = (float)offsetX;
        float canvasTop    = (float)offsetY;

        int currentPixelX = static_cast<int>((mouseX - canvasLeft) / zoomScale);
        int currentPixelY = static_cast<int>((mouseY - canvasTop) / zoomScale);

        int prevPixelX = static_cast<int>((lastMouseX - canvasLeft) / zoomScale);
        int prevPixelY = static_cast<int>((lastMouseY - canvasTop) / zoomScale);


        if (isFirstClick) {
            switch (selectedTool) {
                case PENCIL: executePencil(doc, currentPixelX, currentPixelY, 1, currentColor); break;
                case RUBBER: executeRubber(doc, currentPixelX, currentPixelY, 1); break;
                default: break;
            }
        } else {

            drawLineOnCanvas(doc, prevPixelX, prevPixelY, currentPixelX, currentPixelY, selectedTool);
        }
        lastMouseX = mouseX;
        lastMouseY = mouseY;
    }
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


    void InitAppIcons(SDL_Renderer* renderer) {
        my_ui_icons[0].icon_id = ICON_PBM_PENCIL;
        my_ui_icons[0].texture = LoadIconFrom64BitArray(renderer, pencil_icon_bits, 50, 50);

        my_ui_icons[1].icon_id = ICON_PBM_RUBBER;
        my_ui_icons[1].texture = LoadIconFrom64BitArray(renderer, rubber_icon_bits, 50, 50);
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
                    if (hold && window.CurrentFile) {
                        // isFirstClick = false, ponieważ mysz jest już przesuwana i trzymana
                        ProcessTool(uiConfig.selectedTool, *window.CurrentFile, window.event.motion.x, window.event.motion.y, false);
                    }
                    mu_input_mousemove(window.mu_ctx, (int)window.event.motion.x, (int)window.event.motion.y);
                    break;

                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                    if (window.event.button.button == SDL_BUTTON_LEFT) {
                        hold = true;

                        lastMouseX = window.event.button.x;
                        lastMouseY = window.event.button.y;

                        if (window.CurrentFile) {
                            ProcessTool(uiConfig.selectedTool, *window.CurrentFile, window.event.button.x, window.event.button.y, true);
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
                    if (window.event.wheel.y > 0.0f) {
                        uiConfig.scale *= 1.2f;
                    } else if (window.event.wheel.y < 0.0f) {
                        uiConfig.scale /= 1.2f;
                    }


                    if (uiConfig.scale < 0.5f)  uiConfig.scale = 0.5f;
                    if (uiConfig.scale > 32.0f) uiConfig.scale = 32.0f;
                    break;

            }
        }
    }
    void Init() {
        window = {};
        uiConfig = {};
        window.window = SDL_CreateWindow("Pixel Editor", WIN_W, WIN_H, SDL_WINDOW_MAXIMIZED );
        window.renderer = SDL_CreateRenderer(window.window, NULL);
        InitAppIcons(window.renderer);
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





double POS() {
    double value = std::fmod(static_cast<double>(currentHSV.h) / 60.0, 2.0) - 1.0;
    return std::abs(value);
}

Color HSVToRGB(uint8_t pixel_x, uint8_t pixel_y) {
    // x_pct i y_pct to wartości od 0.0 do 1.0 wyciągnięte z pozycji piksela na kwadracie
    double s = pixel_x / 255.0;
    double v = (255 - pixel_y) / 255.0;

    double c = v * s;
    double x = c * (1.0 - POS());
    double m = v - c;

    double r_ = 0, g_ = 0, b_ = 0;
    if (currentHSV.h < 60)        { r_ = c; g_ = x; b_ = 0; }
    else if (currentHSV.h < 120)  { r_ = x; g_ = c; b_ = 0; }
    else if (currentHSV.h < 180)  { r_ = 0; g_ = c; b_ = x; }
    else if (currentHSV.h < 240)  { r_ = 0; g_ = x; b_ = c; }
    else if (currentHSV.h < 300)  { r_ = x; g_ = 0; b_ = c; }
    else if (currentHSV.h <= 360) { r_ = c; g_ = 0; b_ = x; }

    return Color{
        static_cast<uint8_t>((r_ + m) * 255.0),
        static_cast<uint8_t>((g_ + m) * 255.0),
        static_cast<uint8_t>((b_ + m) * 255.0),
        255 // Alfa
    };
}

void UpdatePalette(Color (&Palette)[256][256]) {
    for (int y = 0; y < 256; y++) {
        for (int x = 0; x < 256; x++) {
            Palette[y][x] = HSVToRGB(x, y);
        }
    }
}

mu_Color HSVTo_mu_Color(uint8_t pixel_x, uint8_t pixel_y) {
    // x_pct i y_pct to wartości od 0.0 do 1.0 wyciągnięte z pozycji piksela na kwadracie
    double s = pixel_x / 255.0;
    double v = (255 - pixel_y) / 255.0;

    double c = v * s;
    double x = c * (1.0 - POS());
    double m = v - c;

    double r_ = 0, g_ = 0, b_ = 0;
    if (currentHSV.h < 60)        { r_ = c; g_ = x; b_ = 0; }
    else if (currentHSV.h < 120)  { r_ = x; g_ = c; b_ = 0; }
    else if (currentHSV.h < 180)  { r_ = 0; g_ = c; b_ = x; }
    else if (currentHSV.h < 240)  { r_ = 0; g_ = x; b_ = c; }
    else if (currentHSV.h < 300)  { r_ = x; g_ = 0; b_ = c; }
    else if (currentHSV.h <= 360) { r_ = c; g_ = 0; b_ = x; }

    return mu_Color{
        static_cast<uint8_t>((r_ + m) * 255.0),
        static_cast<uint8_t>((g_ + m) * 255.0),
        static_cast<uint8_t>((b_ + m) * 255.0),
        255 // Alfa
    };
}


}