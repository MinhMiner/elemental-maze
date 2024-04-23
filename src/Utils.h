#pragma once

#include <iostream>

enum stateID {
    TITLE_SCREEN,
    PLAY_SCREEN,
    PAUSE_SCREEN,
    SELECT_MAP_SCREEN,
    END_SCREEN
};

enum buttonType {
    START_BUTTON,
    PLAY_AGAIN_BUTTON,
    SELECT_MAP_BUTTON,
    MAP_1_BUTTON,
    MAP_2_BUTTON,
    PAUSE_BUTTON
};

enum foodType {
	BONE,
	FISH,
	STEAK,
	CHICKEN,
	GIFT
};

struct inputKeys {
	bool keyWPressed = false;
	bool keyAPressed = false;
	bool keySPressed = false;
	bool keyDPressed = false;
	bool keyMousePressed = false;
	bool keySpacePressed = false;
	int mouseX, mouseY;
};

enum effectType {
    SPEED,
    SHIELD,
    DASH,
    INVINCIBLE
};

struct Vector2f {
    Vector2f()
    :x(0.0f), y(0.0f)
    {}

    Vector2f(float p_x, float p_y)
    :x(p_x), y(p_y)
    {}

    void print()
	{
		std::cout << x << ", " << y << std::endl;
	}

    float x, y;
};

struct Effect {
    Effect()
    :effectName(SPEED), amplifier(0.25f), duration(0.0f), maxDuration(0.0f)
    {}

    Effect(effectType p_effectName, double p_amplifier, double p_duration)
    :effectName(p_effectName), amplifier(p_amplifier), duration(p_duration), maxDuration(p_duration)
    {}

    void print()
	{
		std::cout << effectName << ", " << amplifier << ", " << duration << std::endl;
	}

    effectType effectName;
    double amplifier;
    double duration;
    double maxDuration;
};

