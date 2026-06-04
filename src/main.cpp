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


    // Inside main.cpp loop:
    while (!Render::quit) {
        Render::Process_Events(); // Now it quickly clears inputs and returns!

        mu_begin(Render::mu_ctx);
        // ... UI markup definitions ...
        mu_end(Render::mu_ctx);

        // This runs EVERY frame, drawing over the black void
        SDL_SetRenderDrawColor(Render::renderer, 126, 140, 111, 1); // Your custom color background
        SDL_RenderClear(Render::renderer);

        Render::RenderMicroUI(); // Draws UI widgets over the color background

        SDL_RenderPresent(Render::renderer); // Flips the backbuffer onto the screen!
    }
    // we don't need document at the start
   // CurrentFile = new Document(CANVAS_SIZE, CANVAS_SIZE, "Untitled", std::vector<Color>());
  //  CurrentFile->addLayer("Background", { 1, 1, 1, 1 });



    // VIEW / ADAPTERS
  //  CanvasRenderer canvasView(renderer, CANVAS_SIZE, CANVAS_SIZE);

    // MICROUI SETUP



// stays in Main !! -----------------
  /*  int canvasOffsetX = (WIN_W - (int)(CANVAS_SIZE * uiConfig.scale)) / 2;
    int canvasOffsetY = (WIN_H - (int)(CANVAS_SIZE * uiConfig.scale)) / 2;

    if (!mu_ctx->hover_root && CurrentFile) {
        float mX, mY;
        Uint32 buttons = SDL_GetMouseState(&mX, &mY);

        if (buttons & SDL_BUTTON_LMASK) {
            float localX = (mX - canvasOffsetX) / uiConfig.scale;
            float localY = (mY - canvasOffsetY) / uiConfig.scale;

            int canvasX = (int)localX;
            int canvasY = (int)localY;

            if (canvasX >= 0 && canvasX < CANVAS_SIZE &&
                canvasY >= 0 && canvasY < CANVAS_SIZE) {
                uiConfig.currentTool->execute(*CurrentFile, canvasX, canvasY, (int)uiConfig.PixelSize, {uiConfig.R, uiConfig.G, uiConfig.B, uiConfig.A});
                }
        }
    }
*/
  //  mu_begin(mu_ctx);
// - smth smth
  //  mu_end(mu_ctx);
//------------------------------------------------------------



        // RENDER



     /*   if (CurrentFile) {
            canvasView.draw(renderer, *CurrentFile, canvasOffsetX, canvasOffsetY, uiConfig.scale);
        }
*/

        //RenderMicroUI(renderer, mu_ctx);

       // SDL_RenderPresent(renderer);





 //   delete mu_ctx;
 //   Render::DestroyRenderSDL();

    Render::DestroyRenderSDL();
    delete Render::mu_ctx;

    return 0;
}