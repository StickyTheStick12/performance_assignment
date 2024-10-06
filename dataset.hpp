#ifndef DATASET_H
#define DATASET_H

#include <fstream>
#include <array>
#include "vector.h"

void Read(const std::string& filename, std::array<Vector, 1024>& matrix);
void Write(const std::string& filename, std::array<std::array<double, 1024>, 1024>& matrix);

#endif
