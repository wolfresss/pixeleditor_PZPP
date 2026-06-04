//
// Created by izakr on 04/06/2026.
//

#ifndef PIXELEDITOR_COLORPICKER_H
#define PIXELEDITOR_COLORPICKER_H
#include <cstdint>
#include "../core/Types.h"


typedef uint32_t u32;
typedef uint8_t RGBA255;
struct HSV {
    uint16_t h; // Hue: 0 to 360 (fits perfectly in 16 bits)
    uint8_t  s; // Saturation: 0 to 255 (mapped from 0.0 - 1.0)
    uint8_t  v; // Value: 0 to 255 (mapped from 0.0 - 1.0)
};


#endif //PIXELEDITOR_COLORPICKER_H