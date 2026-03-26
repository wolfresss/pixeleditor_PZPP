#include <SDL3/SDL.h>
#include <memory>
#include <iostream>
#include <string.h>

#include "core/structs.h"
#include "Tools/ITools.h"
#include "Renderer/View.h"

// TO DO : zmienic zmienne aby main uzywal structa z config
#include "Renderer/Config.h"
///////////////////////////////////
extern "C" {
    #include "../microui/src/microui.h"
}

// MICROUI FONT HELPERS
// MicroUI potrzebuje tych funkcji, aby wiedzieć jak układać przyciski i tekst
int text_width(mu_Font font, const char *text, int len) {
    if (len == -1) len = (int)strlen(text);
    return len * 8; // Przyjmujemy 8px na znak (tymczasowo bez SDL_ttf)
}

int text_height(mu_Font font) {
    return 16; // 16px wysokości linii na razie
}

// Most pomiędzy SDL a microui
void render_microui(SDL_Renderer* renderer, mu_Context* ctx) {
    mu_Command *cmd = NULL;
    while (mu_next_command(ctx, &cmd)) {
        switch (cmd->type) {
            case MU_COMMAND_RECT: {
                //TO DO:  Naprawa błędu: Tworzymy zmienną na stosie, zamiast obiektu tymczasowego
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
                // Na razie rysujemy tylko tło pod tekst, dopóki nie dodam SDL_ttf
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


// UI LOGIC (Ports and Adapters)
void process_microui(mu_Context* ctx, Document& doc, std::unique_ptr<ITool>& currentTool) {
    mu_begin(ctx);

    mu_Real value = 50.0f;
    char charvalue[32];
    int widths[1] = {-1};

    if (mu_begin_window(ctx, "Toolbar", mu_rect(10, 10, 160, 150))) {

        mu_layout_row(ctx, 1, widths, 0);

        if (mu_button(ctx, "Pencil")) {
            currentTool = std::make_unique<Pencil>();
        }
        if (mu_button(ctx, "Eraser")) {
            currentTool = std::make_unique<Eraser>();
        }

        mu_layout_row(ctx, 1, widths, 0);
        if (mu_slider_ex(ctx, &value, 0, 100, 1, charvalue, sizeof(charvalue))) {
            doc.currentLayerOpacity = value;  // przykład
            doc.isModified = true;            // oznacz zmienione
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
    const int CANVAS_SIZE = 600;

    SDL_Window* window = SDL_CreateWindow("Pixel Editor", WIN_W, WIN_H, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);

    // DOMENA
    Document doc(CANVAS_SIZE, CANVAS_SIZE);
    doc.addLayer("Background");
    std::unique_ptr<ITool> currentTool = std::make_unique<Pencil>();

    // VIEW / ADAPTERS
    CanvasRenderer canvasView(renderer, CANVAS_SIZE, CANVAS_SIZE);

    // Do srodkowania canvas
    int canvasOffsetX = (WIN_W - CANVAS_SIZE) / 2;
    int canvasOffsetY = (WIN_H - CANVAS_SIZE) / 2;
    // MICROUI SETUP
    mu_Context* mu_ctx = new mu_Context();
    mu_init(mu_ctx);
    mu_ctx->text_width = text_width;
    mu_ctx->text_height = text_height;

    bool quit = false;
    SDL_Event event;

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            // INPUT ADAPTER (Przekazywanie do MicroUI)
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

            // 2. LOGIKA RYSOWANIA (Tylko jeśli myszka nie jest nad UI)
            // Sprawdzamy hover_root (czy mysz jest nad jakimkolwiek oknem UI)
            if (!mu_ctx->hover_root) {
                float mX, mY;
                Uint32 buttons = SDL_GetMouseState(&mX, &mY);

                if (buttons & SDL_BUTTON_LMASK) {
                    // Mapowanie współrzędnych okna na współrzędne canvasu
                    float localX = mX - canvasOffsetX;
                    float localY = mY - canvasOffsetY;

                    int canvasX = (int)localX;
                    int canvasY = (int)localY;;

                    if (canvasX >= 0 && canvasX < CANVAS_SIZE && canvasY >= 0 && canvasY < CANVAS_SIZE) {
                        currentTool->execute(doc, canvasX, canvasY);
                    }
                }
            }
        }

        //  UI LOGIC (Budowanie interfejsu)
        process_microui(mu_ctx, doc, currentTool);

        //  RENDERING
        SDL_SetRenderDrawColor(renderer, 35, 35, 38, 255); // Tło edytora
        SDL_RenderClear(renderer);

        // Rysujemy płótno (Piksele z Dokumentu)
        canvasView.draw(renderer, doc, canvasOffsetX, canvasOffsetY);

        // Rysujemy MicroUI na wierzchu (Przycisk i okna)
        render_microui(renderer, mu_ctx);

        SDL_RenderPresent(renderer);
    }

    delete mu_ctx;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}