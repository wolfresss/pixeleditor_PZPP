//
// Created by izakr on 25/05/2026.
//

#ifndef PIXELEDITOR_FILE_H
#define PIXELEDITOR_FILE_H


#pragma once
#include <vector>
#include <string>
#include "../core/structs.h"

#pragma pack(push, 1)
struct ppe_header {
    uint32_t MagicValue;       // 0x505045 ("PPE")
    uint32_t w;
    uint32_t h;
    uint32_t LayerCount;
};

struct ppe_layer_header {
    uint32_t Id;
    uint8_t visible;
    uint32_t name_length;
};
#pragma pack(pop)

void SaveDocumentPPE(const char* filename, const Document& doc);
Document* LoadDocumentPPE(const char* filename);
std::string open_file_dialog_windows();
std::string save_file_dialog_windows();
#endif //PIXELEDITOR_FILE_H