#include "analysis.h"
#include "vector.h"
#include "dataset.h"

//TODO
// 1: fix 1024 dataset
// 2: fix multithreading
// 3: general code cleanup
// 4: can move the data read from file to another function and pass the data array to that so when that func leaves it gets deleted so we dont smash the stack

int main(int argc, char const* argv[])
{
    int file = open(argv[1], O_RDONLY);

    off_t size = lseek(file, 0, SEEK_END);

    lseek(file, 0, SEEK_SET); // TODO: IS this needed

    char* mappedData = static_cast<char*>(mmap(nullptr, size, PROT_READ, MAP_PRIVATE, file, 0));

    int dimension = 0;

    off_t endPoint = 3;

    if(mappedData[3] != '\n')
        endPoint++;

    for(off_t i = 0; i < endPoint; ++i)
        dimension = dimension*10 + (mappedData[i] - '0');

    endPoint++; //jump over newline

    if(dimension == 128) {
        std::array<Vector128, 128> matrix;

        for(int i = 0; i < dimension; ++i)
            for(int x = (i*dimension); x < (i*dimension)+dimension; ++x)
                matrix[i].Add(CharArrToDouble(mappedData+endPoint+6*x));

        std::array<double, 8128> data;

        CorrelationCoefficients128(matrix, data);

        Write128(argv[2], data);

        return 0;
    }
    else if(dimension == 256)
    {
        std::array<Vector256, 256> matrix;

        for(int i = 0; i < dimension; ++i)
            for(int x = (i*dimension); x < (i*dimension)+dimension; ++x)
                matrix[i].Add(CharArrToDouble(mappedData+endPoint+6*x));

        std::array<double, 32640> data;

        CorrelationCoefficients256(matrix, data);

        Write256(argv[2], data);

        return 0;

    }
    else if(dimension == 512)
    {
        std::array<Vector512, 512> matrix;

        for(int i = 0; i < dimension; ++i)
            for(int x = (i*dimension); x < (i*dimension)+dimension; ++x)
                matrix[i].Add(CharArrToDouble(mappedData+endPoint+6*x));

        std::array<double, 130816> data;

        CorrelationCoefficients512(matrix, data);

        Write512(argv[2], data);

        return 0;
    }
    else
    {
        std::array<Vector1024, 512> matrix;
        return 0;

    }
}
