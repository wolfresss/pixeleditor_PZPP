#include "CanvasRenderer.h"

CanvasRenderer::CanvasRenderer(SDL_Renderer* renderer, int w, int h)
    : width(w), height(h), canvasTexture(nullptr) {

    canvasTexture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_STREAMING,
        w, h
    );

    if (!canvasTexture) {
        SDL_Log("Blad tworzenia tekstury płótna: %s", SDL_GetError());
    } else {
        SDL_SetTextureScaleMode(canvasTexture, SDL_SCALEMODE_NEAREST);
    }
}

void CanvasRenderer::draw(SDL_Renderer* renderer, Document& doc, int OffsetX, int OffsetY, float scale) {
    if (!canvasTexture) return;

    const auto& compositeData = doc.composite();

    SDL_UpdateTexture(canvasTexture, nullptr, compositeData.data(), doc.width * sizeof(uint32_t));

    SDL_FRect dst = {
        (float)OffsetX,
        (float)OffsetY,
        doc.width * scale,
        doc.height * scale
    };

    SDL_RenderTexture(renderer, canvasTexture, nullptr, &dst);
}

CanvasRenderer::~CanvasRenderer() {
    if (canvasTexture) {
        SDL_DestroyTexture(canvasTexture);
    }
}