//
// Created by izakr on 14/06/2026.
//

#include "ColorUtils.h"
#include <cmath>
#include <algorithm>

namespace ColorUtils {

    double POS(uint16_t hue) {
        double value = std::fmod(static_cast<double>(hue) / 60.0, 2.0) - 1.0;
        return std::abs(value);
    }

    Color HSVToRGB(HSV hsv, uint8_t pixel_x, uint8_t pixel_y) {
        double s = pixel_x / 255.0;
        double v = (255 - pixel_y) / 255.0;
        double c = v * s;
        double x = c * (1.0 - POS(hsv.h));
        double m = v - c;

        double r_ = 0, g_ = 0, b_ = 0;
        if (hsv.h < 60)        { r_ = c; g_ = x; b_ = 0; }
        else if (hsv.h < 120)  { r_ = x; g_ = c; b_ = 0; }
        else if (hsv.h < 180)  { r_ = 0; g_ = c; b_ = x; }
        else if (hsv.h < 240)  { r_ = 0; g_ = x; b_ = c; }
        else if (hsv.h < 300)  { r_ = x; g_ = 0; b_ = c; }
        else if (hsv.h <= 360) { r_ = c; g_ = 0; b_ = x; }

        return Color{
            static_cast<uint8_t>((r_ + m) * 255.0),
            static_cast<uint8_t>((g_ + m) * 255.0),
            static_cast<uint8_t>((b_ + m) * 255.0),
            255
        };
    }

    mu_Color HSVTo_mu_Color(HSV hsv, uint8_t pixel_x, uint8_t pixel_y) {
        Color c = HSVToRGB(hsv, pixel_x, pixel_y);
        return mu_Color{ c.r, c.g, c.b, c.a };
    }

    void UpdatePalette(HSV hsv, Color (&Palette)[256][256]) {
        for (int y = 0; y < 256; y++) {
            for (int x = 0; x < 256; x++) {
                Palette[y][x] = HSVToRGB(hsv, x, y);
            }
        }
    }

    HSV RGBToHSV(Color rgba) {
        HSV hsv;
        double r = rgba.r / 255.0;
        double g = rgba.g / 255.0;
        double b = rgba.b / 255.0;

        double max_val = std::max({r, g, b});
        double min_val = std::min({r, g, b});
        double delta = max_val - min_val;

        hsv.v = static_cast<uint8_t>(max_val * 255.0);
        if (max_val > 0.0) {
            hsv.s = static_cast<uint8_t>((delta / max_val) * 255.0);
        } else {
            hsv.s = 0; hsv.h = 0; return hsv;
        }

        double h_deg = 0.0;
        if (delta > 0.0) {
            if (max_val == r)      h_deg = 60.0 * std::fmod(((g - b) / delta), 6.0);
            else if (max_val == g) h_deg = 60.0 * (((b - r) / delta) + 2.0);
            else if (max_val == b) h_deg = 60.0 * (((r - g) / delta) + 4.0);
            if (h_deg < 0.0) h_deg += 360.0;
        }
        hsv.h = static_cast<uint16_t>((h_deg / 360.0) * 255.0);
        return hsv;
    }
}