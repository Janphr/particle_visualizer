//
// Created by janphr on 07.09.20.
//

#include <cmath>
#include <iostream>
#include "particle_system.h"

void particle_system::check_out_of_bounds(particle *p) {
    bool reflectedX = false, reflectedY = false;

    auto x = p->getPos().x;
    auto y = p->getPos().y;

    if (x >= this->width) {
        reflectedX = true;
        p->getPos().x = this->width - fmod(x ,this->width);
    } else if (x < 0) {
        reflectedX = true;
        p->getPos().x = fmod(-x ,this->width);
    }

    if (y >= this->height) {
        reflectedY = true;
        p->getPos().y = this->height - fmod(y ,this->height);
    } else if (y < 0) {
        reflectedY = true;
        p->getPos().y = fmod(-y ,this->height);
    }

    if (reflectedX) {
        p->getVel().x *= -0.1;
    }
    if (reflectedY) {
        p->getVel().y *= -0.1;
    }
}

particle_system::particle_system(int width, int height, float initialDistance, float G, int particleCount, float color_scale) {

//    srand(time(NULL));
//
//    int side_length = (int) sqrt(particleCount);
//    Vector2f offset = Vector2f(width / 2 - initialDistance * side_length / 2,
//                               height / 2 - initialDistance * side_length / 2);
//
//    bool sun;
//    int sun_counter = 0;
//
//    for (int i = 0; i < side_length; i++) {
//        for (int j = 0; j < side_length; j++) {
//            sun = (rand() % 1000) > 998;
//            if(sun)
//                sun_counter++;
//            this->particles.emplace_back(
//                    particle(Vector2f(i * initialDistance + (rand() % 10 + (-5)), j * initialDistance + (rand() % 10 + (-5))) + offset, Vector2f(0, 0),
//                             sun?10000*mass:mass, G));
//
//        }
//    }
//    cout << "suns: " << sun_counter << endl;

    this->particle_count = particleCount;
    this->color_scale = color_scale;
    this->width = (float) width;
    this->height = (float) height;

    this->posX = new float[particleCount];
    this->posY = new float[particleCount];
    this->posZ = new float[particleCount];

    this->velX = new float[particleCount];
    this->velY = new float[particleCount];
    this->velZ = new float[particleCount];

    this->forceX = new float[particleCount];
    this->forceY = new float[particleCount];
    this->forceZ = new float[particleCount];

    this->accX = new float[particleCount];
    this->accY = new float[particleCount];
    this->accZ = new float[particleCount];

    this->mass = new float[particleCount];

    this->coords = new float[3*particleCount];
    this->colors = new unsigned char[3*particleCount];

}

vector<particle> &particle_system::getParticles() {
    return particles;
}

int particle_system::getParticleCount() const {
    return particle_count;
}

void particle_system::init_galaxy(float outer_radius, float inner_radius, float rot_vel, float center_mass,
                                  float galaxy_mass, float ratio) {
    this->mass[0] = center_mass;

    this->posX[0] = 0;
    this->posY[0] = 0;
    this->posZ[0] = 0;

    this->cluster_radius = outer_radius/5;

    float avg_mass = galaxy_mass / (float)this->particle_count;
    float heavy_star_mass = (0.5f/ratio)*avg_mass;
    float light_star_mass = 0.5f*avg_mass;

    int heavy_star_counter = 0;


    for (int i = 1; i < this->particle_count; i++) {
        float r = (float) (drand48() * outer_radius) + inner_radius;
        double alpha = drand48() * 2 * M_PI;
        auto x = (float) (cos(alpha) * r);
        auto y = (float) (sin(alpha) * r);

        this->posX[i] = x;
        this->posY[i] = y;
        this->posZ[i] = (float) ((drand48() - 0.5) / 8);


        if(i <= (int)(ratio*(float)this->particle_count)) {
            this->mass[i] = heavy_star_mass;
            heavy_star_counter++;
        } else {
            this->mass[i] = light_star_mass;
        }

        // orbital velocity
        float v0 = (float) sqrt(((1.0f/6.67408) * this->mass[0]) / (r * r)) * rot_vel;

        // rotate by 90°
        float vx = y * v0;
        float vy = -x * v0;

        velX[i] = vx;
        velY[i] = vy;

//        colors[3* i] = 255;
//        colors[3*i + 1] = 255;
//        colors[3*i + 2] = 255;
    }

    cout << heavy_star_counter << " heavy stars with mass: " << heavy_star_mass << " vs light star mass: " << light_star_mass << endl;
}

