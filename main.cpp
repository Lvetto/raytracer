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

    if (m.det() == 0)
        return 0;

    m = m.inverse();
    vector t(3);
    t = m * (b.m_pos + (a.m_pos * -1));

    return t;
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
