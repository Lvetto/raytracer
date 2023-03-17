#include <iostream>
#include <SDL2/SDL.h>
#include <cmath>
#include "sdl_functions.h"
#include "linalg.h"

vector collide(ray a, surface b) {
    double ma[] = {
        a.m_vers[0], -b.m_v1[0], -b.m_v2[0],
        a.m_vers[1], -b.m_v1[1], -b.m_v2[1],
        a.m_vers[2], -b.m_v1[2], -b.m_v2[2],
    };
    matrix m(3, ma);
    m = m.inverse();
    vector t(3);
    t = m * (b.m_pos + (a.m_pos * -1));

    return t;
}

matrix rot(vector axis, double angle) {
    // https://www.youmath.it/domande-a-risposte/view/6230-rotazione.html
    if (axis.norm() == 0) {
        double a[] = {
            1, 0, 0,
            0, 1, 0,
            0, 0, 1
        };
        return matrix(3, a);
    }

    double s = 1. / sqrt(axis * axis);
    axis = axis * s;
    double a[] = {
        pow(axis[0],2)+(1-pow(axis[0],2))*cos(angle), (1-cos(angle))*axis[0]*axis[1]-sin(angle)*axis[2], (1-cos(angle))*axis[0]*axis[2]+sin(angle)*axis[1],
        (1-cos(angle))*axis[0]*axis[1]+sin(angle)*axis[3], pow(axis[1],2)+(1-pow(axis[1],2))*cos(angle), (1-cos(angle))*axis[1]*axis[2]-sin(angle)*axis[0],
        (1-cos(angle))*axis[0]*axis[2]-sin(angle)*axis[1], (1-cos(angle))*axis[1]*axis[2]+sin(angle)*axis[0], pow(axis[2],2)+(1-pow(axis[2],2))*cos(angle)
    };
    return matrix(3, a);
}

vector cross_prod(vector a, vector b) {
    double t[] = {
        a[1]*b[2] - a[2]*b[1],
        a[2]*b[0] - a[0]*b[2],
        a[0]*b[1] - a[1]*b[0]
    };
    return vector(3, t);
}

double angle(vector a, vector b) {
    return acos((a*b)/(a.norm() * b.norm()));
}

#define FPS 60

int main() {
    app_data app;
    init(&app, 1000, 500);

    // double p0a[] = {0, 250, 250}; vector p0(3, p0a);
    // double v0a[] = {7, -2, -0.6}; vector v0(3, v0a);
    // ray r(p0, v0);

    // double p1a[] = {0, 30, 0}; vector p1(3, p1a);
    // double v1a[] = {0, 0, 1000}; vector v1(3, v1a);
    // double v2a[] = {1000, 0, 0}; vector v2(3, v2a);
    // surface s(p1, v1, v2);

    // vector c(3);
    // c = collide(r, s);

    // vector norm = cross_prod(s.m_v1, s.m_v2);
    // vector axis = cross_prod(r.m_vers, norm);
    // matrix rm = rot(axis, 2 * angle(r.m_vers, norm));
    // vector v3 = rm * r.m_vers * -1;

    // printf("%f %f\n", angle(v0, s.m_v1), angle(s.m_v1, v3));
    // printf("%f %f\n", angle(v0, s.m_v2), angle(s.m_v2, v3));

    while (true) {
        Uint32 t0 = SDL_GetTicks();

        event_handle();

        Uint32 t1 = SDL_GetTicks();
        int delay = 1000/FPS - (t1-t0);
        SDL_RenderPresent(app.renderer);
        if (delay > 0) SDL_Delay(delay);


        //break; // debug
    }

    return 0;
}
