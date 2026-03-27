#pragma once
#include <vector>
#include <string>
#include <cstdint>
#include "Types.h"

struct Pixel {
    uint8_t r, g, b, a;
};

struct Layer {
    int IdD;
    int Id;
    std::string name; // Do microui
    bool visible = true;
    int width, height;
    std::vector<Pixel> pixels;

    Layer( int id, std::string name, int w, int h)
        : Id(id), name(name), width(w), height(h), pixels(w * h, {0, 0, 0, 0}) {}

    void setPixel(int x, int y, Pixel color) {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            pixels[y * width + x] = color;
        }
    }
};

class Document {
public:
    int width, height;
    std::vector<Layer> layers;
    int activeLayerIndex = 0;
    int PixelSize = 1;

    Document(int w, int h) : width(w), height(h) {}

    void addLayer(std::string name) {
        layers.emplace_back((int)layers.size(), name, width, height);
    }

    Layer& activeLayer() { return layers[activeLayerIndex]; }

    // Do rendrowania warstw
    std::vector<Pixel> composite() {
        // Tworzymy pusty bufor wynikowy (przezroczysty)
        std::vector<Pixel> result(width * height, {0, 0, 0, 255});

        for (const auto& layer : layers) {
            if (!layer.visible) continue;

            for (size_t i = 0; i < layer.pixels.size(); ++i) {
                const Pixel& src = layer.pixels[i];
                Pixel& dst = result[i];

                // Prosty Alpha Blending (A over B)
                if (src.a == 255) {
                    // Piksel w pełni nieprzezroczysty - nadpisujemy
                    dst = src;
                } else if (src.a > 0) {
                    // Piksel półprzezroczysty - proste mieszanie
                    float alpha = src.a / 255.0f;
                    dst.r = (RGB255)(src.r * alpha + dst.r * (1.0f - alpha));
                    dst.g = (RGB255)(src.g * alpha + dst.g * (1.0f - alpha));
                    dst.b = (RGB255)(src.b * alpha + dst.b * (1.0f - alpha));
                    dst.a = 255; // Dla uproszczenia wynik zawsze nieprzezroczysty na razie

                    // TO DO: mieszanie warstw rozne tryby
                }
            }
        }
        return result;
    }
};