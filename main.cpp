#include <iostream>
#include <SDL2/SDL.h>
#include <cmath>
#include "functions.h"

#define FPS 60

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
