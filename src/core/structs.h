#pragma once
#include <vector>
#include <string>
#include "Types.h"


struct Layer {
    int IdD;
    int Id;
    std::string name; // Do microui
    bool visible = true;
    int width;
    int height;
    std::vector<Color> pixels;


    Layer(int id, std::string name, int w, int h);
    void setPixel(int x, int y, Color color);
    void setPixels(int cx, int cy, int r, Color color);


    // void setPixels_checkerboard(int cx, int cy, Color color, int r);
};

class Document {
public:
    int width;
    int height;
    std::vector<Layer> layers;
    int activeLayerIndex = 0;
    int PixelSize = 1;

    Document(int w, int h);
    void addLayer(std::string name);
    Layer& activeLayer();
    std::vector<Color> composite();
};