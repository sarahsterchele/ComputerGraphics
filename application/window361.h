#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include "application/drawable.h"
#include "application/renderarea361.h"

class Window361 : public QWidget
{
    Q_OBJECT

public:
    Window361();
    ~Window361();
    Drawable *getDrawable();

private:
    RenderArea361 *renderArea;
};

#endif // WINDOW_H