//
// Created by izakr on 04/06/2026.
//

#include "ColorPicker.h"
#include <cmath>

HSV currentHSV{0, 255, 255};


double POS() {
    double value = std::fmod(static_cast<double>(currentHSV.h) / 60.0, 2.0) - 1.0;
    return std::abs(value);
}

Color HSVToRGB(uint8_t pixel_x, uint8_t pixel_y) {
    // x_pct i y_pct to wartości od 0.0 do 1.0 wyciągnięte z pozycji piksela na kwadracie
    double s = pixel_x / 255.0;
    double v = (255 - pixel_y) / 255.0;

    double c = v * s;
    double x = c * (1.0 - POS());
    double m = v - c;

    double r_ = 0, g_ = 0, b_ = 0;
    if (currentHSV.h < 60)        { r_ = c; g_ = x; b_ = 0; }
    else if (currentHSV.h < 120)  { r_ = x; g_ = c; b_ = 0; }
    else if (currentHSV.h < 180)  { r_ = 0; g_ = c; b_ = x; }
    else if (currentHSV.h < 240)  { r_ = 0; g_ = x; b_ = c; }
    else if (currentHSV.h < 300)  { r_ = x; g_ = 0; b_ = c; }
    else if (currentHSV.h <= 360) { r_ = c; g_ = 0; b_ = x; }

    return Color{
        static_cast<uint8_t>((r_ + m) * 255.0),
        static_cast<uint8_t>((g_ + m) * 255.0),
        static_cast<uint8_t>((b_ + m) * 255.0),
        255 // Alfa
    };
}

void UpdatePalette(Color (&Palette)[256][256]) {
    for (int y = 0; y < 256; y++) {
        for (int x = 0; x < 256; x++) {
            Palette[y][x] = HSVToRGB(x, y);
        }
    }
}