#ifndef ANALYSIS_H
#define ANALYSIS_H

#include <array>
#include "vector.h"
#include <atomic>

void CorrelationCoefficients128(std::array<Vector128, 128> &matrix, std::array<double, 8128>& data);
void CorrelationCoefficients256(std::array<Vector256, 256> &matrix, std::array<double, 32640>& data);
void CorrelationCoefficients512(std::array<Vector512, 512> &matrix, std::array<double, 130816>& data);
void CorrelationCoefficients1024(Vector1024* matrix, std::array<double, 523776>& data);

void CorrelationCoefficients128Threaded(std::array<Vector128, 128> &matrix, std::array<double, 8128>& data);
void CorrelationCoefficients256Threaded(std::array<Vector256, 256> &matrix, std::array<double, 32640>& data);
void CorrelationCoefficients512Threaded(std::array<Vector512, 512> &matrix, std::array<double, 130816>& data);
void CorrelationCoefficients1024Threaded(Vector1024* matrix, std::array<double, 523776>& data);

#endif
