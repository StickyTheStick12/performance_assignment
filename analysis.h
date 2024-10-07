#ifndef ANALYSIS_H
#define ANALYSIS_H

#include <array>
#include "vector.h"

int Correlation_coefficients(std::array<Vector, 512> &matrix, std::array<double, 130816>& data);
double Pearson(Vector vec1, Vector vec2);

#endif
