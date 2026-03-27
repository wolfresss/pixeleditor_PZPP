#include <SDL3/SDL.h>
#include <memory>
#include <iostream>
#include <string.h>
#include "Renderer/View.h"

UIConfig uiConfig;
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
                    currentTool->execute(doc, canvasX, canvasY, (int)uiConfig.PixelSize, {uiConfig.R, uiConfig.G, uiConfig.B, uiConfig.A});
                }
            }
        }

        // UI LOGIC
        process_microui(mu_ctx, doc, currentTool, uiConfig);

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