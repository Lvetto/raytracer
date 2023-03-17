struct app_data {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Surface *window_surf;
};

void init(app_data *app, int width, int height);

void event_handle();

Uint8* read_pixel(SDL_Surface* surf, int x, int y);

void write_pixel(SDL_Surface* surf, int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

class ray {
    public:

    ray(vector p, double phi, double theta);
    ray(vector p, vector vers);

    vector operator()(double t);

    vector m_pos, m_vers;
    double m_theta, m_phi;
};

class surface {
    public:

    surface(vector p, vector v1, vector v2);

    vector operator()(double a, double b);

    vector m_pos, m_v1, m_v2;
};
