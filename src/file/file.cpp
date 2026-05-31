//
// Created by izakr on 25/05/2026.
//
//#include "../core/structs.h"
#include <fstream>
#include "file.h"

//delete
FILE * out;
 u32 PPA_MAGIC_VALUE = 0x505045;
void ReadFileType() {

}

void WriteFileType(u32 h, u32 w) {
// Open an empty binary file for writing.
// If the wb mode is specified for a ddname that has DISP=MOD,
// the behavior is the same as if ab had been specified. Otherwise,
// if the file already exists, its contents are destroyed.
    out = fopen("file.ppe", "wb");
    if (out) {
        ppe_header header = {};
        header.MagicValue = PPA_MAGIC_VALUE;
        header.h = h;
        header.w = w;
        header.PixelsPerLayer = sizeof(header);
        header.PixelOffset = h * w * sizeof(Color) * 4;
        fwrite(&header, 1,1,out);
        // this many size of this pointer? lol
        fclose(out);
    }
}
