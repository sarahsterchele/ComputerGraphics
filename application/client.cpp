#include <iostream>
#include "application/client.h"

Client::Client(Drawable *drawable, const char* filename) : drawable(drawable)
{
    drawRect(0, 0, 750, 750, 0xFFFFFFFF);
    drawRect( 50,  50, 700, 700, 0x00000000);
    
    simpReader = new SimpReader(drawable);
    simpReader->readSimpFile(filename);
    
    drawable->updateScreen();  
}

void Client::drawRect(int x1, int y1, int x2, int y2, unsigned int color) 
{
    for(int x = x1; x < x2; x++) 
    {
        for(int y = y1; y < y2; y++) 
        {
            drawable->setPixel(x, y, color);
        }
    }
}