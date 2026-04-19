#include "ITools.h"



void Pencil::execute(Document& doc, int x, int y, int PixelSize, Color drawColor) {

    auto& layer = doc.activeLayer();
    if (x >= 0 && x < layer.width && y >= 0 && y < layer.height) {
       layer.setPixels(x, y, PixelSize,drawColor);
        //layer.pixels[y * layer.width + x] = drawColor;

    }
}
const char* Pencil::getName() const {
    return "Pencil";
}
void Eraser::execute(Document& doc, int x, int y, int PixelSize, Color drawcolor) {
    auto& layer = doc.activeLayer();

    if (x >= 0 && x < layer.width && y >= 0 && y < layer.height) {
        // Gumka ustawia kolor na w pełni przezroczysty
        // TO DO: Kontrola przezroczystości gumki
        Color drawColor = {0, 0, 0, 0};
        layer.setPixels(x, y,PixelSize, drawColor);

    }
}
const char* Eraser::getName() const {
    return "Eraser";
}
class RectangleTool : public ITool {
private:
    bool active = false;
    int sx = 0, sy = 0;

public:
    const char* getName() const override { return "Rectangle"; }

    void execute(Document& doc,
        ToolEvent event,
        int x,
        int y,
        int PixelSize,
        Color color) override
    {
        if (event == ToolEvent::Down) {
            active = true;
            sx = x;
            sy = y;
            return;
        }

        if (event == ToolEvent::Up && active) {

            int minX = std::min(sx, x);
            int maxX = std::max(sx, x);
            int minY = std::min(sy, y);
            int maxY = std::max(sy, y);

            // top + bottom
            for (int i = minX; i <= maxX; i += PixelSize) {
                doc.setPixel(i, minY, color, PixelSize);
                doc.setPixel(i, maxY, color, PixelSize);
            }

            // left+ right
            for (int j = minY; j <= maxY; j += PixelSize) {
                doc.setPixel(minX, j, color, PixelSize);
                doc.setPixel(maxX, j, color, PixelSize);
            }

            active = false;
        }
    }
};
class CircleTool : public ITool {
private:
    bool active = false;
    int cx = 0, cy = 0;

public:
    const char* getName() const override { return "Circle"; }

    void execute(Document& doc,
        ToolEvent event,
        int x,
        int y,
        int PixelSize,
        Color color) override
    {
        if (event == ToolEvent::Down) {
            active = true;
            cx = x;
            cy = y;
            return;
        }

        if (event == ToolEvent::Up && active) {

            int dx = x - cx;
            int dy = y - cy;
            int r = (int)sqrt(dx * dx + dy * dy);

            int px = r;
            int py = 0;
            int err = 0;

            while (px >= py) {
                doc.setPixel(cx + px, cy + py, color, PixelSize);
                doc.setPixel(cx + py, cy + px, color, PixelSize);
                doc.setPixel(cx - py, cy + px, color, PixelSize);
                doc.setPixel(cx - px, cy + py, color, PixelSize);
                doc.setPixel(cx - px, cy - py, color, PixelSize);
                doc.setPixel(cx - py, cy - px, color, PixelSize);
                doc.setPixel(cx + py, cy - px, color, PixelSize);
                doc.setPixel(cx + px, cy - py, color, PixelSize);

                py++;
                if (err <= 0) err += 2 * py + 1;
                if (err > 0) { px--; err -= 2 * px + 1; }
            }

            active = false;
        }
    }
};


