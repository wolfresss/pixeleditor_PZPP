#include "structs.h"

u32 pixelSize = 1;
std::vector<Color> Document::dynamic_palette;
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

//NOTE(Iza) : A bug with brush not touching edges was tied to unsigned int, the pixel could not be
// set because it only could execute positive numbers

void Layer::setPixel(int x, int y, Color color) {
    if (x >= 0 && x < static_cast<int>(width) && y >= 0 && y < static_cast<int>(height)) {
        pixels[y * width + x] = color;
    }
}

void Layer::setPixels(int cx, int cy, Color color) {
    int r = static_cast<int>(pixelSize);

    for(int x = cx - r; x <= cx + r; ++x) {
        for(int y = cy - r; y <= cy + r; ++y) {
            int dx = x - cx;
            int dy = y - cy;
            if(dx * dx + dy * dy <= r * r) {
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
    std::vector<Color> result(width * height, {0, 0, 0, 0});

    for (const auto& layer : layers) {
        if (!layer.visible) continue;

        for (size_t i = 0; i < layer.pixels.size(); ++i) {
            const Color& src = layer.pixels[i];
            Color& dst = result[i];

            if (src.a == 0) continue;

            if (src.a == 255 || dst.a == 0) {

                dst = src;
            } else {

                float src_a = src.a / 255.0f;
                float dst_a = dst.a / 255.0f;

                float out_a = src_a + dst_a * (1.0f - src_a);

                dst.r = (RGBA255)((src.r * src_a + dst.r * dst_a * (1.0f - src_a)) / out_a);
                dst.g = (RGBA255)((src.g * src_a + dst.g * dst_a * (1.0f - src_a)) / out_a);
                dst.b = (RGBA255)((src.b * src_a + dst.b * dst_a * (1.0f - src_a)) / out_a);
                dst.a = (RGBA255)(out_a * 255.0f);
            }
        }
    }
    return result;
}