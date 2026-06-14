//
// Created by izakr on 14/06/2026.
//

#include "CanvasTools.h"
#include <cmath>

extern float lastMouseX;
extern float lastMouseY;

namespace CanvasTools {

    void executePencil(Document& doc, int x, int y, Color drawColor) {
        auto& layer = doc.activeLayer();
        if (x >= 0 && x < layer.width && y >= 0 && y < layer.height) {
            layer.setPixels(x, y, drawColor);
        }
    }

    void executeRubber(Document& doc, int x, int y) {
        auto& layer = doc.activeLayer();
        if (x >= 0 && x < layer.width && y >= 0 && y < layer.height) {
            Color transparent = {0, 0, 0, 0};
            layer.setPixels(x, y, transparent);
        }
    }

    void drawLineOnCanvas(Document& doc, int x0, int y0, int x1, int y1, ToolType selectedTool, Color currentColor) {
        int dx = std::abs(x1 - x0);
        int dy = std::abs(y1 - y0);
        int sx = (x0 < x1) ? 1 : -1;
        int sy = (y0 < y1) ? 1 : -1;
        int err = dx - dy;

        while (true) {
            if (selectedTool == PENCIL) executePencil(doc, x0, y0, currentColor);
            else if (selectedTool == RUBBER) executeRubber(doc, x0, y0);

            if (x0 == x1 && y0 == y1) break;
            int e2 = 2 * err;
            if (e2 > -dy) { err -= dy; x0 += sx; }
            if (e2 < dx)  { err += dx; y0 += sy; }
        }
    }

    void ProcessTool(int selectedTool, Document& doc, float mouseX, float mouseY, bool isClick, Color color, int offsetX, int offsetY, float zoomScale) {
        int canvas_x = static_cast<int>((mouseX - offsetX) / zoomScale);
        int canvas_y = static_cast<int>((mouseY - offsetY) / zoomScale);


        if (canvas_x >= 0 && canvas_x < static_cast<int>(doc.width) &&
            canvas_y >= 0 && canvas_y < static_cast<int>(doc.height)) {


            Layer& currentLayer = doc.activeLayer();


            if (selectedTool == PENCIL) {

                currentLayer.setPixels(canvas_x, canvas_y, color);
            }
            else if (selectedTool == RUBBER) {

                currentLayer.setPixels(canvas_x, canvas_y, Color{0, 0, 0, 0});
            }
            }
    }
}