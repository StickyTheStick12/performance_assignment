#include "analysis.h"
#include "vector.h"
#include "dataset.h"
#include <atomic>
#include <thread>
#include <charconv>
#include <cstring>


//TODO
// 1: fix multithreading
// 2: find improvements for report
//either set the arrays as global variables and then pass the other things as parameters, or change the other to loadu.


int inFile;
int outFile;
off_t inFileSize;

std::atomic<int> counterIn(0);
std::atomic<int> counterOut(0);

void Thread128(void* args)
{
    char* mappedData = static_cast<char*>(args); // Cast args to char*
    std::array<Vector128, 128>& matrix = *reinterpret_cast<std::array<Vector128, 128>*>(mappedData + inFileSize); // Extract matrix
    int endPoint = *reinterpret_cast<int*>(mappedData + inFileSize + sizeof(std::array<Vector128, 128>)); // Extract endpoint
    char* mappedOut = mappedData + inFileSize + sizeof(std::array<Vector128, 128>) + sizeof(int); // Extract output pointer

    int i = 0;

    for(i = counterIn.fetch_add(1); i < 128;)
    {
        for(int x = (i*128); x < (i*128)+128; ++x)
        {
            double d;
            std::from_chars(mappedData+endPoint+6*x, mappedData+endPoint+6*x+5, d);
            matrix[i].Add(d);
        }
    }

    if(i == 127)
    {
        munmap(mappedData, inFileSize);
        close(inFile);
    }

    std::array<double, 8128> data;

    CorrelationCoefficients128Threaded(matrix, data);

    int x = 0;

    for(x = counterOut.fetch_add(1); i < 8128;)
        WriteThreaded(mappedOut, data[i], i);

    if(x == 8127)
    {
        msync(mappedOut, 164383, MS_SYNC);
        munmap(mappedOut, 164383);
        close(outFile);
    }
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
    int file = open(argv[1], O_RDONLY);

    off_t size = lseek(file, 0, SEEK_END);

    char* mappedData = static_cast<char*>(mmap(nullptr, size, PROT_READ, MAP_PRIVATE, file, 0));

    int dimension = 0;

    off_t endPoint = 3;

    if(mappedData[3] != '\n')
        endPoint++;

    for(off_t i = 0; i < endPoint; ++i)
        dimension = dimension*10 + (mappedData[i] - '0');

    endPoint++; //jump over newline

    std::array<Vector128, 128> matrix;

    if(dimension == 128) {
        if(argv[3] != "1")
        {
            outFile = open(argv[3], O_WRONLY, 0666);

            size_t outFileSize = 164383;
            ftruncate(outFile, outFileSize);

            char* mappedOut = static_cast<char*>(mmap(nullptr, outFileSize, PROT_WRITE, MAP_SHARED, outFile, 0));

            //create thread array
            std::array<pthread_t, 32> threads;

            inFile = file;
            inFileSize = size;

            void* args = new char[inFileSize + sizeof(matrix) + sizeof(int) + sizeof(mappedOut)];
            std::memcpy(args, mappedData, inFileSize);
            std::memcpy(static_cast<char*>(args) + inFileSize, &matrix, sizeof(matrix));
            std::memcpy(static_cast<char*>(args) + inFileSize + sizeof(matrix), &endPoint, sizeof(int));
            std::memcpy(static_cast<char*>(args) + inFileSize + sizeof(matrix) + sizeof(int), &mappedOut, sizeof(mappedOut));

            int nrThreads = std::from_chars(argv[3]);

            for(int i = 0; i < nrThreads; ++i)
                pthread_create(&threads[i], nullptr, Thread128, args)
        }

        for(int i = 0; i < dimension; ++i)
            for(int x = (i*dimension); x < (i*dimension)+dimension; ++x)
            {
                double d;
                std::from_chars(mappedData+endPoint+6*x, mappedData+endPoint+6*x+5, d);
                matrix[i].Add(d);
            }

        munmap(mappedData, size);
        close(file);

        std::array<double, 8128> data;

        CorrelationCoefficients128(matrix, data);

        //Write128(argv[2], data);
        Write128(argv[2], data);

        return 0;
    }

    if(dimension == 256)
    {
        std::array<Vector256, 256> matrix;

        for(int i = 0; i < dimension; ++i)
            for(int x = (i*dimension); x < (i*dimension)+dimension; ++x)
            {
                double d;
                std::from_chars(mappedData+endPoint+6*x, mappedData+endPoint+6*x+5, d);
                matrix[i].Add(d);
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
            for(int x = (i*dimension); x < (i*dimension)+dimension; ++x)
            {
                double d;
                std::from_chars(mappedData+endPoint+6*x, mappedData+endPoint+6*x+5, d);
                matrix[i].Add(d);
            }

        munmap(mappedData, size);
        close(file);

        std::array<double, 130816> data;

        CorrelationCoefficients512(matrix, data);

        Write512(argv[2], data);

        return 0;
    }

    Vector1024* matrix = new Vector1024[1024];

    for(int i = 0; i < 1024; ++i)
        for(int x = (i*dimension); x < (i*dimension)+dimension; ++x)
        {
            double d;
            std::from_chars(mappedData+endPoint+6*x, mappedData+endPoint+6*x+5, d);
            matrix[i].Add(d);
        }

    std::array<double, 523776> data;

    CorrelationCoefficients1024(matrix, data);

    Write1024(argv[2], data);

    return 0;
}
