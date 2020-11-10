//
// Created by janphr on 07.09.20.
//
#pragma once

#include "worker.h"
#include "util.h"

class particle_system {

public:
    particle_system(int width, int height, float initialDistance, float G, int particleCount, float color_scale);
    void update(float &dt, int mode);
    vector<particle> &getParticles();
    void check_out_of_bounds(particle *p);
    int getParticleCount() const;
    void init_galaxy(float outer_radius, float inner_radius, float rot_vel, float center_mass, float galaxy_mass, float heavy_stars_ratio);
    void step(float &dt, int mode);

    float *getCoords() const;

    unsigned char *getColors() const;


private:
    int particle_count;
    float color_scale;
    float width;
    float height;
    vector<particle> particles;

    float *posX;
    float *posY;
    float *posZ;

    float *velX;
    float *velY;
    float *velZ;

    float *accX;
    float *accY;
    float *accZ;

    float *forceX;
    float *forceY;
    float *forceZ;

    float *mass;

    float *coords;
    unsigned char *colors;

    float cluster_radius;
};
