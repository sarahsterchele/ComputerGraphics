#include <iostream>
#include <QGridLayout>
#include "application/window361.h"

Window361::Window361()
{
    renderArea = new RenderArea361((QWidget *)0);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(renderArea, 0, 0);
    setLayout(layout);

    setWindowTitle(tr("361 Window"));
}

Drawable *Window361::getDrawable() 
{
    return renderArea;
}

Window361::~Window361() 
{
    delete renderArea;
}