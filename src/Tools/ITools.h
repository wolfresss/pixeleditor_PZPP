#pragma once

// Forward declaration (opcjonalnie, zamiast include)
class Document;

class ITool {
public:
    virtual ~ITool() = default;
    virtual void execute(Document& doc, int x, int y) = 0;
    virtual const char* getName() const = 0;
};

class Pencil : public ITool {
public:
    void execute(Document& doc, int x, int y) override;
    const char* getName() const override;
};

class Eraser : public ITool {
public:
    void execute(Document& doc, int x, int y) override;
    const char* getName() const override;
};