#include "View.h"


CanvasRenderer::CanvasRenderer(SDL_Renderer* renderer, int w, int h)
    : width(w), height(h) {

    canvasTexture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_STREAMING,
        w, h
    );

    if (!canvasTexture) {
        SDL_Log("Blad tworzenia tekstury: %s", SDL_GetError());
    }
}

CanvasRenderer::~CanvasRenderer() {
    if (canvasTexture) {
        SDL_DestroyTexture(canvasTexture);
    }
}

// Implementacja metody draw
void CanvasRenderer::draw(SDL_Renderer* renderer, Document& doc) {
    // Pobieramy dane z domeny (zwróć uwagę na const auto& - to optymalizacja)
    const auto& compositeData = doc.composite();

    // Kopiujemy do tekstury GPU
    SDL_UpdateTexture(
        canvasTexture,
        nullptr,
        compositeData.data(),
        width * sizeof(uint32_t)
    );

    // Wyświetlamy na ekranie
    SDL_FRect dst = { 0.0f, 0.0f, 600.0f, 600.0f };
    SDL_RenderTexture(renderer, canvasTexture, nullptr, &dst);
}