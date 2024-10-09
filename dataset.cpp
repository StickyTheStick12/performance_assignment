#include "dataset.h"

#include <condition_variable>
#include <cstring>

int writeIndex = 0;
std::mutex mtx;
std::condition_variable cv;
int fileOffset = 0;

void Write128Threaded(char* mappedData, double data, int index)
{
    char buffer[28];

    int len = snprintf(buffer, 28, "%.15g\n", data);

    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [&] {return writeIndex == index;});

    std::memcpy(mappedData+fileOffset, buffer, len);
    fileOffset += len;

    writeIndex++;

    cv.notify_all();
}

double CharArrToDouble(const char *arr)
{
    int firstDigit = arr[2] - '0'; //see if we replace with 48 is better?
    int SecondDigit = arr[3] - '0';
    int thirdDigit = arr[4] - '0';

    return firstDigit * 0.1 + SecondDigit*0.01 + thirdDigit*0.001;
}

void Write128(const std::string& filename, std::array<double, 8128>& data)
{
    int fd = open(filename.c_str(), O_RDWR | O_CREAT, 0666);
    size_t fileSize = 165000;

    ftruncate(fd, fileSize);

    char* mappedData = static_cast<char*>(mmap(nullptr, fileSize, PROT_WRITE, MAP_SHARED, fd, 0));

    char buffer[28]; // Sufficient size for a double + newline

    for (int i = 0; i < 8128; ++i) {
        int len = snprintf(buffer, 28, "%.15g\n", data[i]);

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
    size_t fileSize = 667000;

    ftruncate(fd, fileSize);

    char* mappedData = static_cast<char*>(mmap(nullptr, fileSize, PROT_WRITE, MAP_SHARED, fd, 0));

    char buffer[28]; // Sufficient size for a double + newline

    for (int i = 0; i < 32640; ++i) {
        int len = snprintf(buffer, 28, "%.15g\n", data[i]);
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
    size_t fileSize = 2690000;

    ftruncate(fd, fileSize);

    char* mappedData = static_cast<char*>(mmap(nullptr, fileSize, PROT_WRITE, MAP_SHARED, fd, 0));

    char buffer[28]; // Sufficient size for a double + newline

    for (int i = 0; i < 130816; ++i) {
        int len = snprintf(buffer, 28, "%.15g\n", data[i]);
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
    size_t fileSize = 10830000;

    ftruncate(fd, fileSize);

    char* mappedData = static_cast<char*>(mmap(nullptr, fileSize, PROT_WRITE, MAP_SHARED, fd, 0));

    char buffer[28]; // Sufficient size for a double + newline

    for (int i = 0; i < 8128; ++i) {
        int len = snprintf(buffer, 28, "%.15g\n", data[i]);
        std::memcpy(mappedData, buffer, len);
        mappedData += len;
    }

    msync(mappedData, fileSize, MS_SYNC);
    munmap(mappedData, fileSize);
    close(fd);
}
