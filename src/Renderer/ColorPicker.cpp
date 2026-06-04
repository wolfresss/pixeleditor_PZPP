//
// Created by izakr on 04/06/2026.
//

#include "ColorPicker.h"


HSV currentHSV;

inline Color Palette[255][255]; //actual pixels that will be turned into SDL surface to be rendered
double POS( ) {
    double value = ((currentHSV.h / 60) % 2 ) - 1;

    if (value < 0) {value = value*(-1);
    }
    return value;
}
Color HSVToRGB(uint8_t s , uint8_t v) {
    RGBA255 x = s/255.0 * s/255.0 * ( 1 - POS() );
    RGBA255 c = (v/255.0)*(s/255.0);
    if (0 <= currentHSV.h && currentHSV.h < 60) {
        return Color{c, x, 0};
    }
    if (60 <= currentHSV.h && currentHSV.h < 120) {
        return Color{x, c, 0};
    } if (120 <= currentHSV.h && currentHSV.h < 180) {
        return Color{0, c, x};
    } if (180 <= currentHSV.h && currentHSV.h < 240)
    {return Color{0, x, c};
    } if (240 <= currentHSV.h && currentHSV.h < 300 ) {
        return Color{x, 0, c};
    } if (300 <= currentHSV.h && currentHSV.h < 360) {
        return Color{c, 0, x};
    }
}


void UpdatePalette() {
    for (int i = 0; i < 255; i++) {
        for (int j = 0; j < 255; j++) {
            Palette[i][j] = HSVToRGB(i, j);
        }
    }
}

