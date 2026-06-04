#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <memory>
#include <iostream>
#include <string.h>
#include "Renderer/View.h"
#include "Renderer/Renderer.h"
#include <fstream>

using namespace std;

int main(int argc, char* argv[]) {
    Render::InitRenderSDL();
    Render::InitMicroUI();

    while (!Render::quit) {

        Render::Process_Events();


        SDL_SetRenderDrawColor(Render::renderer, 126, 140, 111, 255);
        SDL_RenderClear(Render::renderer);

      mu_begin(Render::mu_ctx);
      ProcessMainMenu(Render::mu_ctx);
      mu_end(Render::mu_ctx);

      Render::RenderMicroUI();
      //  SDL_SetRenderClipRect(Render::renderer, nullptr);
        SDL_RenderPresent(Render::renderer);

    }


    Render::DestroyRenderSDL();

    return 0;
}