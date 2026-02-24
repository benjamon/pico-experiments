#pragma once
#include "../pch.h"
#include <vector>
#include <queue>

class snake
{
    private:
    std::queue<int> tail;
    int x_dir;
    int y_dir;
    int length;
    
    public:
    snake() : 
        x_dir(1),
        y_dir(0),
        length(3)
    {}

    void Step(int x_input, int y_input);
};

class grid
{
    private:
    int screenX;
    int screenY;
    int gridWidth;
    int gridHeight;
    int cellSize;
    UBYTE *image;
    std::vector<int> cells;

    public:
    grid (int x, int y, int columns, int rows, int cellSize, UBYTE *image) :
        screenX(x),
        screenY(y),
        gridWidth(columns),
        gridHeight(rows),
        cellSize(cellSize),
        image(image),
        cells(columns*rows, 0)
    {}

    int PixelWidth() { return gridWidth * cellSize; }
    int PixelHeight() { return gridHeight * cellSize; }
    int Width() { return gridWidth; }
    int Height() { return gridHeight; }
    int Bottom() { return screenY + PixelWidth(); }
    int Right() { return screenX + PixelHeight(); }

    void DrawBox();
    void DrawCell(int x, int y, int fill);
    void DrawCell(int cellId, int fill);
    void SetImage();
    void Display();
};