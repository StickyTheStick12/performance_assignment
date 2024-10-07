#include "analysis.h"

int Correlation_coefficients(std::array<Vector, 512> &matrix, std::array<double, 130816>& data)
{
    int dimension = 128;
    int index = 0;
    for(unsigned i = 0; i < dimension-1; ++i)
        for(unsigned j = (i+1); j < dimension; ++j)
            data[index++] = Pearson(matrix[i], matrix[j]);

    return index;
}

double Pearson(Vector vec1, Vector vec2)
{
    // todo: if time: for small arrays we have to copy the full 512 array even if we only use 128 positions
    double xMean = vec1.Mean();
    double yMean = vec2.Mean();

    vec1 - xMean;
    vec2 - yMean;

    double xMag = vec1.Magnitude();
    double yMag = vec2.Magnitude();

    vec1 / xMag;
    vec2 / yMag;

    double r = vec1.DotProduct(vec2);

    return std::max(std::min(r, 1.0), -1.0);
}
