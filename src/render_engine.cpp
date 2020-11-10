//
// Created by janphr on 07.09.20.
//

#include "render_engine.h"
#include <iostream>
#include <sstream>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs.hpp>

render_engine::render_engine(int width, int height, int color_scale, float dt) :
        width(width), height(height), color_scale(color_scale), dt(dt) {
    this->window = new Window(VideoMode(width, height, 32), "Particle Visualizer");

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
//    glOrtho(width/3, width - width/3, height - height/3, height/3, -1, 1);
    glOrtho(0, width, height, 0, -1, 1);
    glEnable(GL_POINT_SIZE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPointSize(1);
}

void render_engine::run(particle_system &ps) {

    int p_count = ps.getParticleCount();
    int counter = 0;

    float coords[2 * p_count];
    unsigned char colors[3 * p_count];

    Event e{};
    Clock deltaTime;
    cv::Mat img(height, width, CV_8UC3);

    while (this->window->isOpen()) {
        while (this->window->pollEvent(e)) {
            if (e.type == Event::Closed)
                this->window->close();
        }

        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);

#pragma omp parallel for
        for (int i = 0; i < p_count; i++) {
            particle *this_particle = &ps.getParticles()[i];
            coords[2 * i] = this_particle->getPos().x;
            coords[2 * i + 1] = this_particle->getPos().y;

            float vx = this_particle->getVel().x;
            float vy = this_particle->getVel().y;
            float v = min((int) (vx*vx + vy*vy) / this->color_scale, 255);
            colors[3 * i] = 255;
            colors[3 * i + 1] = 255 - v;
            colors[3 * i + 2] = 255 - v;
        }

//        cout << coords[0] << " " << coords[1] << endl;


        glPushMatrix();

        glEnableClientState(GL_VERTEX_ARRAY); // we are using VBAs : here's how to draw them
        glEnableClientState(GL_COLOR_ARRAY);


        glVertexPointer(2, GL_FLOAT, 0, &coords);
        glColorPointer(3, GL_UNSIGNED_BYTE, 0, &colors);
        glDrawArrays(GL_POINTS, 0, p_count);


        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_COLOR_ARRAY);

        glPopMatrix();

        glFlush();
        this->window->display();


        ps.update(dt, 1);
//        dt = deltaTime.restart().asSeconds();
//        cout << deltaTime.restart().asSeconds() << endl;

//use fast 4-byte alignment (default anyway) if possible
        glPixelStorei(GL_PACK_ALIGNMENT, (img.step & 3) ? 1 : 4);

//set length of one complete row in destination data (doesn't need to equal img.cols)
        glPixelStorei(GL_PACK_ROW_LENGTH, img.step/img.elemSize());

        glReadPixels(0, 0, img.cols, img.rows, GL_BGR, GL_UNSIGNED_BYTE, img.data);
        std::stringstream ss;
        ss << "../frames/" << counter++ << ".png";
        cv::imwrite( ss.str(), img );
    }

}

void render_engine::sim(particle_system &ps) {
    int p_count = ps.getParticleCount();
    int counter = 0;

    Event e{};
    Clock deltaTime;
    cv::Mat img(height, width, CV_8UC3);

    while (this->window->isOpen()) {
        while (this->window->pollEvent(e)) {
            if (e.type == Event::Closed)
                this->window->close();
        }

        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        glPushMatrix();
        glEnableClientState(GL_VERTEX_ARRAY); // we are using VBAs : here's how to draw them
        glEnableClientState(GL_COLOR_ARRAY);

        ps.step(dt, 1);

        glVertexPointer(3, GL_FLOAT, 0, ps.getCoords());
        glColorPointer(3, GL_UNSIGNED_BYTE, 0, ps.getColors());
        glDrawArrays(GL_POINTS, 0, p_count);


        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_COLOR_ARRAY);

        glPopMatrix();

        glFlush();
        this->window->display();


        glPixelStorei(GL_PACK_ALIGNMENT, (img.step & 3) ? 1 : 4);
        glPixelStorei(GL_PACK_ROW_LENGTH, img.step/img.elemSize());
        glReadPixels(0, 0, img.cols, img.rows, GL_BGR, GL_UNSIGNED_BYTE, img.data);
        std::stringstream ss;
        ss << "../frames/" << counter++ << ".png";
        cv::imwrite( ss.str(), img );

        cout << deltaTime.restart().asSeconds() << endl;
    }
}

