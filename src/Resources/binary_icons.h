//
// Created by izakr on 11/06/2026.
//

#ifndef PIXELEDITOR_BINARY_ICONS_H
#define PIXELEDITOR_BINARY_ICONS_H
#include <cstdint>

#include "microui.h"
#include "SDL3/SDL_render.h"
#define ICON_COLOR 0xFFFFFFFF;

#endif //PIXELEDITOR_BINARY_ICONS_H

enum {
    ICON_GRADIENT = MU_ICON_MAX,
    ICON_PBM_PENCIL,
    ICON_PBM_RUBBER,
    ICON_PBM_TOTAL_COUNT
};

typedef struct {
    int icon_id;
    SDL_Texture* texture;
} PBM_IconMap;

#define MY_ICONS_SIZE (ICON_PBM_TOTAL_COUNT - MU_ICON_MAX)

static PBM_IconMap my_ui_icons[MY_ICONS_SIZE];

// Dodano static lub inline
static SDL_Texture* GetTextureByIconID(int icon_id) {
    for (int i = 0; i < MY_ICONS_SIZE; i++) {
        if (my_ui_icons[i].icon_id == icon_id) {
            return my_ui_icons[i].texture;
        }
    }
    return NULL;
}

static SDL_Texture* LoadIconFrom64BitArray(SDL_Renderer* renderer, const uint64_t* bits, int width, int height) {
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, width, height);
    if (!texture) {
        SDL_Log("Błąd tworzenia tekstury ikonki: %s", SDL_GetError());
        return NULL;
    }

    void* pixels;
    int pitch;
    if (SDL_LockTexture(texture, NULL, &pixels, &pitch) < 0) {
        SDL_Log("Nie udało się zablokować tekstury: %s", SDL_GetError());
        SDL_DestroyTexture(texture);
        return NULL;
    }

    Uint32* dst = (Uint32*)pixels;

    for (int y = 0; y < height; y++) {
        uint64_t row_mask = bits[y];

        for (int x = 0; x < width; x++) {
            int bit_shift = 63 - x;
            int is_pixel_active = (row_mask >> bit_shift) & 1;

            int pixel_index = y * (pitch / 4) + x;
            if (is_pixel_active) {
                dst[pixel_index] = ICON_COLOR;
            } else {
                dst[pixel_index] = 0x00000000;
            }
        }
    }

    SDL_UnlockTexture(texture);
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

    return texture;
}

const uint64_t rubber_icon_bits[50] = {
    0x0000000000000000, // Wiersz 0
    0x0000000000000000, // Wiersz 1
    0x0000000000000000, // Wiersz 2
    0x0000000000000000, // Wiersz 3
    0x0000000000000000, // Wiersz 4
    0x0000000e00000000, // Wiersz 5
    0x0000001f00000000, // Wiersz 6
    0x0000003f80000000, // Wiersz 7
    0x0000007fc0000000, // Wiersz 8
    0x000000ffe0000000, // Wiersz 9
    0x000001fff0000000, // Wiersz 10
    0x000003fff8000000, // Wiersz 11
    0x000007fffc000000, // Wiersz 12
    0x00000ffffe000000, // Wiersz 13
    0x00001fffff000000, // Wiersz 14
    0x00003fffff800000, // Wiersz 15
    0x00007fffffc00000, // Wiersz 16
    0x0000ffffffe00000, // Wiersz 17
    0x0001ffffffe00000, // Wiersz 18
    0x0003ffffffc00000, // Wiersz 19
    0x0005ffffff800000, // Wiersz 20
    0x0008ffffff000000, // Wiersz 21
    0x00107ffffe000000, // Wiersz 22
    0x00203ffffc000000, // Wiersz 23
    0x00401ffff8000000, // Wiersz 24
    0x00800ffff0000000, // Wiersz 25
    0x008007ffe0000000, // Wiersz 26
    0x010003ffc0000000, // Wiersz 27
    0x010001ff80000000, // Wiersz 28
    0x010000ff00000000, // Wiersz 29
    0x0100007e00000000, // Wiersz 30
    0x0080003c00000000, // Wiersz 31
    0x0040001800000000, // Wiersz 32
    0x0020001000000000, // Wiersz 33
    0x0010002000000000, // Wiersz 34
    0x0010004000000000, // Wiersz 35
    0x0008008000000000, // Wiersz 36
    0x0004010000000000, // Wiersz 37
    0x0002020000000000, // Wiersz 38
    0x1ffffffffffc0000, // Wiersz 39
    0x0000000000000000, // Wiersz 40
    0x0000000000000000, // Wiersz 41
    0x0000000000000000, // Wiersz 42
    0x0000000000000000, // Wiersz 43
    0x0000000000000000, // Wiersz 44
    0x0000000000000000, // Wiersz 45
    0x0000000000000000, // Wiersz 46
    0x0000000000000000, // Wiersz 47
    0x0000000000000000, // Wiersz 48
    0x0000000000000000 // Wiersz 49
};

const uint64_t pencil_icon_bits[50] = {
    0x0000000000000000, // Wiersz 0
    0x0000000000000000, // Wiersz 1
    0x0000000000000000, // Wiersz 2
    0x0000000000000000, // Wiersz 3
    0x0000000000000000, // Wiersz 4
    0x0000000000000000, // Wiersz 5
    0x0000000000000000, // Wiersz 6
    0x0000000040000000, // Wiersz 7
    0x00000000b0000000, // Wiersz 8
    0x000000010c000000, // Wiersz 9
    0x0000000203000000, // Wiersz 10
    0x0000000400800000, // Wiersz 11
    0x0000000800800000, // Wiersz 12
    0x0000001001000000, // Wiersz 13
    0x0000002002000000, // Wiersz 14
    0x0000004004000000, // Wiersz 15
    0x0000008008000000, // Wiersz 16
    0x0000010010000000, // Wiersz 17
    0x0000020020000000, // Wiersz 18
    0x0000040040000000, // Wiersz 19
    0x0000080080000000, // Wiersz 20
    0x0000100100000000, // Wiersz 21
    0x0000200200000000, // Wiersz 22
    0x0000400400000000, // Wiersz 23
    0x0000800800000000, // Wiersz 24
    0x0001001000000000, // Wiersz 25
    0x0002002000000000, // Wiersz 26
    0x0004004000000000, // Wiersz 27
    0x0008008000000000, // Wiersz 28
    0x0010010000000000, // Wiersz 29
    0x0020020000000000, // Wiersz 30
    0x0060040000000000, // Wiersz 31
    0x0050080000000000, // Wiersz 32
    0x0048100000000000, // Wiersz 33
    0x0044200000000000, // Wiersz 34
    0x0082400000000000, // Wiersz 35
    0x0081800000000000, // Wiersz 36
    0x0081000000000000, // Wiersz 37
    0x0086000000000000, // Wiersz 38
    0x0108000000000000, // Wiersz 39
    0x0130000000000000, // Wiersz 40
    0x01c0000000000000, // Wiersz 41
    0x01ffffffe7fc0000, // Wiersz 42
    0x0000000000000000, // Wiersz 43
    0x0000000000000000, // Wiersz 44
    0x0000000000000000, // Wiersz 45
    0x0000000000000000, // Wiersz 46
    0x0000000000000000, // Wiersz 47
    0x0000000000000000, // Wiersz 48
    0x0000000000000000 // Wiersz 49
};