void particle_system::update(float &dt, int mode) {
#pragma omp parallel for
    for (int i = 0; i < (int) this->particle_count; i++) {
        particle *this_particle = &this->particles[i];

        if (mode == 1) {
            for (particle &other_particle : this->particles) {
                Vector2f r = {this_particle->getPos() - other_particle.getPos()};
                if (!(r.x == 0 && r.y == 0)) {
                    float dist = sqrt(r.x * r.x + r.y * r.y) + 0.05f * 0.05f;
                    float M = (this_particle->getMass() * other_particle.getMass()) / (dist * dist * dist);
                    this_particle->add_force(-M * r);
//                    other_particle.add_force(M * r);
                }
            }
        }
    }
#pragma omp parallel for
    for (int i = 0; i < (int) this->particle_count; i++) {
        particle *this_particle = &this->particles[i];
        Vector2f prev_pos = this_particle->getPos();
        this_particle->setPos(prev_pos + this_particle->getVel() * dt +
                              0.5f * this_particle->getForce() * dt * dt / this_particle->getMass());
        this_particle->setVel((this_particle->getPos() - prev_pos) / dt);
        check_out_of_bounds(this_particle);
        this_particle->reset_force();
    }
}



void particle_system::step(float &dt, int mode) {

    float offsetX = width/2,
    offsetY = height/2;

#pragma omp parallel for schedule(static, this->particle_count/128)
    for (int i = 0; i < this->particle_count; i++) {
        int star_counter = 1;
        for (int j = i + 1; j < this->particle_count; j++) {

            float dx = posX[i] - posX[j];
            float dy = posY[i] - posY[j];
            float dz = posZ[i] - posZ[j];


            float dist = sqrtf(dx * dx + dy * dy + dz * dz) + 0.05f * 0.05f;

            if(dist < this->cluster_radius)
                star_counter++;

            float f = (mass[i] * mass[j]) / (dist * dist);

            forceX[i] -= dx*f;
            forceY[i] -= dy*f;
            forceZ[i] -= dz*f;

            forceX[j] += dx*f;
            forceY[j] += dy*f;
            forceZ[j] += dz*f;
        }
        int r = 3 * 255 * star_counter/(this->particle_count-i);
        colors[3* i] = min(100 + r, 255);
        colors[3*i + 1] = min(150 + r/1.5,255.0);
        colors[3*i + 2] = 255;
    }

#pragma omp parallel for
    for (int i = 0; i < this->particle_count; i++) {

        float   x = posX[i],
                y = posY[i],
                z = posZ[i];

        posX[i] += dt * velX[i] + (0.5f * forceX[i] * dt * dt)/mass[i];
        posY[i] += dt * velY[i] + (0.5f * forceY[i] * dt * dt)/mass[i];
        posZ[i] += dt * velZ[i] + (0.5f * forceZ[i] * dt * dt)/mass[i];

        velX[i] = (posX[i] - x)/dt;
        velY[i] = (posY[i] - y)/dt;
        velZ[i] = (posZ[i] - z)/dt;

        coords[3*i] = posX[i] + offsetX;
        coords[3*i + 1] = posY[i] + offsetY;
        coords[3*i + 2] = posZ[i];

//        float v = pow(1.25, (velX[i]*velX[i] + velY[i]*velY[i] + velZ[i]*velZ[i]) / color_scale);
////        cout << v << endl;
//
//        v = min((v), 255.0f);
//        colors[3* i] = 255 - v;
//        colors[3*i + 1] = 255 - v/1.5;
//        colors[3*i + 2] = 255;

        forceX[i] = 0;
        forceY[i] = 0;
        forceZ[i] = 0;
    }

    colors[0] = 0;
    colors[1] = 0;
    colors[2] = 0;
}

float *particle_system::getCoords() const {
    return coords;
}

unsigned char *particle_system::getColors() const {
    return colors;
}


