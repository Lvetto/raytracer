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