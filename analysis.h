#ifndef ANALYSIS_H
#define ANALYSIS_H

#include <array>
#include "vector.h"
#include <atomic>

void CorrelationCoefficients128(std::array<Vector128, 128> &matrix, std::array<double, 8128>& data);
void CorrelationCoefficients256(std::array<Vector256, 256> &matrix, std::array<double, 32640>& data);
void CorrelationCoefficients512(std::array<Vector512, 512> &matrix, std::array<double, 130816>& data);
int CorrelationCoefficients1024(std::array<Vector1024, 512> &matrix, double* data, int index=0);

void CorrelationCoefficients128Threaded(std::array<Vector128, 128> &matrix, std::array<double, 8128>& data);
void CorrelationCoefficients256Threaded(std::array<Vector256, 256> &matrix, std::array<double, 32640>& data);
void CorrelationCoefficients512Threaded(std::array<Vector512, 512> &matrix, std::array<double, 130816>& data);
int CorrelationCoefficients1024Threaded(std::array<Vector1024, 512> &matrix, double* data);

double Pearson1024(Vector1024 vec1, Vector1024 vec2);

#endif
