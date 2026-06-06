//
// Created by izakr on 04/06/2026.
//

#ifndef PIXELEDITOR_COLORPICKER_H
#define PIXELEDITOR_COLORPICKER_H
#include <cstdint>
#include "microui.h"
#include "../core/Types.h"

typedef uint32_t u32;
typedef uint8_t RGBA255;

struct HSV {
    uint16_t h; // Hue: 0 to 360
    uint8_t  s; // Saturation: 0 to 255
    uint8_t  v; // Value: 0 to 255
};

extern HSV currentHSV;
extern RGBA255 currentColor;
void UpdatePalette(Color (&Palette)[256][256]);

Color HSVToRGB(uint8_t pixel_x, uint8_t pixel_y);
mu_Color HSVTo_mu_Color(uint8_t pixel_x, uint8_t pixel_y);

#endif //PIXELEDITOR_COLORPICKER_H