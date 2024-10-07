#ifndef DATASET_H
#define DATASET_H

#include <fstream>
#include <array>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include "vector.h"
#include <iomanip>


void Read(const std::string& filename, std::array<Vector, 512>& matrix);
void Write(const std::string& filename, std::array<double, 130816>& data, int endPoint);
double CharArrToDouble(const char* arr);

#endif
