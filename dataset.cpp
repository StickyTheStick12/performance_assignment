#include "dataset.h"

#include <condition_variable>
#include <cstring>
#include <iomanip>

int writeIndex = 0;
std::mutex mtx;
std::condition_variable cv;
int fileOffset = 0;

void Write1024Threaded(char* mappedData, std::array<double, 523776>& data, int startIndex, int endIndex, int threadId)
{
    char* buff = new char[24*(endIndex - startIndex)];

    char buffer[24];
    int maxLen = 0;

    for (int i = startIndex; i < endIndex; ++i) {
        int len = snprintf(buffer, 24, "%.16g\n", data[i]); // Use sizeof to avoid buffer overflow
        std::memcpy(buff + maxLen, buffer, len); // Copy to the correct position in buff
        maxLen += len; // Increment
    }

    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [&] {return writeIndex == threadId;});

    std::memcpy(mappedData+fileOffset, buff, maxLen);
    fileOffset += maxLen;

    writeIndex++;

    cv.notify_all();

    delete[] buff;
}

void Write512Threaded(char* mappedData, std::array<double, 130816>& data, int startIndex, int endIndex, int threadId)
{
    char* buff = new char[24*(endIndex - startIndex)];

    char buffer[24];
    int maxLen = 0;

    for (int i = startIndex; i < endIndex; ++i) {
        int len = snprintf(buffer, 24, "%.16g\n", data[i]); // Use sizeof to avoid buffer overflow
        std::memcpy(buff + maxLen, buffer, len); // Copy to the correct position in buff
        maxLen += len; // Increment
    }

    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [&] {return writeIndex == threadId;});

    std::memcpy(mappedData+fileOffset, buff, maxLen);
    fileOffset += maxLen;

    writeIndex++;

    cv.notify_all();

    delete[] buff;
}

void Write256Threaded(char* mappedData, std::array<double, 32640>& data, int startIndex, int endIndex, int threadId)
{
    char* buff = new char[24*(endIndex - startIndex)];

    char buffer[24];
    int maxLen = 0;

    for (int i = startIndex; i < endIndex; ++i) {
        int len = snprintf(buffer, 24, "%.16g\n", data[i]); // Use sizeof to avoid buffer overflow
        std::memcpy(buff + maxLen, buffer, len); // Copy to the correct position in buff
        maxLen += len; // Increment
    }

    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [&] {return writeIndex == threadId;});

    std::memcpy(mappedData+fileOffset, buff, maxLen);
    fileOffset += maxLen;

    writeIndex++;

    cv.notify_all();

    delete[] buff;
}

void Write128Threaded(char* mappedData, std::array<double, 8128>& data, int startIndex, int endIndex, int threadId)
{
    char* buff = new char[24*(endIndex - startIndex)];

    char buffer[24];
    int maxLen = 0;

    for (int i = startIndex; i < endIndex; ++i) {
        int len = snprintf(buffer, 24, "%.16g\n", data[i]); // Use sizeof to avoid buffer overflow
        std::memcpy(buff + maxLen, buffer, len); // Copy to the correct position in buff
        maxLen += len; // Increment
    }

    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [&] {return writeIndex == threadId;});

    std::memcpy(mappedData+fileOffset, buff, maxLen);
    fileOffset += maxLen;

    writeIndex++;

    cv.notify_all();

    delete[] buff;
}

void Write128(const std::string& filename, std::array<double, 8128>& data)
{
    int fd = open(filename.c_str(), O_RDWR | O_CREAT, 0666);
    size_t fileSize = 164383;

    ftruncate(fd, fileSize);

    char* mappedData = static_cast<char*>(mmap(nullptr, fileSize, PROT_WRITE, MAP_SHARED, fd, 0));
    madvise(mappedData, fileSize, MADV_SEQUENTIAL);

    char buffer[24]; // Sufficient size for a double + newline

    for (int i = 0; i < 8128; ++i) {
        int len = snprintf(buffer, 24, "%.16g\n", data[i]);
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
    madvise(mappedData, fileSize, MADV_SEQUENTIAL);

    char buffer[24]; // Sufficient size for a double + newline

    for (int i = 0; i < 32640; ++i) {
        int len = snprintf(buffer, 24, "%.16g\n", data[i]);
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
    madvise(mappedData, fileSize, MADV_SEQUENTIAL);

    char buffer[24]; // Sufficient size for a double + newline

    for (int i = 0; i < 130816; ++i) {
        int len = snprintf(buffer, 24, "%.16g\n", data[i]);
        std::memcpy(mappedData, buffer, len);
        mappedData += len;
    }

    msync(mappedData, fileSize, MS_SYNC);
    munmap(mappedData, fileSize);
    close(fd);
}

void Write1024(const std::string& filename, std::array<double, 523776>& data)
{
    int fd = open(filename.c_str(), O_RDWR | O_CREAT, 0666);
    size_t fileSize = 10822467;

    int n = 0;

    ftruncate(fd, fileSize);

    char* mappedData = static_cast<char*>(mmap(nullptr, fileSize, PROT_WRITE, MAP_SHARED, fd, 0));
    madvise(mappedData, fileSize, MADV_SEQUENTIAL);

    char buffer[24]; // Sufficient size for a double + newline

    for (int i = 0; i < 523776; ++i) {
        int len = snprintf(buffer, 24, "%.16g\n", data[i]);
        std::memcpy(mappedData, buffer, len);
        mappedData += len;

        n += len;
    }

    msync(mappedData, fileSize, MS_SYNC);
    munmap(mappedData, fileSize);
    close(fd);
}
