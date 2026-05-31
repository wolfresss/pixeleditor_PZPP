#include "structs.h"

Layer::Layer(int id, std::string name, u32 w, u32 h)
    : Id(id), name(name),
width(w), height(h),
pixels(w * h, {0, 0, 0, 0}
)
{
}

Layer::Layer(int id, std::string name, u32 w, u32 h, std::vector<Color> p)
    : Id(id), name(name),
width(w), height(h),
pixels(p)
{
}
void Layer::setPixel(u32 x, u32 y, Color color) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        pixels[y * width + x] = color;
    }
}

void Layer::setPixels(u32 cx, u32 cy, u32 r, Color color) {
    for(int x = cx - r; x <= cx + r; ++x) {
        for(int y = cy - r; y <= cy + r; ++y) {
            int dx = x - cx, dy = y - cy;
            if(dx*dx + dy*dy <= r*r) {
                setPixel(x, y, color);
            }
        }
    }
}

void Layer::setAllPixels(Color color) {
    for(int x = 0; x < width; ++x) {
        for(int y = 0; y < height; ++y) {
            setPixel(x, y, color);
        }
    }
}

std::vector<Color> setAllPixelsChunk(Color color, u32 w, u32 h) {
    std::vector<Color> chunk;
    for (int x = 0; x < w; ++x) {
        for (int y = 0; y < h; ++y) {
            chunk.push_back(color);
        }
    }
    return chunk;
}

Document::Document(u32 w, u32 h, std::string n, Color color) : width(w), height(h),name(n)
{
    addLayer(n, color);
}
Document::Document(u32 w, u32 h, std::string n, std::vector<Color> pixels) : width(w), height(h),name(n)
{
    addLayer(n, w,h, pixels);
}

void Document::addLayer(std::string name, Color color) {

    layers.emplace_back((int)layers.size(), name, width, height,setAllPixelsChunk(color, width, height) );

}
void Document::addLayer( std::string name, u32 w, u32 h, std::vector<Color> pixels) {
    layers.emplace_back((int)layers.size(), name, width, height, pixels );

}

Layer& Document::activeLayer() {
    return layers[activeLayerIndex];
}

std::vector<Color> Document::composite() {
    std::vector<Color> result(width * height, {0, 0, 0, 255});

    for (const auto& layer : layers) {
        if (!layer.visible) continue;

        for (size_t i = 0; i < layer.pixels.size(); ++i) {
            const Color& src = layer.pixels[i];
            Color& dst = result[i];

            // Prosty Alpha Blending (A over B)
            if (src.a == 255) {
                // Piksel w pełni nieprzezroczysty - nadpisujemy
                dst = src;
            } else if (src.a > 0) {
                // Piksel półprzezroczysty - proste mieszanie
                float alpha = src.a / 255.0f;
                dst.r = (RGBA255)(src.r * alpha + dst.r * (1.0f - alpha));
                dst.g = (RGBA255)(src.g * alpha + dst.g * (1.0f - alpha));
                dst.b = (RGBA255)(src.b * alpha + dst.b * (1.0f - alpha));
                dst.a = 255; // Dla uproszczenia wynik zawsze nieprzezroczysty na razie

                // TO DO: mieszanie warstw rozne tryby
            }
        }
    }
    return result;
}