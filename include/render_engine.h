//
// Created by janphr on 07.09.20.
//
#pragma once

#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include "particle_system.h"
#include <algorithm>

class render_engine {

public:
    render_engine(int width, int height, int color_scale, float dt);
    void run(particle_system &ps);
    void sim(particle_system &ps);

private:
    int width;
    int height;
    int color_scale;
    float dt;

    Window *window;
};
