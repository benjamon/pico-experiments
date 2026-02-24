#include "snek.h"

void snake::Step(int x_dir, int y_dir)
{
    
}

void grid::SetImage()
{
    Paint_NewImage(image, cellSize*gridWidth, cellSize*gridHeight, 0, WHITE);
    Paint_SelectImage(image);
}

void grid::DrawBox()
{
    Paint_DrawRectangle(1, 1, PixelWidth(), PixelHeight(), BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawRectangle(1, cellSize, PixelWidth(), PixelHeight()-cellSize, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawRectangle(cellSize, 1, PixelWidth()-cellSize, PixelHeight(), BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    // for (int i = 1; i < gridWidth; i++)
    // {
    //     int x = cellSize * i;
    //     Paint_DrawLine(x, 1, x, Height(), BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    // }
    // for (int i = 1; i < gridHeight; i++)
    // {
    //     int y = cellSize * i;
    //     Paint_DrawLine(1, y, Width(), y, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    // }
}
 
void grid::DrawCell(int cx, int cy, int fill)
{
    int left = 2 + cx * cellSize;
    int top = 2 + cy * cellSize;
    Paint_DrawRectangle(left, top, left+cellSize-4, top+cellSize-4, fill, DOT_PIXEL_1X1, DRAW_FILL_FULL);
}

void grid::Display()
{
    EPD_4IN2_V2_PartialDisplay(image, screenX, screenY, Right(), Bottom());
}