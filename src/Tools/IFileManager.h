//
// Created by izakr on 22/03/2026.
//

#ifndef PIXELEDITOR_IFILEMANAGER_H
#define PIXELEDITOR_IFILEMANAGER_H
#pragma once
#include "../core/structs.h"
class IFileManager {
public:
     ~IFileManager() {};
    Document CreateDocument(int w, int h);
  //  Layer CreateLayer(int DocumentId, int w, int h);
    bool DeleteLayer(int DocumentId, int LayerId);
};
#endif //PIXELEDITOR_IFILEMANAGER_H