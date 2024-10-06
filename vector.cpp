#include "vector.h"

void Vector::Add(const double num)
{
    data[size++] = num;
    sum += num;
}

double Vector::Mean() const
{
    return sum / size;
}

void Vector::operator-(const double sub)
{
    __m256d vec_scalar = _mm256_set1_pd(sub);

    for(unsigned i = 0; i < size; i += 4)
    {
        __m256d vec_data = _mm256_load_pd(&data[i]); // Ensure `array` is aligned to 32 bytes
        __m256d vec_result = _mm256_sub_pd(vec_data, vec_scalar);
        _mm256_store_pd(&data[i], vec_result);
    }

    /*
    __m512d vec_scalar = _mm512_set1_pd(sub);

    for(unsigned i = 0; i < size; i += 8)
    {
        __m512d vec_data = _mm512_load_pd(&data[i]); // Ensure `array` is aligned to 32 bytes
        __m256d vec_result = _mm512_sub_pd(vec_data, vec_scalar);
        _mm512_store_pd(&data[i], vec_result);
    }
     */
}

double Vector::Magnitude() const
{
    __m256d sum = _mm256_setzero_pd();

    for(unsigned i = 0; i < size; i += 4)
    {
        __m256d vec_data = _mm256_load_pd(&data[i]);

        __m256d product = _mm256_mul_pd(vec_data, vec_data);

        sum = _mm256_add_pd(sum, product);
    }

    alignas(32) double result[4];
    _mm256_store_pd(result, sum);

    return result[0] + result[1] + result[2] + result[3];

    /*
    __m512d sum = _mm512_setzero_pd();

    for(unsigned i = 0; i < size; i += 8)
    {
        __m512d vec_data = _mm512_load_pd(&data[i]);

        __m512d product = _mm512_mul_pd(vec_data, vec_data);

        sum = _mm512_add_pd(sum, product);
    }

    alignas(32) double result[8];
    _mm512_store_pd(result, sum); // Store the AVX register into a regular array

    // Final reduction (sum the elements of result array)
    return result[0] + result[1] + result[2] + result[3] + result[4] + result[5] + result[6] + result[7];
     */
}




