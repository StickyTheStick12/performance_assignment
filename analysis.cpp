/*
Author: David Holmqvist <daae19@student.bth.se>
*/

#include "analysis.hpp"
#include <algorithm>
#include <list>
#include <vector>

namespace Analysis {

std::vector<double> correlation_coefficients(std::vector<Vector> datasets)
{
    //give vector a size from the beginning, no need to resize, will have n/2 size?
    // dont use a vector, use array to store on the stack? faster?
    //dont return a copy of the vector, better to return a pointer

    std::vector<double> result {};

    //if not asyncio, reverse loop? compare zero is faster so sample ? dataset.size() - 1 < 0
    // sample2 < 0; then break? or atleast sample2 < 0 and the other -1. We reverse when we write, which again is a compare zero
    for (auto sample1 { 0 }; sample1 < datasets.size() - 1; sample1++) {
        for (auto sample2 { sample1 + 1 }; sample2 < datasets.size(); sample2++) {
            auto corr { pearson(datasets[sample1], datasets[sample2]) };
            result.push_back(corr);
        }
    }

    return result;
}

double pearson(Vector vec1, Vector vec2)
{
    //When we add a new element, we can calculate sum, during runtime to skip looping the vector here, the same math should
    // be done just that we are doing it when we have a copy of it instead of writing getting the num, write to array, later fetch num from array and sum.
    auto x_mean { vec1.mean() };
    auto y_mean { vec2.mean() };

    auto x_mm { vec1 - x_mean };
    auto y_mm { vec2 - y_mean };

    auto x_mag { x_mm.magnitude() };
    auto y_mag { y_mm.magnitude() };

    auto x_mm_over_x_mag { x_mm / x_mag };
    auto y_mm_over_y_mag { y_mm / y_mag };

    auto r { x_mm_over_x_mag.dot(y_mm_over_y_mag) };

    return std::max(std::min(r, 1.0), -1.0);
}
};
