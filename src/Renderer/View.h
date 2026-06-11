#pragma once
#include <memory>
#include <SDL3/SDL.h>
#include "../core/structs.h"
#include "../file/file.h"
extern "C" {
#include "../../microui/src/microui.h"
}
//micro UI functions (OLD)
int text_width(mu_Font font, const char *text, int len);
int text_height(mu_Font font);
void RenderMicroUI(SDL_Renderer* renderer, mu_Context* ctx);
void MainLoop(mu_Context* ctx, Document &CurrentFile);
//cancas reader SDL
class CanvasRenderer {
    SDL_Texture* canvasTexture;
    int width;
    int height;
public:
    CanvasRenderer(SDL_Renderer* renderer, int w, int h);
    ~CanvasRenderer();
    void draw(SDL_Renderer* renderer, Document& doc, int OffsetX, int OffsetY, float scale);
};
//UI
void ProcessMainMenu(mu_Context* ctx);
void ProcessLeftBar(mu_Context* ctx);