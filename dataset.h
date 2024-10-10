#ifndef DATASET_H
#define DATASET_H

#include <array>
#include <sys/mman.h>
#include <fcntl.h>
#include "vector.h"
#include <unistd.h>

void WriteThreaded(char* mappedData, double data, int index);

void Write128(const std::string& filename, std::array<double, 8128>& data);
void Write256(const std::string& filename, std::array<double, 32640>& data);
void Write512(const std::string& filename, std::array<double, 130816>& data);
void Write1024(const std::string& filename, double* data);

#endif
