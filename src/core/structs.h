#pragma once

#include <vector>
#include <string>
#include <cstdint>
#include <algorithm>

// Struktura reprezentująca pojedynczy kolor (RGBA)
struct Pixel {
    uint8_t r, g, b, a;
};

// Struktura pojedynczej warstwy
struct Layer {
    int id;
    std::string name;
    bool visible = true;
    int width, height;
    std::vector<Pixel> pixels;

    Layer(int id, std::string name, int w, int h)
        : id(id), name(name), width(w), height(h), pixels(w * h, {0, 0, 0, 0}) {}

    // Metoda pomocnicza do bezpiecznego ustawiania piksela
    void setPixel(int x, int y, Pixel color) {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            pixels[y * width + x] = color;
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
                    cacheComposite[i].r = (uint8_t)(src.r * a + cacheComposite[i].r * (1.0f - a));
                    cacheComposite[i].g = (uint8_t)(src.g * a + cacheComposite[i].g * (1.0f - a));
                    cacheComposite[i].b = (uint8_t)(src.b * a + cacheComposite[i].b * (1.0f - a));
                }
            }
        }

        needsRedraw = false;
        return cacheComposite;
    }
};