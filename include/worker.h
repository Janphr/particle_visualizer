//
// Created by janphr on 07.09.20.
//

#pragma once

#include "particle.h"

class worker {

public:
    worker(vector<particle> *_particles, int _index, int _worker_count, float *_dt) {
        particles = _particles;
        index = _index;
        worker_count = _worker_count;
        dt = _dt;

    }
    vector<particle> *particles;
    float *dt;
    int index;
    int worker_count;
};
