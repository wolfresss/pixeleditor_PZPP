#include <SDL3/SDL.h>
#include <memory>
#include <iostream>
#include <string.h>

#include "core/structs.h"
#include "Tools/ITools.h"
#include "Renderer/View.h"
#include "Renderer/Config.h"

UIConfig uiConfig = { 1.0f, 1 }; // scale domyślnie 1, PixelSize domyślnie 1

extern "C" {
    #include "../microui/src/microui.h"
}

// MICROUI FONT HELPERS
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

// UI LOGIC
void process_microui(mu_Context* ctx, Document& doc, std::unique_ptr<ITool>& currentTool) {
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
        if (mu_slider_ex(ctx, &uiConfig.PixelSize, 1, 100, 1, charvalue, sizeof(charvalue))) {
            doc.PixelSize = uiConfig.PixelSize;
        }

        // Scale slider
        mu_layout_row(ctx, 1, widths, 0);
        float value = uiConfig.scale;
        if (mu_slider_ex(ctx, &value, 0.1f, 20.0f, 0.1f, charvalue, sizeof(charvalue))) {
            uiConfig.scale = value;
        }

        mu_label(ctx, "Layers:");
        for (size_t i = 0; i < doc.layers.size(); i++) {
            mu_text(ctx, doc.layers[i].name.c_str());
        }

        mu_end_window(ctx);
    }

    mu_end(ctx);
}

int main(int argc, char* argv[]) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    const int WIN_W = 800;
    const int WIN_H = 800;
    const int CANVAS_SIZE = 100;

    SDL_Window* window = SDL_CreateWindow("Pixel Editor", WIN_W, WIN_H, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);

    // DOMENA
    Document doc(CANVAS_SIZE, CANVAS_SIZE);
    doc.addLayer("Background");
    std::unique_ptr<ITool> currentTool = std::make_unique<Pencil>();

    // VIEW / ADAPTERS
    CanvasRenderer canvasView(renderer, CANVAS_SIZE, CANVAS_SIZE);

    // MICROUI SETUP
    mu_Context* mu_ctx = new mu_Context();
    mu_init(mu_ctx);
    mu_ctx->text_width = text_width;
    mu_ctx->text_height = text_height;

    bool quit = false;
    SDL_Event event;

    while (!quit) {
        // Eventy
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
            }
        }

        // Przeliczenie offsetów w każdej klatce
        int canvasOffsetX = (WIN_W - (int)(CANVAS_SIZE * uiConfig.scale)) / 2;
        int canvasOffsetY = (WIN_H - (int)(CANVAS_SIZE * uiConfig.scale)) / 2;

        // LOGIKA RYSOWANIA
        if (!mu_ctx->hover_root) {
            float mX, mY;
            Uint32 buttons = SDL_GetMouseState(&mX, &mY);

            if (buttons & SDL_BUTTON_LMASK) {
                float localX = (mX - canvasOffsetX) / uiConfig.scale;
                float localY = (mY - canvasOffsetY) / uiConfig.scale;

                int canvasX = (int)localX;
                int canvasY = (int)localY;

                if (canvasX >= 0 && canvasX < CANVAS_SIZE &&
                    canvasY >= 0 && canvasY < CANVAS_SIZE) {
                    currentTool->execute(doc, canvasX, canvasY, (int)uiConfig.PixelSize);
                }
            }
        }

        // UI LOGIC
        process_microui(mu_ctx, doc, currentTool);

        // RENDER
        SDL_SetRenderDrawColor(renderer, 35, 35, 38, 255); // tło edytora
        SDL_RenderClear(renderer);

        canvasView.draw(renderer, doc, canvasOffsetX, canvasOffsetY, uiConfig.scale);

        render_microui(renderer, mu_ctx);

        SDL_RenderPresent(renderer);
    }

    delete mu_ctx;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}