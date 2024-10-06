#include <array>
#include "vector.h"
#include "dataset.h"

//TODO
// 1: check if we can use 512 bit instructions for vector, then remove 256 bit (check alignment (must be 64))
// 2: increase stack in main
// 3: fix vector division
// 4: fix vector dot product
// 5: fix write to file from dataset
// 6: test run
// 7: maybe align all arrays?
// 8: general code cleanup
// 9: fix multithreading
// 10: general code cleanup




int main(int argc, char const* argv[]) {
    //remove this during runtime
    //if (argc != 3) {
    //    std::cerr << "Usage: " << argv[0] << " [dataset] [outfile]" << std::endl;
    //    std::exit(1);
    //}


    // increase stack

    std::array<Vector, 1024> matrix;

    Read(argv[1], matrix);




    //auto corrs { Analysis::correlation_coefficients(datasets) };
    //Dataset::write(corrs, argv[2]);

    return 0;
}
