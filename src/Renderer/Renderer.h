// Created by izakr on 01/06/2026.
//
#pragma once
#ifndef PIXELEDITOR_RENDERER_H
#define PIXELEDITOR_RENDERER_H

#include "SDL3/SDL_render.h"
#include "SDL3/SDL_video.h"
#include "SDL3_ttf/SDL_ttf.h"
#include "View.h"
#include "ColorPicker.h"

struct mu_Context;
struct UIConfig {
    float scale = 1.0f;
    mu_Real PixelSize = 1.0F;
    mu_Real layerOpacity = 50.0f;        // domyślna wartość slidera
    char layerOpacityText[32] = {};      // buffer dla mu_slider_ex
    int layoutWidths[1] = {-1};          // dla mu_layout_row
    bool showGrid = true; //TO DO: implementacja prostego grida
    bool snapToPixel = false; //TO DO: funkcjonalność algorytmy dokumentacja
    std::unique_ptr<ITool> currentTool;
};

struct WindowContext {
    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* font;
    bool shouldQuit = false;
    SDL_Event event;
    mu_Context* mu_ctx;
    Document* CurrentFile;
    SDL_Texture* paletteTexture;
};

namespace Render {
   // CONSTANTS
    inline constexpr int WIN_W = 1500;
    inline constexpr int WIN_H = 600;
    // GLOBAL VARIABLES
    extern int CANVAS_SIZE;
   extern WindowContext window;
    extern UIConfig uiConfig;
    //  MANDATORY INITIALIZERS
    void Init();
    void RenderMicroUI();
    void Process_Events();

    //Features
    void UpdateGradientSquare() ;

}

#endif //PIXELEDITOR_RENDERER_H