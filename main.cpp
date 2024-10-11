#include "analysis.h"
#include "vector.h"
#include "dataset.h"
#include <atomic>
#include <pthread.h>
#include <charconv>

//TODO
// 1: fix multithreading
// 2: find improvements for report
// 3: fix madvise for threaded, use willneed/dontneed?

int inFile;
int outFile;
off_t inFileSize;

std::atomic<int> counterIn(0);
std::atomic<int> counterOut(0);
char* mappedDataThread;
int endPointThread;
char* mappedOutThread;

void* Thread128(std::array<Vector128, 128>* matrix)
{
    int i;

    while ((i = counterIn.fetch_add(1)) < 128)
    {
        for(int x = (i*128); x < (i*128)+128; ++x)
        {
            double d;
            std::from_chars(mappedDataThread+endPointThread+6*x, mappedDataThread+endPointThread+6*x+5, d);
            (*matrix)[i].Add(d);
        }
    }

    if(i == 127)
    {
        munmap(mappedDataThread, inFileSize);
        close(inFile);
    }

    std::array<double, 8128> data;

    CorrelationCoefficients128Threaded(*matrix, data);

    int x = 0;

    for(x = counterOut.fetch_add(1); i < 8128;)
        WriteThreaded(mappedOutThread, data[i], i);

    if(x == 8127)
    {
        msync(mappedOutThread, 164383, MS_SYNC);
        munmap(mappedOutThread, 164383);
        close(outFile);
    }

    return nullptr;
}

void Thread256(char* mappedData, std::array<Vector256, 256>& matrix, int endPoint, char* mappedOut)
{
    int i = 0;

    for(i = counterIn.fetch_add(1); i < 256;)
    {
        for(int x = (i*256); x < (i*256)+256; ++x)
        {
            double d;
            std::from_chars(mappedData+endPoint+6*x, mappedData+endPoint+6*x+5, d);
            matrix[i].Add(d);
        }
    }

    if(i == 255)
    {
        munmap(mappedData, inFileSize);
        close(inFile);
    }

    std::array<double, 32640> data;

    CorrelationCoefficients256Threaded(matrix, data);

    int x = 0;

    for(x = counterOut.fetch_add(1); i < 32640;)
        WriteThreaded(mappedOut, data[i], i);

    if(x == 32639)
    {
        msync(mappedOut, 666539, MS_SYNC);
        munmap(mappedOut, 666539);
        close(outFile);
    }
}

void Thread512(char* mappedData, std::array<Vector512, 512>& matrix, int endPoint, char* mappedOut)
{
    int i = 0;

    for(i = counterIn.fetch_add(1); i < 512;)
    {
        for(int x = (i*512); x < (i*512)+512; ++x)
        {
            double d;
            std::from_chars(mappedData+endPoint+6*x, mappedData+endPoint+6*x+5, d);
            matrix[i].Add(d);
        }
    }

    if(i == 511)
    {
        munmap(mappedData, inFileSize);
        close(inFile);
    }

    std::array<double, 130816> data;

    CorrelationCoefficients512Threaded(matrix, data);

    int x = 0;

    for(x = counterOut.fetch_add(1); i < 130816;)
        WriteThreaded(mappedOut, data[i], i);

    if(x == 130815)
    {
        msync(mappedOut, 2689713, MS_SYNC);
        munmap(mappedOut, 2689713);
        close(outFile);
    }
}

void Thread1024(char* mappedData, Vector1024* matrix, int endPoint, char* mappedOut)
{
    int i = 0;

    for(i = counterIn.fetch_add(1); i < 1024;)
    {
        for(int x = (i*1024); x < (i*1024)+1024; ++x)
        {
            double d;
            std::from_chars(mappedData+endPoint+6*x, mappedData+endPoint+6*x+5, d);
            matrix[i].Add(d);
        }
    }

    if(i == 1023)
    {
        munmap(mappedData, inFileSize);
        close(inFile);
    }

    std::array<double, 523776> data;

    CorrelationCoefficients1024Threaded(matrix, data);

    int x = 0;

    for(x = counterOut.fetch_add(1); i < 523776;)
        WriteThreaded(mappedOut, data[i], i);

    if(x == 523775)
    {
        msync(mappedOut, 10822467, MS_SYNC);
        munmap(mappedOut, 10822467);
        close(outFile);
    }
}

