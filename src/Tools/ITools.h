#pragma once
 #include "../core/structs.h"
// Forward declaration (opcjonalnie, zamiast include)
class Document;
enum class ToolEvent {
	Down,//zapis stanu startowego, klikniecie
	Move,//ruch myszki
	Up//zapis stanu końcowego, puszczenie przycisku myszki
};
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
class LineTool : public ITool {
private:
    bool active = false;
    int sx = 0, sy = 0;

public:
    const char* getName() const override { return "Line"; }

    void execute(Document& doc,
        ToolEvent event,
        int x,
        int y,
        int PixelSize,
        Color color) override;
};
class FloodFillTool : public ITool {//narazie prosty floodfill bez rekurencji
public:
    const char* getName() const override { return "FloodFill"; }

    void execute(Document& doc,
        ToolEvent event,
        int x,
        int y,
        int PixelSize,
        Color color) override;
};



