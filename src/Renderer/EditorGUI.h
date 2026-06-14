//
// Created by izakr on 14/06/2026.
//

#ifndef PIXELEDITOR_EDITORGUI_H
#define PIXELEDITOR_EDITORGUI_H

extern "C" {
#include "microui.h"
}
#include "../core/structs.h"

void ProcessMainMenu(mu_Context* ctx);
void ProcessLeftBar(mu_Context* ctx);
void DrawLayersWindow(mu_Context* ctx);
void MainLoop(mu_Context* ctx, Document &CurrentFile);

#endif //PIXELEDITOR_EDITORGUI_H

