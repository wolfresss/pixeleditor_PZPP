//
// Created by izakr on 14/06/2026.
//

#ifndef PIXELEDITOR_CANVASTOOLS_H
#define PIXELEDITOR_CANVASTOOLS_H

#pragma once
#include "../core/structs.h"
#include "Renderer.h" // ze względu na ToolType i stan, jeśli tam zostaje

namespace CanvasTools {
    void executePencil(Document& doc, int x, int y, Color drawColor);
    void executeRubber(Document& doc, int x, int y);
    void drawLineOnCanvas(Document& doc, int x0, int y0, int x1, int y1, ToolType selectedTool, Color currentColor);
    void ProcessTool(int selectedTool, Document& doc, float mouseX, float mouseY, bool isClick, Color color, int offsetX, int offsetY, float zoomScale);
}


#endif //PIXELEDITOR_CANVASTOOLS_H