#include "analysis.h"
#include "vector.h"
#include "dataset.h"
#include <atomic>
#include <thread>


//TODO
// 1: fix multithreading
// 2: general code cleanup
// 3: find improvements for report

int inFile;
int outFile;
off_t inFileSize;
off_t outFileSize; //remove later

std::atomic<int> counter(0);

void Thread128(char* mappedData, std::array<Vector128, 128>& matrix, int endPoint, const std::string& filename)
{
    int i = 0;

    for(i = counter.fetch_add(1); i < 128;)
    {
        for(int x = (i*128); x < (i*128)+128; ++x)
            matrix[i].Add(CharArrToDouble(mappedData+endPoint+6*x));
    }

    if(i == 127)
    {
        munmap(mappedData, inFileSize);
        close(inFile);
    }

    std::array<double, 8128> data;

    CorrelationCoefficients128Threaded(matrix, data);

    Write128(filename, data);
}










int main(int argc, char const* argv[])
{
    int file = open("128.data", O_RDONLY);

    off_t size = lseek(file, 0, SEEK_END);

    char* mappedData = static_cast<char*>(mmap(nullptr, size, PROT_READ, MAP_PRIVATE, file, 0));

    int dimension = 0;

    off_t endPoint = 3;

    if(mappedData[3] != '\n')
        endPoint++;

    for(off_t i = 0; i < endPoint; ++i)
        dimension = dimension*10 + (mappedData[i] - '0');

    endPoint++; //jump over newline

    if(dimension == 128) {
        if('1' != '1') //this wont work
        {
            outFile = open(argv[3], O_WRONLY, 0666);

            size_t outFileSize = 95000;
            ftruncate(outFile, outFileSize);

            char* outFileData = static_cast<char*>(mmap(nullptr, outFileSize, PROT_WRITE, MAP_SHARED, outFile, 0));

            //create threads
        }

        std::array<Vector128, 128> matrix;

        for(int i = 0; i < dimension; ++i)
            for(int x = (i*dimension); x < (i*dimension)+dimension; ++x)
                matrix[i].Add(CharArrToDouble(mappedData+endPoint+6*x));

        munmap(mappedData, size);
        close(file);

        std::array<double, 8128> data;

        CorrelationCoefficients128(matrix, data);

        //Write128(argv[2], data);
        Write128("out.txt", data);

        return 0;
    }

    if(dimension == 256)
    {
        std::array<Vector256, 256> matrix;

        for(int i = 0; i < dimension; ++i)
            for(int x = (i*dimension); x < (i*dimension)+dimension; ++x)
                matrix[i].Add(CharArrToDouble(mappedData+endPoint+6*x));

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
                matrix[i].Add(CharArrToDouble(mappedData+endPoint+6*x));

        munmap(mappedData, size);
        close(file);

        std::array<double, 130816> data;

        CorrelationCoefficients512(matrix, data);

        Write512(argv[2], data);

        return 0;
    }

    std::array<Vector1024, 512> matrix;

    for(int i = 0; i < 512; ++i)
        for(int x = (i*dimension); x < (i*dimension)+dimension; ++x)
            matrix[i].Add(CharArrToDouble(mappedData+endPoint+6*x));

    double* data = new double[523776];

    int index = CorrelationCoefficients1024(matrix, data);

    Vector1024 temp = matrix[511];

    for(int i = 512; i < 1024; ++i)
        for(int x = (i*dimension); x < (i*dimension)+dimension; ++x)
            matrix[i].Add(CharArrToDouble(mappedData+endPoint+6*x));

    data[index++] = Pearson1024(temp, matrix[0]);

    CorrelationCoefficients1024(matrix, data, index);

    Write1024(argv[2], data);

    return 0;
}
