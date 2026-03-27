#include "ITools.h"



void Pencil::execute(Document& doc, int x, int y, int PixelSize, Color drawColor) {

    auto& layer = doc.getActiveLayer();
    if (x >= 0 && x < layer.width && y >= 0 && y < layer.height) {
       layer.setPixels(x, y, PixelSize,drawColor);
        //layer.pixels[y * layer.width + x] = drawColor;
        doc.markDirty();
    }
}
const char* Pencil::getName() const {
    return "Pencil";
}
void Eraser::execute(Document& doc, int x, int y, int PixelSize, Color drawcolor) {
    auto& layer = doc.getActiveLayer();

    if (x >= 0 && x < layer.width && y >= 0 && y < layer.height) {
        // Gumka ustawia kolor na w pełni przezroczysty
        // TO DO: Kontrola przezroczystości gumki
        Color drawColor = {0, 0, 0, 0};
        layer.setPixels(x, y,PixelSize, drawColor);
        doc.markDirty();
    }
}
const char* Eraser::getName() const {
    return "Eraser";
}