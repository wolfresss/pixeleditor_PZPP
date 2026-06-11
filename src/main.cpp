#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <memory>
#include <iostream>
#include <string.h>
#include <fstream>
#include "Renderer/View.h"
#include "Renderer/Renderer.h"

using namespace std;


void ProcessMainMenu(mu_Context* ctx);
void ProcessLeftBar(mu_Context* ctx);

int main(int argc, char* argv[]) {

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    if (TTF_Init() < 0) {
        std::cout << "TTF_Init Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    Render::Init();
    while (!Render::window.shouldQuit) {

        Render::Process_Events();

        mu_begin(Render::window.mu_ctx);
        ProcessMainMenu(Render::window.mu_ctx);
        ProcessLeftBar(Render::window.mu_ctx);
        mu_end(Render::window.mu_ctx);

        SDL_SetRenderDrawColor(Render::window.renderer, 95, 100, 105, 255);
        SDL_RenderClear(Render::window.renderer);


        if (Render::window.CurrentFile && Render::window.canvasRenderer) {
            float zoomScale = Render::uiConfig.scale;

            int windowWidth = Render::WIN_W;
            int windowHeight = Render::WIN_H;

            int docWidth = Render::window.CurrentFile->width;
            int docHeight = Render::window.CurrentFile->height;


            int offsetX = (windowWidth - (docWidth * zoomScale)) / 2 + 100;
            int offsetY = (windowHeight - (docHeight * zoomScale)) / 2;

            Render::window.canvasRenderer->draw(
                Render::window.renderer,
                *Render::window.CurrentFile,
                offsetX,
                offsetY,
                zoomScale
            );
        }

        Render::RenderMicroUI();


        SDL_RenderPresent(Render::window.renderer);
    }


    TTF_Quit();
    SDL_Quit();

    return 0;
}