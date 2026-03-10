//
// Created by izakr on 10/03/2026.
//

#include "Image.h"
#include <vector>


public class Image  {

private:
    std::vector<Layer> layers;
    size_t width, height;

public:
    void addLayer() {
        layers.emplace_back();
    }

    void removeLayer(size_t index) {
        if (index < layers.size()) {
            layers.erase(layers.begin() + index);
        }
    }

    Layer& getLayer(size_t index) {
        return layers.at(index);
    }

    size_t layerCount() const { return layers.size(); }
};


