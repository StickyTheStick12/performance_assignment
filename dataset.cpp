#include "dataset.h"

void Read(const std::string &filename, std::array<Vector, 1024> &matrix)
{
    std::ifstream file;
    file.open(filename);

    unsigned dimension;
    file >> dimension;

    file.seekg(1, std::ios::cur);

    for(unsigned i = 0; i < dimension; ++i)
    {
        double temp;
        file >> temp;
        matrix[i].Add(temp);
    }
}






void Write(const std::string& filename, std::array<std::array<double, 1024>, 1024>& matrix)
{
    std::ofstream f {};

    f.open(filename);

}

