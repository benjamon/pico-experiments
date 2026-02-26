#pragma once
#include "../pch.h"
#include <vector>
#include <queue>

class grid
{
    private:
    int screenX;
    int screenY;
    int gridWidth;
    int gridHeight;
    int cellSize;
    void DrawCell(int x, int y, int fill);
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
        cells(columns*rows, WHITE)
    {}

    int PixelWidth() { return gridWidth * cellSize; }
    int PixelHeight() { return gridHeight * cellSize; }
    int Width() { return gridWidth; }
    int Height() { return gridHeight; }
    int Bottom() { return screenY + PixelWidth(); }
    int Right() { return screenX + PixelHeight(); }
    int GetCellIndex(int x, int y) 
    {
        if (x < 0 || x >= gridWidth || y < 0 || y >= gridHeight)
        {
            return -1;
        }
        
        return y * gridWidth + x;
    }

    int GetCellType(int id) 
    {
        if (id < 0 || id >= cells.size())
        {
            return -1;
        }

        return cells[id];
    }

    void Init();
    void DrawBox();
    void SetCell(int cellId, int fill);
    void Display();
};

class snake
{
    private:
    std::queue<int> tail;
    int x_dir;
    int y_dir;
    int x_pos;
    int y_pos;
    grid& owner;
    
    public:
    int Length;

    snake(grid& owner, int x_pos, int y_pos) : 
        x_pos(x_pos),
        y_pos(y_pos),
        x_dir(1),
        y_dir(0),
        Length(3),
        owner(owner)
    {}

    bool Step(int x_input, int y_input);

    void Init()
    {
        int index = owner.GetCellIndex(x_pos, y_pos);
        if (index == -1)
        {
            return;
        }

        owner.SetCell(index, BLACK);
        tail.push(index);
    }
};