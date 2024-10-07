#ifndef DATASET_H
#define DATASET_H

#include <fstream>
#include <array>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include "vector.h"
#include <iomanip>

void Write128(const std::string& filename, std::array<double, 8128>& data);
void Write256(const std::string& filename, std::array<double, 32640>& data);
void Write512(const std::string& filename, std::array<double, 130816>& data);
void Write1024(const std::string& filename, double* data);
double CharArrToDouble(const char* arr);

#endif
