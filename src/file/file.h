//
// Created by izakr on 25/05/2026.
//

#ifndef PIXELEDITOR_FILE_H
#define PIXELEDITOR_FILE_H
#include <cstdint>
#include "..\core\Types.h"
#include <vector>
#pragma once
#endif //PIXELEDITOR_FILE_H

void WriteFileType(const char* filename, u32 h, u32 w, const std::vector<Color>& image);
std::vector<Color> ReadFileType(const char* filename, uint32_t& out_w, uint32_t& out_h);
struct ppe_header
{
#define PPE_MAGIC_VALUE_ PPE_CODE('p', 'p', 'e', 'f')
    u32 MagicValue;
    u32 h;
    u32 w;
    u32 PixelsPerLayer;
    u64 PixelOffset;
};
