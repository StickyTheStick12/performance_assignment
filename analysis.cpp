#include "analysis.h"

std::atomic<int> counter(0);
std::atomic<int> index(0);


void CorrelationCoefficients128(std::array<Vector128, 128> &matrix, std::array<double, 8128>& data)
{
    int index = 0;
    for(unsigned i = 0; i < 127; ++i)
        for(unsigned j = (i+1); j < 128; ++j)
            data[index++] = Pearson128(matrix[i], matrix[j]);
}

void CorrelationCoefficients256(std::array<Vector256, 256> &matrix, std::array<double, 32640>& data)
{
    int index = 0;
    for(unsigned i = 0; i < 255; ++i)
        for(unsigned j = (i+1); j < 256; ++j)
            data[index++] = Pearson256(matrix[i], matrix[j]);
}

void CorrelationCoefficients512(std::array<Vector512, 512> &matrix, std::array<double, 130816>& data)
{
    int index = 0;
    for(unsigned i = 0; i < 511; ++i)
        for(unsigned j = (i+1); j < 512; ++j)
            data[index++] = Pearson512(matrix[i], matrix[j]);
}

int CorrelationCoefficients1024(std::array<Vector1024, 512> &matrix, double* data, int index)
{
    for(unsigned i = 0; i < 511; ++i)
        for(unsigned j = (i+1); j < 512; ++j)
            data[index++] = Pearson1024(matrix[i], matrix[j]);

    return index;
}

void CorrelationCoefficients128Threaded(std::array<Vector128, 128> &matrix, std::array<double, 8128>& data)
{
    for(unsigned i = counter.fetch_add(1); i < 127;)
        for(unsigned j = (i+1); j < 128; ++j)
            data[index.fetch_add(1)] = Pearson128(matrix[i], matrix[j]);
}

void CorrelationCoefficients256Threaded(std::array<Vector256, 256> &matrix, std::array<double, 32640>& data)
{
    for(unsigned i = counter.fetch_add(1); i < 255;)
        for(unsigned j = (i+1); j < 256; ++j)
            data[index.fetch_add(1)] = Pearson256(matrix[i], matrix[j]);
}

void CorrelationCoefficients512Threaded(std::array<Vector512, 512> &matrix, std::array<double, 130816>& data)
{
    for(unsigned i = counter.fetch_add(1); i < 511; ++i)
        for(unsigned j = (i+1); j < 512; ++j)
            data[index.fetch_add(1)] = Pearson512(matrix[i], matrix[j]);
}

int CorrelationCoefficients1024Threaded(std::array<Vector1024, 512> &matrix, double* data)
{
    for(unsigned i = counter.fetch_add(1); i < 511; ++i)
        for(unsigned j = (i+1); j < 512; ++j)
            data[index.fetch_add(1)] = Pearson1024(matrix[i], matrix[j]);

    return index;
}

double Pearson128(Vector128 vec1, Vector128 vec2)
{
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

double Pearson256(Vector256 vec1, Vector256 vec2)
{
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

double Pearson512(Vector512 vec1, Vector512 vec2)
{
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

double Pearson1024(Vector1024 vec1, Vector1024 vec2)
{
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
