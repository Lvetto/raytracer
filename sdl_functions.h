struct app_data {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Surface *window_surf;
};

void init(app_data *app, int width, int height);

void event_handle();

Uint8* read_pixel(SDL_Surface* surf, int x, int y);

void write_pixel(SDL_Surface* surf, int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

class vector {
    public:

    vector ();

    vector (int size);

    vector(int size, double *c);

    vector *operator=(const vector b);

    vector *operator=(const double b[]);

    vector operator+(const vector &b);

    vector operator*(const double a);

    double operator*(const vector &b);

    double operator[](int i);

    double *operator[](int i) const;

    void print();

    double norm();

    double *comps;
    int size;
};

class matrix {
    public:

    matrix (int size);

    matrix (int size, double *c);

    vector operator*(vector a);

    double cof(int i, int j);

    double det();

    matrix transpose();

    matrix* operator=(const matrix m);

    matrix inverse();

    void print();

    double *coeffs;
    int size;
};

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
