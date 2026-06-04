// Created by izakr on 01/06/2026.
//
#pragma once
#ifndef PIXELEDITOR_RENDERER_H
#define PIXELEDITOR_RENDERER_H

#include "SDL3/SDL_render.h"
#include "SDL3/SDL_video.h"
#include "SDL3_ttf/SDL_ttf.h"
#include "View.h"

// Forward declaration of mu_Context and mu_Command if they aren't included via View.h
struct mu_Context;
namespace Render {

   // CONSTANTS (Use inline constexpr so they are safe in headers)
    inline constexpr int WIN_W = 1900;
    inline constexpr int WIN_H = 1060;

    // GLOBAL VARIABLES (Must all be 'extern' with no assignments here)
    extern int CANVAS_SIZE;
    extern TTF_Font* g_font;
    extern SDL_Window* window;
    extern SDL_Renderer* renderer;
    extern SDL_Event event;
    extern bool quit;
    extern mu_Context* mu_ctx;
    extern mu_Command* cmd;
    extern UIConfig uiConfig;
    extern Document* CurrentFile;

    //  MANDATORY INITIALIZERS
    void InitRenderSDL();
    void DestroyRenderSDL();
    void InitMicroUI();
    void RenderMicroUI();
    void Process_Events();


    //Optional
    //Canvas Renderer (after file has been initialized)
}

#endif //PIXELEDITOR_RENDERER_H