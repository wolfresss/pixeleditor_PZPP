#pragma once
#include <vector>
#include <string>
#include "Types.h"


extern u32 pixelSize;
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
    Layer(int id, std::string name, u32 w, u32 h, std::vector<Color> p);
    void setPixel(u32 x, u32 y, Color color);
    void setPixels(u32 cx, u32 cy, Color color);
    void setAllPixels(Color color);
};

struct Document {
    std::string name;
    u32 width;
    u32 height;
    std::vector<Layer> layers;
    int activeLayerIndex = 0;
    int PixelSize = 1;
    Document(u32 w, u32 h, std::string n, Color color); //this one is for creating new document
    Document(u32 w, u32 h, std::string n, std::vector <Color> p); //this one is for existing ones when load
    void addLayer(std::string name, Color color);

    void addLayer(std::string name, u32 w, u32 h, std::vector<Color> pixels);

    Layer& activeLayer();
    std::vector<Color> composite();
};