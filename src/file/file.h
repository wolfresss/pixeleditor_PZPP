//
// Created by izakr on 25/05/2026.
//

#ifndef PIXELEDITOR_FILE_H
#define PIXELEDITOR_FILE_H
#include <cstdint>
#include "..\core\Types.h"

#endif //PIXELEDITOR_FILE_H

struct ppe_header
{

#define PPE_MAGIC_VALUE_ PPE_CODE('p', 'p', 'e', 'f')
    u32 MagicValue;
    u32 h;
    u32 w;
   // u32 LayersCount; //idk if it should be color times array of pixel
    //u64 TagOffset;
    u32 PixelsPerLayer;
    u64 PixelOffset;
};

struct ppe_tag
{
 u32 ID;
    u32 Value;
};

struct ImageProperties
{
  u32 ID;
    u32 Width;
    u32 Height;

};
