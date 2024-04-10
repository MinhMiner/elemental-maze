#include "Button.h"

void Button::update(const int &mouseX, const int &mouseY, const bool &keyMousePressed)
{
    if (mouseX >= pos.x && mouseX <= pos.x + currentFrame.w &&
        mouseY >= pos.y && mouseY <= pos.y + currentFrame.h) {
        hovered = true;
        if (keyMousePressed)
            clicked = true;
        else
            clicked = false;
    } else
        hovered = false;
}