#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <memory>
#include <iostream>
#include <string.h>
#include "Renderer/View.h"
#include "Renderer/renderer.h"
#include <fstream>

#include "Renderer/Renderer.h"


using namespace std;

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    Render::Init();
    while (!Render::window.shouldQuit) {

        Render::Process_Events();
        SDL_SetRenderDrawColor(Render::window.renderer, 126, 140, 111, 255);
        SDL_RenderClear(Render::window.renderer);

      mu_begin(Render::window.mu_ctx);
      ProcessMainMenu(Render::window.mu_ctx);
        ProcessLeftBar(Render::window.mu_ctx);
      mu_end(Render::window.mu_ctx);

      Render::RenderMicroUI();
      //  SDL_SetRenderClipRect(Render::window.window.renderer, nullptr);
        SDL_RenderPresent(Render::window.renderer);

    }

    return 0;
}