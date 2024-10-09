#include "vector.h"

void Vector128::Add(const double num)
{
    data[size++] = num;
    sum += num;
}

double Vector128::Mean() const
{
    return sum / size;
}

void Vector128::operator-(const double sub)
{
    __m256d vec_scalar = _mm256_set1_pd(sub);

    for(unsigned i = 0; i < size; i += 4)
    {
        __m256d vec_data = _mm256_load_pd(&data[i]);
        __m256d vec_result = _mm256_sub_pd(vec_data, vec_scalar);
        _mm256_store_pd(&data[i], vec_result);
    }
}

void Vector128::operator/(double divisor)
{
    __m256d vec_scalar = _mm256_set1_pd(divisor);

    for(unsigned i = 0; i < size; i += 4)
    {
        __m256d vec_data = _mm256_load_pd(&data[i]);
        __m256d vec_result = _mm256_div_pd(vec_data, vec_scalar);
        _mm256_store_pd(&data[i], vec_result);
    }
}

double Vector128::Magnitude() const
{
    double result = 0.0;

    for (unsigned i = 0; i < size; i += 4) {
        __m256d vec_data = _mm256_load_pd(&data[i]);
        __m256d product = _mm256_mul_pd(vec_data, vec_data);

        alignas(32) double temp[4];
        _mm256_store_pd(temp, product);

        result += temp[0];
        result += temp[1];
        result += temp[2];
        result += temp[3];
    }

    return sqrt(result);
}

double Vector128::DotProduct(const Vector128 &other) const
{
    double result;

    for(unsigned i = 0; i < size; i += 4)
    {
        __m256d vecData = _mm256_load_pd(&data[i]);
        __m256d otherVecData = _mm256_load_pd(&other.data[i]);

        __m256d product = _mm256_mul_pd(vecData, otherVecData);

        alignas(32) double temp[4];
        _mm256_store_pd(temp, product);

        result += temp[0];
        result += temp[1];
        result += temp[2];
        result += temp[3];
    }

    return result;
}

void Vector256::Add(const double num)
{
    data[size++] = num;
    sum += num;
}

double Vector256::Mean() const
{
    return sum / size;
}

void Vector256::operator-(const double sub)
{
    __m256d vec_scalar = _mm256_set1_pd(sub);

    for(unsigned i = 0; i < size; i += 4)
    {
        __m256d vec_data = _mm256_load_pd(&data[i]); // Ensure `array` is aligned to 32 bytes
        __m256d vec_result = _mm256_sub_pd(vec_data, vec_scalar);
        _mm256_store_pd(&data[i], vec_result);
    }
}

void Vector256::operator/(double divisor)
{
    __m256d vec_scalar = _mm256_set1_pd(divisor);

    for(unsigned i = 0; i < size; i += 4)
    {
        __m256d vec_data = _mm256_load_pd(&data[i]);
        __m256d vec_result = _mm256_div_pd(vec_data, vec_scalar);
        _mm256_store_pd(&data[i], vec_result);
    }
}

double Vector256::Magnitude() const
{
    double result = 0.0;

    for (unsigned i = 0; i < size; i += 4) {
        __m256d vec_data = _mm256_load_pd(&data[i]);
        __m256d product = _mm256_mul_pd(vec_data, vec_data);

        alignas(32) double temp[4];
        _mm256_store_pd(temp, product);

        result += temp[0];
        result += temp[1];
        result += temp[2];
        result += temp[3];
    }

    return sqrt(result);
}

double Vector256::DotProduct(const Vector256 &other) const
{
    double result;

    for(unsigned i = 0; i < size; i += 4)
    {
        __m256d vecData = _mm256_load_pd(&data[i]);
        __m256d otherVecData = _mm256_load_pd(&other.data[i]);

        __m256d product = _mm256_mul_pd(vecData, otherVecData);

        alignas(32) double temp[4];
        _mm256_store_pd(temp, product);

        result += temp[0];
        result += temp[1];
        result += temp[2];
        result += temp[3];
    }

    return result;
}

