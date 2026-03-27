#include "View.h"

CanvasRenderer::CanvasRenderer(SDL_Renderer* renderer, int w, int h)
    : width(w), height(h) {

    canvasTexture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_STREAMING,
        w, h
    );

    SDL_SetTextureScaleMode(canvasTexture, SDL_SCALEMODE_NEAREST);

    if (!canvasTexture) {
        SDL_Log("Blad tworzenia tekstury: %s", SDL_GetError());
    }
}
void CanvasRenderer::draw(SDL_Renderer* renderer, Document& doc, int OffsetX, int OffsetY, float scale) {
    const auto& compositeData = doc.composite();

    SDL_UpdateTexture(canvasTexture, nullptr, compositeData.data(), width * sizeof(uint32_t));

    SDL_FRect dst = {
        (float)OffsetX,
        (float)OffsetY,
        width * scale,
        height * scale
    };

    SDL_RenderTexture(renderer, canvasTexture, nullptr, &dst);
}

CanvasRenderer::~CanvasRenderer() {
    if (canvasTexture) {
        SDL_DestroyTexture(canvasTexture);
    }
}

