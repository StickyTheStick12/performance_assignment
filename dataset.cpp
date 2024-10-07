#include "dataset.h"


double CharArrToDouble(const char *arr)
{
    int firstDigit = arr[2] - '0';
    int SecondDigit = arr[3] - '0';
    int thirdDigit = arr[4] - '0';

    return firstDigit * 0.1 + SecondDigit*0.01 + thirdDigit*0.001;
}


void Read(const std::string &filename, std::array<Vector, 512> &matrix)
{
    int file = open(filename.c_str(), O_RDONLY);

    off_t size = lseek(file, 0, SEEK_END);

    lseek(file, 0, SEEK_SET);

    char* mappedData = static_cast<char*>(mmap(nullptr, size, PROT_READ, MAP_PRIVATE, file, 0));

    int dimension = 0;

    off_t endPoint = 3;

    if(mappedData[3] != '\n')
        endPoint++;

    for(off_t i = 0; i < endPoint; ++i)
        dimension = dimension*10 + (mappedData[i] - '0');

    endPoint++; //jump over newline

    for(int i = 0; i < dimension; ++i)
        for(int x = (i*dimension); x < (i*dimension)+dimension; ++x)
            matrix[i].Add(CharArrToDouble(mappedData+endPoint+6*x));
}


void Write(const std::string& filename, std::array<double, 130816>& data, int endPoint)
{
    //TODO: fix, roughly 5 percent on callgrind spent here

    std::ofstream file;

    file.open(filename);

    for(unsigned i = 0; i < endPoint; ++i) {
        file << std::setprecision(std::numeric_limits<double>::digits10 + 1) << data[i] << std::endl;
    }
}

