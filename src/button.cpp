#include "Button.h"
#include <iostream>

void Button::update(const inputKeys &inputQueue)
{
    if (inputQueue.mouseX >= pos.x && inputQueue.mouseX <= pos.x + currentFrame.w &&
        inputQueue.mouseY >= pos.y && inputQueue.mouseY <= pos.y + currentFrame.h) {
        hovered = true;
        currentFrame.y = currentFrame.h;
        if (inputQueue.keyMousePressed) 
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