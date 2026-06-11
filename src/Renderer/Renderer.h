// Created by izakr on 01/06/2026.
//
#pragma once
#ifndef PIXELEDITOR_RENDERER_H
#define PIXELEDITOR_RENDERER_H

#include "SDL3/SDL_render.h"
#include "SDL3/SDL_video.h"
#include "SDL3_ttf/SDL_ttf.h"
#include "View.h"


typedef uint32_t u32;
typedef uint8_t RGBA255;

struct HSV {
    uint16_t h; // Hue: 0 to 360
    uint8_t  s; // Saturation: 0 to 255
    uint8_t  v; // Value: 0 to 255
};


enum ToolType {
    PENCIL,
    FLOODFILL,
    RUBBER
};

struct mu_Context;
struct UIConfig {
    float scale = 1.0f;
    mu_Real PixelSize = 1.0F;// domyślna wartość slidera
    char layerOpacityText[32] = {};      // buffer dla mu_slider_ex
    int layoutWidths[1] = {-1};          // dla mu_layout_row
    bool showGrid = true; //TO DO: implementacja prostego grida
    bool snapToPixel = false; //TO DO: funkcjonalność algorytmy dokumentacja
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
    CanvasRenderer* canvasRenderer;
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

    //Pallete / Gradient
    void UpdateGradientSquare() ;
    void UpdatePalette(Color (&Palette)[256][256]);
    Color HSVToRGB(uint8_t pixel_x, uint8_t pixel_y);
    mu_Color HSVTo_mu_Color(uint8_t pixel_x, uint8_t pixel_y);
    SDL_Texture* CreatePaletteTexture();
    extern HSV currentHSV;
    extern Color currentColor;

    // Funkcje pomocnicze microui do tekstu
    int text_width(mu_Font font, const char *text, int len);
    int text_height(mu_Font font);

    // Zarządzanie ikonami binarnymi
    void InitAppIcons(SDL_Renderer* renderer);
    void FreeAppIcons();



}

#endif //PIXELEDITOR_RENDERER_H