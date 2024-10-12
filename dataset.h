#ifndef DATASET_H
#define DATASET_H

#include <array>
#include <sys/mman.h>
#include <fcntl.h>
#include "vector.h"
#include <unistd.h>
#include <string>

void Write1024Threaded(char* mappedData, std::array<double, 523776>& data, int startIndex, int endIndex, int threadId);
void Write128Threaded(char* mappedData, std::array<double, 8128>& data, int startIndex, int endIndex, int threadId);
void Write256Threaded(char* mappedData, std::array<double, 32640>& data, int startIndex, int endIndex, int threadId);
void Write512Threaded(char* mappedData, std::array<double, 130816>& data, int startIndex, int endIndex, int threadId);

void Write128(const std::string& filename, std::array<double, 8128>& data);
void Write256(const std::string& filename, std::array<double, 32640>& data);
void Write512(const std::string& filename, std::array<double, 130816>& data);
void Write1024(const std::string& filename, std::array<double, 523776>& data);

#endif
