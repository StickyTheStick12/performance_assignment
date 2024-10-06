#ifndef VECTOR_H
#define VECTOR_H

#include "immintrin.h"

class Vector
{
private:
    unsigned size = 0;
    double sum = 0;
    alignas(32) double data[1024];

public:
    void Add(double num);
    double Mean() const;
    void operator-(double sub);
    double Magnitude() const;
};

#endif
