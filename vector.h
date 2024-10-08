#ifndef VECTOR_H
#define VECTOR_H

#include "immintrin.h"
#include <cmath>
#include <atomic>
#include <thread>

class Vector128
{
    unsigned size = 0;
    double sum = 0;

public:
    alignas(32) double data[128];
    void Add(double num);
    double Mean() const;
    void operator-(double sub);
    void operator/(double divisor);
    double Magnitude() const;
    double DotProduct(const Vector128& other) const;
};

class Vector256
{
    unsigned size = 0;
    double sum = 0;

public:
    alignas(32) double data[256];
    void Add(double num);
    double Mean() const;
    void operator-(double sub);
    void operator/(double divisor);
    double Magnitude() const;
    double DotProduct(const Vector256& other) const;
};

class Vector512
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
    double DotProduct(const Vector512& other) const;
};

class Vector1024
{
    unsigned size = 0;
    double sum = 0;

public:
    alignas(32) double data[1024];
    void Add(double num);
    double Mean() const;
    void operator-(double sub);
    void operator/(double divisor);
    double Magnitude() const;
    double DotProduct(const Vector1024& other) const;
};

#endif
