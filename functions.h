#include "linalg.h"

struct app_data {   // struct to represet window information
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Surface *window_surf;
};

void init(app_data *app, int width, int height);    // initialize window

void event_handle();    // handle window events

Uint8* read_pixel(SDL_Surface* surf, int x, int y); // read pixel from a surface

void write_pixel(SDL_Surface* surf, int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a);  // write pixel to surface

class ray {
    public:

    ray(vector p, double phi, double theta);
    ray(vector p, vector vers);

    vector operator()(double t);

    vector m_pos, m_vers;   // base position and orientation
    double m_theta, m_phi;  // orientation
};

class surface {
    public:

    surface(vector p, vector v1, vector v2);

    vector operator()(double a, double b);

    vector m_pos, m_v1, m_v2;   // base position and characteristic vectors
};
