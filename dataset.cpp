#include "dataset.h"

#include <condition_variable>
#include <cstring>
#include <fstream>
#include <iomanip>

int writeIndex = 0;
std::mutex mtx;
std::condition_variable cv;
int fileOffset = 0;

void WriteThreaded(char* mappedData, double data, int index)
{
    char buffer[28];

    int len = snprintf(buffer, 28, "%.16g\n", data);

    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [&] {return writeIndex == index;});

    std::memcpy(mappedData+fileOffset, buffer, len);
    fileOffset += len;

    writeIndex++;

    cv.notify_all();
}

void Write128(const std::string& filename, std::array<double, 8128>& data)
{
    int fd = open(filename.c_str(), O_RDWR | O_CREAT, 0666);
    size_t fileSize = 164383;

    ftruncate(fd, fileSize);

    char* mappedData = static_cast<char*>(mmap(nullptr, fileSize, PROT_WRITE, MAP_SHARED, fd, 0));

    char buffer[28]; // Sufficient size for a double + newline

    for (int i = 0; i < 8128; ++i) {
        int len = snprintf(buffer, 28, "%.16g\n", data[i]);
        std::memcpy(mappedData, buffer, len);
        mappedData += len;
    }

    msync(mappedData, fileSize, MS_SYNC);
    munmap(mappedData, fileSize);
    close(fd);
}

void Write256(const std::string& filename, std::array<double, 32640>& data)
{
    int fd = open(filename.c_str(), O_RDWR | O_CREAT, 0666);
    size_t fileSize = 666539;

    ftruncate(fd, fileSize);

    char* mappedData = static_cast<char*>(mmap(nullptr, fileSize, PROT_WRITE, MAP_SHARED, fd, 0));

    char buffer[28]; // Sufficient size for a double + newline

    for (int i = 0; i < 32640; ++i) {
        int len = snprintf(buffer, 28, "%.16g\n", data[i]);
        std::memcpy(mappedData, buffer, len);
        mappedData += len;
    }

    msync(mappedData, fileSize, MS_SYNC);
    munmap(mappedData, fileSize);
    close(fd);
}

void Write512(const std::string& filename, std::array<double, 130816>& data)
{
    int fd = open(filename.c_str(), O_RDWR | O_CREAT, 0666);
    size_t fileSize = 2689713;

    ftruncate(fd, fileSize);

    char* mappedData = static_cast<char*>(mmap(nullptr, fileSize, PROT_WRITE, MAP_SHARED, fd, 0));

    char buffer[28]; // Sufficient size for a double + newline

    for (int i = 0; i < 130816; ++i) {
        int len = snprintf(buffer, 28, "%.16g\n", data[i]);
        std::memcpy(mappedData, buffer, len);
        mappedData += len;
    }

    msync(mappedData, fileSize, MS_SYNC);
    munmap(mappedData, fileSize);
    close(fd);
}

void Write1024(const std::string& filename, double* data)
{
    int fd = open(filename.c_str(), O_RDWR | O_CREAT, 0666);
    size_t fileSize = 10822467;

    ftruncate(fd, fileSize);

    char* mappedData = static_cast<char*>(mmap(nullptr, fileSize, PROT_WRITE, MAP_SHARED, fd, 0));

    char buffer[28]; // Sufficient size for a double + newline

    for (int i = 0; i < 8128; ++i) {
        int len = snprintf(buffer, 28, "%.16g\n", data[i]);
        std::memcpy(mappedData, buffer, len);
        mappedData += len;
    }

    msync(mappedData, fileSize, MS_SYNC);
    munmap(mappedData, fileSize);
    close(fd);
}
