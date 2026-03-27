#pragma once
#include "../core/structs.h"
// Forward declaration (opcjonalnie, zamiast include)
class Document;

//TO DO: pozbyc sie klas wirtualnych potem
class ITool {
public:
    virtual ~ITool() = default;
    virtual void execute(Document& doc, int x, int y, int PixelSize, Color color)  = 0;
    virtual const char* getName() const = 0;
};
class Pencil : public ITool {
public:
    void execute(Document& doc, int x, int y, int PixelSize, Color color) override;
    const char* getName() const override;
};

class Eraser : public ITool {
public:
    void execute(Document& doc, int x, int y, int PixelSize, Color color) override;
    const char* getName() const override;
};