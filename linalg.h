class vector {
    public:

    vector ();

    vector (int size);

    vector(int size, double *c);

    vector *operator=(const vector b);  // assignement operator

    vector *operator=(const double b[]);    // assignement to array of doubles

    vector operator+(const vector &b);  // vector addition

    vector operator*(const double a);   // product by scalar

    double operator*(const vector &b);  // scalar product

    double operator[](int i);   // subscription operator

    double *operator[](int i) const;    // assigne singole component

    void print();   // print components

    double norm();  // norm or modulus

    double *comps;  // components array
    int size;   // number of components
};

class matrix {
    public:

    matrix (int size);

    matrix (int size, double *c);

    vector operator*(vector a); // vector-matrix product

    double cof(int i, int j);   // compute algebric cofactor

    double det();   // compute determinant

    matrix transpose(); // compute transposed matrix

    matrix* operator=(const matrix m);  // assignement operator

    matrix inverse();   // compute inverse matrix

    void print();   // print coefficients

    double *coeffs; // coefficients array
    int size;   // size of the matrix (number of coefficients = size * size)
};

vector cross_prod(vector a, vector b); // compute cross product of two vectors

matrix rot(vector axis, double angle);  // compute rotation matrix given angle and an axis

double angle(vector a, vector b);   // compute angle between vectors