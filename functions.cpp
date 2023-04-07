#include <iostream>
#include <cmath>
#include <SDL2/SDL.h>
#include "functions.h"

void init(app_data *app, int width, int height) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
        std::cout << "Initialization error\n";
        exit(-1);
    }

    app->window = SDL_CreateWindow("Finestra", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 0);
    app->renderer = SDL_CreateRenderer(app->window, -1, SDL_RENDERER_ACCELERATED);
    app->window_surf = SDL_GetWindowSurface(app->window);

    if ((not app->window) or (not app->renderer) or (not app->window_surf)) {
        std::cout << "Initialization error\n";
        exit(-1);
    }
}

void event_handle() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            SDL_Quit();
            exit(0);
            break;
        
        default:
            break;
        }
    }
}

Uint8* read_pixel(SDL_Surface* surf, int x, int y) {
    SDL_LockSurface(surf);
    Uint32 *pixarr = (Uint32 *)surf->pixels;
    Uint32 pix = pixarr[(y * surf->w) + x];
    Uint8 *out = new Uint8[4];
    SDL_GetRGBA(pix, surf->format, &(out[0]), &(out[1]), &(out[2]), &(out[3]));
    SDL_UnlockSurface(surf);
    return out;
}

void write_pixel(SDL_Surface* surf, int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    SDL_LockSurface(surf);
    Uint32 *pixarr = (Uint32 *)surf->pixels;
    pixarr[(y * surf->w) + x] = SDL_MapRGBA(surf->format, r, g, b, a);
    SDL_UnlockSurface(surf);
}

ray::ray(vector p, double phi, double theta) : m_pos(3), m_vers(3) {
        m_theta = theta; m_phi = phi; m_pos = p;
        m_vers.comps[0] = sin(phi) * cos(theta);
        m_vers.comps[1] = sin(phi) * sin(theta); 
        m_vers.comps[2] = cos(theta); 
    }

ray::ray(vector p, vector vers) : m_pos(3), m_vers(3) {
        m_vers = vers;
        m_pos = p;
    }

vector ray::operator()(double t) {
        vector out(3);
        return m_pos + m_vers * t;
    }

surface::surface(vector p, vector v1, vector v2, bool is_light=false) : m_pos(3), m_v1(3), m_v2(3) {
    m_pos = p;
    m_v1 = v1;
    m_v2 = v2;
    m_is_light = is_light;
}

vector surface::operator()(double a, double b) {
    return m_pos + m_v1 * a + m_v2 * b;
}

vector intersect(ray a, ray b) {
    vector v3 = cross_prod(a.m_vers, b.m_vers);
    double ma[] = {
        a.m_vers[0], -b.m_vers[0], v3[0],
        a.m_vers[1], -b.m_vers[1], v3[1],
        a.m_vers[2], -b.m_vers[2], v3[2]
    };
    matrix m(3, ma);
    return a((m.inverse() * (b.m_pos + a.m_pos * -1 + v3))[0]);
}
