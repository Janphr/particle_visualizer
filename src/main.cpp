#include <iostream>
#include <particle_system.h>
#include <render_engine.h>
#include <cmath>

int main() {

    int
//    width = 3840,
//    height = 2160,
//    width = 1920,
//    height = 1080,
            width = 2880,
            height = 1620;

    float center_mass = 7500;
    float galaxy_mass = center_mass / expf(0.0015);

    int particle_count =
//            50000
            pow(200, 2)
            ;

    cout << "particles: " << particle_count << endl;

    particle_system ps(width, height,2, 1.0e-2, particle_count, 250.0);
    ps.init_galaxy(750.0, 50.0, 2, center_mass, galaxy_mass, 0.001);
    render_engine re(width, height, 15, 1.0e-1);
    re.sim(ps);
//    re.run(ps);

    return 0;
}
