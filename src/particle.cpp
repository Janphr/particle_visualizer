//
// Created by janphr on 07.09.20.
//

#include <iostream>
#include "particle.h"

Vector2f &particle::getPos() {
    return pos;
}

void particle::setPos(const Vector2f &p) {
    particle::pos = p;
}

Vector2f &particle::getVel() {
    return vel;
}

void particle::setVel(const Vector2f &v) {
    particle::vel = v;
}

particle::particle(const Vector2f &pos, const Vector2f &vel, float mass, float G) : pos(pos), vel(vel), mass(mass) {
    this->G_mass = G * mass;
    this->force = {0,0};
}

void particle::add_force(Vector2f f) {
    this->force += f;
}

void particle::reset_force() {
    this->force = {0,0};
}

float particle::getGMass() const {
    return G_mass;
}

float particle::getMass() const {
    return mass;
}

const Vector2f &particle::getForce() const {
    return force;
}

