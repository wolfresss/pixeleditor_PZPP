#pragma once
#include <vector>
#include <string>
#include "Types.h"


struct Layer {
    u32 IdD;
    u32 Id;
    std::string name; // Do microui
    bool visible = true;
    u32 width;
    u32 height;
    uint32_t DataOffset;
    std::vector<Color> pixels;
    Layer(int id, std::string name, u32 w, u32 h);
    void setPixel(u32 x, u32 y, Color color);
    void setPixels(u32 cx, u32 cy, u32 r, Color color);
};

struct Document {
    u32 width;
    u32 height;
    std::vector<Layer> layers;
    int activeLayerIndex = 0;
    int PixelSize = 1;
    Document(u32 w, u32 h);
    void addLayer(std::string name);
    Layer& activeLayer();
    std::vector<Color> composite();
};