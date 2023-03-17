#include <iostream>
#include <cmath>
#include <SDL2/SDL.h>
#include "linalg.h"

vector::vector (int s) {
        size = s;
        comps = static_cast<double*>(::operator new(s * sizeof(double)));
    }

vector::vector(int s, double *c) {
        size = s;
        comps = static_cast<double*>(::operator new(s * sizeof(double)));
        std::copy(c, c+s, comps);
    }

vector* vector::operator=(const vector b) {
        if (size != b.size) {
            this->size = b.size;
            comps = static_cast<double*>(::operator new(b.size * sizeof(double)));
        }
            std::copy(b.comps, b.comps + b.size, comps);
            return this;
    }

vector* vector::operator=(const double b[]) {
    std::copy(b, b + size, comps);
    return this;
}

vector vector::operator+(const vector &b) {
        vector o(size);
        if (size != b.size) {
            std::cout << "size doesn't match" << std::endl;
        }
        else {
            for (int i=0; i<size; i++)
                o.comps[i] = comps[i] + b.comps[i];
        }
        return o;
    }

vector vector::operator*(const double a) {
        vector o(size);
        for (int i=0; i<size; i++)
            o.comps[i] = a * comps[i];
        return o;
    }

double vector::operator*(const vector &b) {
        if (size != b.size) {
            std::cout << "size doesn't match" << std::endl;
            return 0;
        }
        else {
            double t=0;
            for (int i=0; i<size; i++)
                t += comps[i] * b.comps[i];
            return t;
        }
    }

double vector::operator[](int i) {
        return this->comps[i];
    }

double *vector::operator[](int i) const {
        return comps;
}

void vector::print() {
    for (int i=0; i<size; i++) {
        printf("%f ", comps[i]);
    }
    printf("\n");
}

double vector::norm() {
    double s = 0;
    for (int i=0; i<size; i++) {
        s+=pow(comps[i], 2);
    }
    return sqrt(s);
}

matrix::matrix (int s) {
        size = s;
        coeffs = static_cast<double*>(::operator new((s * s) * sizeof(double)));
    }

matrix::matrix (int s, double *c) {
        size = s;
        coeffs = static_cast<double*>(::operator new((s*s) * sizeof(double)));
        std::copy(c, c+(s*s), coeffs);
    }

vector matrix::operator*(vector a) {
        vector o(size);
        if (size != a.size)
            std::cout << "size doesn't match" << std::endl;
        else {
            for (int i=0; i<size; i++) {
                double t1[size];
                for (int j=0; j<size; j++)
                    t1[j] = coeffs[(i * size)+j];
                vector v1(size, t1);
                o.comps[i] = v1 * a;
            }
        }
        return o;
    }

double matrix::cof(int i, int j) {
        matrix t(size - 1);
        int k = 0;

        for (int a=0; a<size; a++) {

            for (int b=0; b<size; b++) {

                if ((b != i)  and (a != j)) {
                    t.coeffs[k] = coeffs[(a*size) + b];
                    k++;
                }
            }
        }
        return pow(-1, i+j) * t.det();
    }

double matrix::det() {
        if (size == 2)
            return coeffs[0] * coeffs[3] - coeffs[1] * coeffs[2];
        else {
            double t=0;
            for (int i=0; i<size; i++)
                t += coeffs[i] * cof(i, 0);
            return t;
        }
    }

matrix matrix::transpose() {
    matrix out(size);

    for (int i=0; i<size; i++) {
        for (int j=0; j<size; j++) {
            out.coeffs[(size * i) + j] = coeffs[(size * j) + i];
        }
    }
    return out;
}

matrix* matrix::operator=(const matrix m) {
    if (size != m.size) {
            size = m.size;
            coeffs = static_cast<double*>(::operator new((m.size * m.size) * sizeof(double)));
        }
            std::copy(m.coeffs, m.coeffs + (m.size * m.size), coeffs);
            return this;
}

matrix matrix::inverse() {
    double cofs[size*size];
    double d = det();
    for (int i=0; i<size; i++) {
        for (int j=0; j<size; j++) {
            cofs[(size * i) + j] = cof(i, j)/d;
        }
    }
    matrix b(size, cofs);
    return b;
}

void matrix::print() {
    for (int i=0; i<size; i++) {
        for (int j=0; j<size; j++) {
            printf("%f ", coeffs[(size * i) + j]);
        }
        printf("\n");
    }
    printf("\n");
}

vector cross_prod(vector a, vector b) {
    double t[] = {
        a[1]*b[2] - a[2]*b[1],
        a[2]*b[0] - a[0]*b[2],
        a[0]*b[1] - a[1]*b[0]
    };
    return vector(3, t);
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

double angle(vector a, vector b) {
    return acos((a*b)/(a.norm() * b.norm()));
}
