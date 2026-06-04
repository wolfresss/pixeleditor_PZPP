// Created by izakr on 01/06/2026.
//
#pragma once
#ifndef PIXELEDITOR_RENDERER_H
#define PIXELEDITOR_RENDERER_H

#include "SDL3/SDL_render.h"
#include "SDL3/SDL_video.h"
#include "SDL3_ttf/SDL_ttf.h"
#include "View.h"

struct mu_Context;
struct UIConfig {
    float scale = 1.0f;
    mu_Real PixelSize = 1.0F;
    mu_Real layerOpacity = 50.0f;        // domyślna wartość slidera
    char layerOpacityText[32] = {};      // buffer dla mu_slider_ex
    int layoutWidths[1] = {-1};          // dla mu_layout_row
    bool showGrid = true; //TO DO: implementacja prostego grida
    bool snapToPixel = false; //TO DO: funkcjonalność algorytmy dokumentacja
    RGBA255 R = 0;
    RGBA255 G = 0;
    RGBA255 B = 0;
    RGBA255 A = 100;
    std::unique_ptr<ITool> currentTool;
};

namespace Render {

   // CONSTANTS
    inline constexpr int WIN_W = 1900;
    inline constexpr int WIN_H = 1060;


    // GLOBAL VARIABLES
    extern int CANVAS_SIZE;
    extern TTF_Font* g_font;
    extern SDL_Window* window;
    extern SDL_Renderer* renderer;
    extern SDL_Event event;
    extern bool quit;
    extern mu_Context* mu_ctx;
    extern UIConfig uiConfig;
    extern Document* CurrentFile;
    extern SDL_Texture* paletteTexture;

    //  MANDATORY INITIALIZERS
    void InitRenderSDL();
    void DestroyRenderSDL();
    void InitMicroUI();
    void RenderMicroUI();
    void Process_Events();

    //Optional
    //Canvas Renderer (after file has been initialized)
    void UpdateGradientSquare(int8_t base_r, uint8_t base_g, uint8_t base_b);
}

#endif //PIXELEDITOR_RENDERER_H