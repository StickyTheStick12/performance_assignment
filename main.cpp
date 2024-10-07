#include "analysis.h"
#include "vector.h"
#include "dataset.h"

//TODO
// 1: fix 1024 dataset
// 2: fix so we use dimension everywhere and not hardcoded
// 3: general code cleanup
// 4: fix multithreading
// 5: general code cleanup




int main(int argc, char const* argv[])
{
    std::array<Vector128, 512> matrix;

    Read("128.data", matrix);

    std::array<double, 130816> data;

    int index = Correlation_coefficients(matrix, data);

    Write("out.txt", data, index);

    return 0;
}
