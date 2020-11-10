//
// Created by janphr on 07.09.20.
//
#pragma once

#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

class particle {

public:
    particle(const Vector2f &pos, const Vector2f &vel, float mass, float G);
    Vector2f &getPos();
    void setPos(const Vector2f &pos);
    Vector2f &getVel();
    void setVel(const Vector2f &vel);

    const Vector2f &getForce() const;

    float getMass() const;

    float getGMass() const;

    void add_force(Vector2f f);
    void reset_force();

private:
    Vector2f pos;
    Vector2f vel;
    Vector2f force;
    float mass;
    float G_mass;

};