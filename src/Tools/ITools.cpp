#include "ITools.h"
#include "../core/structs.h"
void Pencil::execute(Document& doc, int x, int y, int PixelSize) {

    auto& layer = doc.getActiveLayer();
    Pixel drawColor = {0, 255, 255, 255};
    if (x >= 0 && x < layer.width && y >= 0 && y < layer.height) {
       layer.setPixels(x, y, drawColor,PixelSize);
        //layer.pixels[y * layer.width + x] = drawColor;
        doc.markDirty();
    }
}
const char* Pencil::getName() const {
    return "Pencil";
}
void Eraser::execute(Document& doc, int x, int y, int PixelSize) {
    auto& layer = doc.getActiveLayer();

    if (x >= 0 && x < layer.width && y >= 0 && y < layer.height) {
        // Gumka ustawia kolor na w pełni przezroczysty
        // TO DO: Kontrola przezroczystości gumki
        Pixel drawColor = {0, 0, 0, 0};
        layer.setPixels(x, y, drawColor,PixelSize);
        doc.markDirty();
    }
}
const char* Eraser::getName() const {
    return "Eraser";
}