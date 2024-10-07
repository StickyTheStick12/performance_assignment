#ifndef VECTOR_H
#define VECTOR_H

#include "immintrin.h"
#include <cmath>

class Vector
{
    unsigned size = 0;
    double sum = 0;

public:
    alignas(32) double data[512];
    void Add(double num);
    double Mean() const;
    void operator-(double sub);
    void operator/(double divisor);
    double Magnitude() const;
    double DotProduct(const Vector& other) const;
};

#endif
