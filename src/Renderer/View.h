#pragma once
#include <SDL3/SDL.h>
#include "../core/structs.h"

class CanvasRenderer {
private:
    SDL_Texture* canvasTexture;
    int width;
    int height;
public:
    CanvasRenderer(SDL_Renderer* renderer, int w, int h);
    ~CanvasRenderer();
    void draw(SDL_Renderer* renderer, Document& doc, int OffsetX, int OffsetY );
};