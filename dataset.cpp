/*
Author: David Holmqvist <daae19@student.bth.se>
*/

#include "dataset.hpp"
#include "vector.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <ctype.h>

void Dataset::Read(const std::string& filename, )
{

    //save into a 2d array instead of vector
    std::ifstream file;
    file.open(filename);

    unsigned dimension;
    file >> dimension;
    Vector result[dimension]; //may need to use 1024

    file.seekg(1, std::ios::cur); //skip /n

    for(unsigned i = 0; i < dimension; ++i)
    {
        Vector newVec[dimension];
        //double temp[1024]; //change to newVec
        for(unsigned x = 0; x < dimension; ++x)
            file >> newVec[x];

        result[i] = newVec;
    }
}

void write(std::vector<double> data, std::string filename)
{
    std::ofstream f {};

    f.open(filename);

    //try catch?
    //asyncio if we are writing at the same time? as we generate data, for 1 thread?
    if (!f) {
        std::cerr << "Failed to write data to file " << filename << std::endl;
        return;
    }

    for (auto i { 0 }; i < data.size(); i++) {
        f << std::setprecision(std::numeric_limits<double>::digits10 + 1) << data[i] << std::endl;
    }
}

};
