//
// Created by izakr on 10/03/2026.
//
#include <vector>
#include "Layer.h"

#include <cstdint>

struct Layer {
    // dane warstwy: piksele, widoczność itp.
    std::vector<uint8_t> pixels;  // lub inny kontener na piksele
    bool visible = true;
    // inne pola...
};
