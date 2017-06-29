#ifndef CLIENT_H
#define CLIENT_H
#include "simpreader.h"

class Client
{
public:
    Client(Drawable *drawable, const char* filename);

private:
    Drawable *drawable;
    SimpReader *simpReader;
    void drawRect(int x1, int y1, int x2, int y2, unsigned int color);
};

#endif // CLIENT_H