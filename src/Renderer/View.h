#pragma once
#include <SDL3/SDL.h>
#include "../core/structs.h"
#include "../core/structs.h"
#include "../Tools/ITools.h"
#include "../Renderer/Config.h"

extern "C" {
#include "../../microui/src/microui.h"
}

//micro UI functions
void process_microui(mu_Context* ctx, Document& doc, std::unique_ptr<ITool>& currentTool, UIConfig &uiConfig);
int text_width(mu_Font font, const char *text, int len);
int text_height(mu_Font font);
void render_microui(SDL_Renderer* renderer, mu_Context* ctx);


//cancas reader SDL
class CanvasRenderer {
private:
    SDL_Texture* canvasTexture;
    int width;
    int height;
public:
    CanvasRenderer(SDL_Renderer* renderer, int w, int h);
    ~CanvasRenderer();
    void draw(SDL_Renderer* renderer, Document& doc, int OffsetX, int OffsetY, float scale);
};