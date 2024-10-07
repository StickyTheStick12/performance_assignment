#include "dataset.h"


double CharArrToDouble(const char *arr)
{
    int firstDigit = arr[2] - '0';
    int SecondDigit = arr[3] - '0';
    int thirdDigit = arr[4] - '0';

    return firstDigit * 0.1 + SecondDigit*0.01 + thirdDigit*0.001;
}


void Write128(const std::string& filename, std::array<double, 8128>& data)
{
    std::ofstream file(filename);

    // Set precision for the file output
    file << std::setprecision(std::numeric_limits<double>::digits10 + 1);

    std::ostringstream buffer;
    for (unsigned i = 0; i < 8128; ++i)
        buffer << data[i] << '\n'; // Append each double to the buffer

    // Write the entire buffer to the file
    file << buffer.str();

    file.close();
}

void Write256(const std::string& filename, std::array<double, 32640>& data)
{
    std::ofstream file(filename);

    // Set precision for the file output
    file << std::setprecision(std::numeric_limits<double>::digits10 + 1);

    std::ostringstream buffer;
    for (unsigned i = 0; i < 32640; ++i)
        buffer << data[i] << '\n'; // Append each double to the buffer

    // Write the entire buffer to the file
    file << buffer.str();

    file.close();
}

void Write512(const std::string& filename, std::array<double, 130816>& data)
{
    std::ofstream file(filename);

    // Set precision for the file output
    file << std::setprecision(std::numeric_limits<double>::digits10 + 1);

    std::ostringstream buffer;
    for (unsigned i = 0; i < 130816; ++i)
        buffer << data[i] << '\n'; // Append each double to the buffer

    // Write the entire buffer to the file
    file << buffer.str();

    file.close();
}

void Write1024(const std::string& filename, double* data)
{
    std::ofstream file(filename);

    // Set precision for the file output
    file << std::setprecision(std::numeric_limits<double>::digits10 + 1);

    std::ostringstream buffer;
    for (unsigned i = 0; i < 523776; ++i)
        buffer << data[i] << '\n'; // Append each double to the buffer

    // Write the entire buffer to the file
    file << buffer.str();

    file.close();
}
