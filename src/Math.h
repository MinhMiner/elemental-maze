#pragma once

#include <iostream>

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