int main(int argc, char const* argv[])
{
    int file = open("1024.data", O_RDONLY);

    off_t size = lseek(file, 0, SEEK_END);

    char* dataPtr = static_cast<char*>(mmap(nullptr, size, PROT_READ, MAP_PRIVATE, file, 0));

    char* mappedData = dataPtr;

    madvise(mappedData, size, MADV_SEQUENTIAL);

    int dimension = 0;

    while(*mappedData != '\n') {
        dimension = dimension * 10 + (*mappedData - '0');
        ++mappedData;
    }

    ++mappedData;

    if(dimension == 128) {
        std::array<Vector128, 128> matrix;

        if("1" != "1")
        {
            //todo change madvise here to something else. Prob willneed and then calculate how many bytes there are in a line

            outFile = open("out.txt", O_WRONLY, 0666);

            size_t outFileSize = 164383;
            ftruncate(outFile, outFileSize);

            char* mappedOut = static_cast<char*>(mmap(nullptr, outFileSize, PROT_WRITE, MAP_SHARED, outFile, 0));

            //create thread array
            pthread_t threads[32];

            inFile = file;
            inFileSize = size;

            int nrThreads = 1;

            mappedDataThread = mappedData;
            mappedOutThread = mappedOut;

            for(int i = 0; i < nrThreads; ++i)
                pthread_create(&threads[i], nullptr, (void * (*)(void *))Thread128, &matrix);

            Thread128(&matrix);

            for(int i = 0; i < nrThreads; ++i)
                pthread_join(threads[i], nullptr);
        }

        for(int i = 0; i < dimension; ++i)
            for(int j = 0; j < dimension; ++j) {
                double d;
                char* endPtr = mappedData+5;
                std::from_chars(mappedData, endPtr, d);
                matrix[i].Add(d);

                mappedData = endPtr;
                ++mappedData;
            }

        munmap(mappedData, size);
        close(file);

        std::array<double, 8128> data;

        CorrelationCoefficients128(matrix, data);

        Write128(argv[2], data);
        //Write128("out.txt", data);

        return 0;
    }

    if(dimension == 256)
    {
        std::array<Vector256, 256> matrix;

        for(int i = 0; i < dimension; ++i)
            for(int j = 0; j < dimension; ++j) {
                double d;
                char* endPtr = mappedData+5;
                std::from_chars(mappedData, endPtr, d);
                matrix[i].Add(d);

                mappedData = endPtr;
                ++mappedData;
            }

        munmap(mappedData, size);
        close(file);

        std::array<double, 32640> data;

        CorrelationCoefficients256(matrix, data);

        Write256(argv[2], data);

        return 0;
    }

    if(dimension == 512)
    {
        std::array<Vector512, 512> matrix;

        for(int i = 0; i < dimension; ++i)
            for(int j = 0; j < dimension; ++j) {
                double d;
                char* endPtr = mappedData+5;
                std::from_chars(mappedData, endPtr, d);
                matrix[i].Add(d);

                mappedData = endPtr;
                ++mappedData;
            }

        munmap(mappedData, size);
        close(file);

        std::array<double, 130816> data;

        CorrelationCoefficients512(matrix, data);

        Write512(argv[2], data);

        return 0;
    }

    Vector1024* matrix = new Vector1024[1024];

    for(int i = 0; i < dimension; ++i)
        for(int j = 0; j < dimension; ++j) {
            double d;
            char* endPtr = mappedData+5;
            std::from_chars(mappedData, endPtr, d);
            matrix[i].Add(d);

            mappedData = endPtr;
            ++mappedData;
        }

    std::array<double, 523776> data;

    CorrelationCoefficients1024(matrix, data);

    Write1024(argv[2], data);

    return 0;
}
