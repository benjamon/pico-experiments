#pragma once
#include <string>
#include "./snek.h"

class grid;

class entity
{ 
    private:
    int x;
    int y;
    grid& owner;
    public:
    std::string name;

    entity (std::string name, int x, int y, grid& owner) :
        name(name),
        x(x),
        y(y),
        owner(owner)
    {}

    int X() { return x; };
    int Y() { return y; };

    void Draw();

    bool TryMove(int shiftX, int shiftY);
};