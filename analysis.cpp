#include "analysis.h"

int Correlation_coefficients(std::array<Vector, 1024> &matrix, std::array<double, 523776>& data)
{
    int dimension = 1024;
    int index = 0;
    for(unsigned i = 0; i < dimension-1; ++i)
    {
        for(unsigned j = (i+1); j < dimension; ++j)
            data[index++] = Pearson(matrix[i], matrix[j]);
    }

    return index;
}

double Pearson(Vector& vec1, Vector& vec2)
{
    double xMean = vec1.Mean();
    double yMean = vec2.Mean();

    vec1 - xMean;
    vec2 - yMean;

    double xMag = vec1.Magnitude();
    double yMag = vec2.Magnitude();

    

    auto x_mm_over_x_mag { x_mm / x_mag };
    auto y_mm_over_y_mag { y_mm / y_mag };

    auto r { x_mm_over_x_mag.dot(y_mm_over_y_mag) };

    return std::max(std::min(r, 1.0), -1.0);
}

