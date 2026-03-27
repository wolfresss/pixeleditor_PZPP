#pragma once

#include <vector>
#include <string>
#include <algorithm>
#include <thread>
#include <cmath>  // dla std::ceil
#include "Types.h"
// Struktura reprezentująca pojedynczy kolor (RGBA)
struct Pixel {
    RGB255 r, g, b, a;
};
// Struktura pojedynczej warstwy
struct Layer {
    int documentId;
    int id;
    std::string name;
    bool visible = true;
    int width, height;
    std::vector<Pixel> pixels;

    Layer( int id, std::string name, int w, int h)
        :  id(id), name(name), width(w), height(h), pixels(w * h, {0, 0, 0, 0}) {}

    void setPixel(int x, int y, Pixel color) {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            pixels[y * width + x] = color;
        }
    }
    void setPixels_checkerboard(int cx, int cy, Pixel color, int r) {
        for(int x = cx - r; x <= cx + r; ++x) {
            for(int y = cy - r; y <= cy + r; ++y) {
                int dx = x - cx, dy = y - cy;
                if (dx * dx + dy * dy <= r * r && (dx + dy) % 2 == 0) {
                    setPixel(x, y, color);
                }
            }
        }
    }

    //TO DO:
    void snake_parallel(std::vector<Pixel>& pixels, int width, int height,
                        int num_threads /*, processPixel*/) {
        int total_pixels = width * height;
        int pixels_per_thread = std::ceil((float)total_pixels / num_threads);

        std::vector<std::thread> threads;
        for (int t = 0; t < num_threads; ++t) {
            int start_idx = t * pixels_per_thread;
            int end_idx = std::min(start_idx + pixels_per_thread, total_pixels);

            threads.emplace_back([&, start_idx, end_idx, width]() {
                for (int idx = start_idx; idx < end_idx; ++idx) {
                    // Snake: x = idx % width, y = idx / width (row-major)
                    int x = idx % width;
                    int y = idx / width;
                    //processPixel(x, y, pixels[idx]);
                }
            });
        }

        for (auto& th : threads) th.join();
    }
    /*snake_parallel(pixels, width, height, 4, [](int x, int y, Pixel& p) {
        // np. blur, gradient itp.
        p.r = (x + y) % 256;
    });*/
    void setPixels(int cx, int cy, Pixel color, int r) {
        for(int x = cx - r; x <= cx + r; ++x) {
            for(int y = cy - r; y <= cy + r; ++y) {
                int dx = x - cx, dy = y - cy;
                if(dx*dx + dy*dy <= r*r) {
                    setPixel(x, y, color);
                }
            }
        }
    }

};

class Document {
private:
    std::vector<Pixel> cacheComposite; // Bufor przechowujący ostatni wynik kompozycji
    bool needsRedraw = true;           // Flaga "brudnego" obrazu

public:
    int width, height;
    std::vector<Layer> layers;
    int activeLayerIndex = 0;
    int PixelSize = 1;

    Document(int w, int h) : width(w), height(h) {
        cacheComposite.resize(w * h, {0, 0, 0, 255});
    }

    // Dodawanie nowej warstwy
    void addLayer(const std::string& name) {
        layers.emplace_back((int)layers.size(), name, width, height);
        needsRedraw = true;
    }

    // Dostęp do aktywnej warstwy
    Layer& getActiveLayer() {
        return layers[activeLayerIndex];
    }
    // Powiadomienie dokumentu, że dane się zmieniły (np. po użyciu ołówka)
    void markDirty() {
        needsRedraw = true;
    }

    // Metoda składająca warstwy w jeden obraz
    const std::vector<Pixel>& composite() {
        if (!needsRedraw) {
            return cacheComposite; // Zwróć gotowy obraz, jeśli nic się nie zmieniło
        }

        // Czyścimy bufor (tło edytora, np. szachownica lub ciemny szary)
        std::fill(cacheComposite.begin(), cacheComposite.end(), Pixel{30, 30, 30, 255});

        for (const auto& layer : layers) {
            if (!layer.visible) continue;

            for (size_t i = 0; i < layer.pixels.size(); ++i) {
                const Pixel& src = layer.pixels[i];
                if (src.a == 0) continue; // Pomiń w pełni przezroczyste

                if (src.a == 255) {
                    cacheComposite[i] = src;
                } else {
                    // Prosty Alpha Blending
                    float a = src.a / 255.0f;
                    cacheComposite[i].r = (RGB255)(src.r * a + cacheComposite[i].r * (1.0f - a));
                    cacheComposite[i].g = (RGB255)(src.g * a + cacheComposite[i].g * (1.0f - a));
                    cacheComposite[i].b = (RGB255)(src.b * a + cacheComposite[i].b * (1.0f - a));
                }
            }
        }
        needsRedraw = false;
        return cacheComposite;
    }
};