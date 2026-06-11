// Created by izakr on 27/03/2026.
#include <iostream>
#include <memory>
#include <ostream>
#include <vector>
#include "Renderer.h"
#include "View.h"
#include "../file/file.h"

using namespace std;

static char export_filename[64] = "my_artwork.ppe";


void process_start_window(mu_Context* ctx, Document* &CurrentFile) {
     {
        int widths[1] = {-1};
        mu_layout_row(ctx, 1, widths, 0);
        mu_layout_row(ctx, 1, widths, 30);

        if (mu_button(ctx, "New Project")) {
            if (CurrentFile) delete CurrentFile;

            CurrentFile = new Document(100, 100, "New Artwork", std::vector<Color>());
            CurrentFile->addLayer("Background", {1, 1, 1, 1});
        }

        if (mu_button(ctx, "Open Project")) {
            std::string selected_path = open_file_dialog_windows();

            if (!selected_path.empty()) {
                u32 loaded_w = 0, loaded_h = 0;
                std::vector<Color> pixels = ReadFileType(selected_path.c_str(), loaded_w, loaded_h);

                if (pixels.empty()) {
                    std::cout << "error, pixels array empty or file missing" << std::endl;
                } else {
                    if (CurrentFile) delete CurrentFile;
                    CurrentFile = new Document(loaded_w, loaded_h, "Loaded Project", pixels);
                }
            } else {
                std::cout << "Uzytkownik anulowal wybor pliku." << std::endl;
            }
        }
        mu_end_window(ctx);
    }
}


void MainLoop(mu_Context* ctx, Document* &CurrentFile) {
    process_start_window(ctx, CurrentFile);
}




void UpdateRGBAPaletteTexture(SDL_Renderer* renderer, SDL_Texture* texture, uint8_t current_blue) {
    uint32_t pixels[256 * 256];

    for (int y = 0; y < 256; y++) {
        for (int x = 0; x < 256; x++) {
            uint8_t r = x;               // Red increases left-to-right (0 to 255)
            uint8_t g = 255 - y;         // Green increases bottom-to-top (0 to 255)
            uint8_t b = current_blue;    // Blue is locked to whatever the slider is at
            uint8_t a = 255;             // Fully opaque for the picker background

            // Pack pixels as RGBA8888
            pixels[y * 256 + x] = (r << 24) | (g << 16) | (b << 8) | a;
        }
    }

    SDL_UpdateTexture(texture, nullptr, pixels, 256 * sizeof(uint32_t));
}