void Vector512::Add(const double num)
{
    data[size++] = num;
    sum += num;
}

double Vector512::Mean() const
{
    return sum / size;
}

void Vector512::operator-(const double sub)
{
    __m256d vec_scalar = _mm256_set1_pd(sub);

    for(unsigned i = 0; i < size; i += 4)
    {
        __m256d vec_data = _mm256_load_pd(&data[i]); // Ensure `array` is aligned to 32 bytes
        __m256d vec_result = _mm256_sub_pd(vec_data, vec_scalar);
        _mm256_store_pd(&data[i], vec_result);
    }
}

void Vector512::operator/(double divisor)
{
    __m256d vec_scalar = _mm256_set1_pd(divisor);

    for(unsigned i = 0; i < size; i += 4)
    {
        __m256d vec_data = _mm256_load_pd(&data[i]);
        __m256d vec_result = _mm256_div_pd(vec_data, vec_scalar);
        _mm256_store_pd(&data[i], vec_result);
    }
}

double Vector512::Magnitude() const
{
    double result = 0.0;

    for (unsigned i = 0; i < size; i += 4) {
        __m256d vec_data = _mm256_load_pd(&data[i]);
        __m256d product = _mm256_mul_pd(vec_data, vec_data);

        alignas(32) double temp[4];
        _mm256_store_pd(temp, product);

        result += temp[0];
        result += temp[1];
        result += temp[2];
        result += temp[3];
    }

    return sqrt(result);
}

double Vector512::DotProduct(const Vector512 &other) const
{
    double result;

    for(unsigned i = 0; i < size; i += 4)
    {
        __m256d vecData = _mm256_load_pd(&data[i]);
        __m256d otherVecData = _mm256_load_pd(&other.data[i]);

        __m256d product = _mm256_mul_pd(vecData, otherVecData);

        alignas(32) double temp[4];
        _mm256_store_pd(temp, product);

        result += temp[0];
        result += temp[1];
        result += temp[2];
        result += temp[3];
    }

    return result;
}

void Vector1024::Add(const double num)
{
    data[size++] = num;
    sum += num;
}

double Vector1024::Mean() const
{
    return sum / size;
}

void Vector1024::operator-(const double sub)
{
    __m256d vec_scalar = _mm256_set1_pd(sub);

    for(unsigned i = 0; i < size; i += 4)
    {
        __m256d vec_data = _mm256_load_pd(&data[i]); // Ensure `array` is aligned to 32 bytes
        __m256d vec_result = _mm256_sub_pd(vec_data, vec_scalar);
        _mm256_store_pd(&data[i], vec_result);
    }
}

void Vector1024::operator/(double divisor)
{
    __m256d vec_scalar = _mm256_set1_pd(divisor);

    for(unsigned i = 0; i < size; i += 4)
    {
        __m256d vec_data = _mm256_load_pd(&data[i]);
        __m256d vec_result = _mm256_div_pd(vec_data, vec_scalar);
        _mm256_store_pd(&data[i], vec_result);
    }
}

double Vector1024::Magnitude() const
{
    double result = 0.0;

    for (unsigned i = 0; i < size; i += 4) {
        __m256d vec_data = _mm256_load_pd(&data[i]);
        __m256d product = _mm256_mul_pd(vec_data, vec_data);

        alignas(32) double temp[4];
        _mm256_store_pd(temp, product);

        result += temp[0];
        result += temp[1];
        result += temp[2];
        result += temp[3];
    }

    return sqrt(result);
}

double Vector1024::DotProduct(const Vector1024 &other) const
{
    double result;

    for(unsigned i = 0; i < size; i += 4)
    {
        __m256d vecData = _mm256_load_pd(&data[i]);
        __m256d otherVecData = _mm256_load_pd(&other.data[i]);

        __m256d product = _mm256_mul_pd(vecData, otherVecData);

        alignas(32) double temp[4];
        _mm256_store_pd(temp, product);

        result += temp[0];
        result += temp[1];
        result += temp[2];
        result += temp[3];
    }

    return result;
}
