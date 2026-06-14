//
// Created by izakr on 14/06/2026.
//

#ifndef PIXELEDITOR_COLORUTILS_H
#define PIXELEDITOR_COLORUTILS_H

#pragma once
#include <cstdint>
#include "../core/structs.h"
extern "C" {
#include "microui.h"
}

struct HSV {
    uint16_t h; // Hue: 0 to 360
    uint8_t  s; // Saturation: 0 to 255
    uint8_t  v; // Value: 0 to 255
};

namespace ColorUtils {
    double POS(uint16_t hue);
    Color HSVToRGB(HSV hsv, uint8_t pixel_x, uint8_t pixel_y);
    mu_Color HSVTo_mu_Color(HSV hsv, uint8_t pixel_x, uint8_t pixel_y);
    HSV RGBToHSV(Color rgba);
    void UpdatePalette(HSV hsv, Color (&Palette)[256][256]);
}













#endif //PIXELEDITOR_COLORUTILS_H