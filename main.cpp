#include <iostream>
#include <SDL2/SDL.h>
#include <cmath>
#include "functions.h"

#define FPS 60

vector collide(ray a, surface b) {  // compute ray-surfafe intersection (coordinates in ray and surface reference frame)
    double ma[] = {
        a.m_vers[0], -b.m_v1[0], -b.m_v2[0],
        a.m_vers[1], -b.m_v1[1], -b.m_v2[1],
        a.m_vers[2], -b.m_v1[2], -b.m_v2[2],
    };
    matrix m(3, ma);

    if (m.det() == 0)   // matrix is singular --> surface isn't valid or is parallel to ray
        return 0;

    m = m.inverse();
    vector t(3);
    t = m * (b.m_pos + (a.m_pos * -1)); // solve linear system (ray coord, surf coord 1, surf coord 2)

    // check if point is in triangle
    vector p = a(t[0]);
    vector p0 = b.m_pos;
    vector r = p + p0 * -1;
    vector v3 = b.m_v1 + b.m_v2 * -1;
    vector p1 = intersect(ray(p, r), ray(p0 + b.m_v2, v3)); // cast a ray passing for the intersection of surf.v1 and surf.v2. find the intersection with <v3>
    double l_coord = (p1 + p * -1)[0] / r[0];   // go from global coordinate to local (with respect to <r>)
    if (l_coord > 0)    // if the coordinate is positive the point is in the triangle
        return a(t[0]); // local coord to global
    else
        return 0;
}

class camera {
    public:

    camera(vector pos, vector dptof, vector width, vector height) {
        m_pos = pos;
        m_dptof = dptof;
        m_width = width;
        m_height = height;

    }

    SDL_Surface render(surface* surf_arr, int surf_count, int max_bounces, int* res) {
        for (int ix=0; ix<res[0]; ix++) {
            for (int iy=0; iy<res[1]; iy++) {
                vector pix_pos(3);
                pix_pos = m_pos + m_dptof +  m_width * ((m_width.norm() / res[0]) * ix - 0.5) + m_height * ((m_height.norm() / res[1]) * iy - 0.5);

                ray r(m_pos, (pix_pos + m_pos * -1) * (1 / (pix_pos + m_pos * -1).norm()));
                for (int bounce=0; bounce < max_bounces; bounce++) {
                    double min_t = 0;
                    for (int i=0; i<surf_count; i++) {
                        double t = collide(r, surf_arr[i])[0];

                        if (t > 0  and t < min_t)
                            min_t = t;
                    }

                    vector new_p = r(min_t);
                    // creare nuovo raggio con p0=new_p e versore ottenuto dalla riflessione
                }


            }

        }
    }

    vector m_pos;   // observer position
    vector m_dptof; // depth of field and orientation
    vector m_width; // screen width and horizontal orientation of the screen
    vector m_height;    // screen height and vertical orientation of the screen
};

int main() {
    app_data app;
    init(&app, 1000, 500);

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
