
extern "C" {
#include "../microui/src/microui.h"
}
struct UIConfig {
    mu_Real layerOpacity = 50.0f;        // domyślna wartość slidera
    char layerOpacityText[32] = {};      // buffer dla mu_slider_ex
    int layoutWidths[1] = {-1};          // dla mu_layout_row
    bool showGrid = true; //TO DO: implementacja prostego grida
    bool snapToPixel = false; //TO DO: funkcjonalność algorytmy dokumentacja
};