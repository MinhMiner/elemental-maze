#include "Button.h"
#include <iostream>

void Button::update(const int &mouseX, const int &mouseY, const bool &keyMousePressed)
{
    if (mouseX >= pos.x && mouseX <= pos.x + currentFrame.w &&
        mouseY >= pos.y && mouseY <= pos.y + currentFrame.h) {
        hovered = true;
        currentFrame.y = currentFrame.h;
        if (keyMousePressed) 
            clicked = true;
        else
            clicked = false;
    } else {
        hovered = false;
        currentFrame.y = 0;
    }

    // if (currentFrame.y != 0)
    // std::cout << "currentFrame.y = " << currentFrame.y << '\n';
}