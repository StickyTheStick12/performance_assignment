#include "analysis.h"
#include "vector.h"
#include "dataset.h"
#include <pthread.h>
#include <charconv>
#include <condition_variable>

int inFile;
int outFile;
off_t inFileSize;

std::atomic<int> counterIn(0);
std::atomic<int> counterOut(0);
std::atomic<int> activeThreads(0);
char* mappedDataThread;
char* mappedOutThread;


struct ThreadData128 {
    alignas(32) std::array<Vector128, 128> matrix;
    std::array<double, 8128> data;
};

struct ThreadData256 {
    alignas(32) std::array<Vector256, 256> matrix;
    std::array<double, 32640> data;
};

struct ThreadData512 {
    alignas(32) std::array<Vector512, 512> matrix;
    std::array<double, 130816> data;
};

struct ThreadData1024 {
    alignas(32) Vector1024* matrix = new Vector1024[1024];
    std::array<double, 523776> data;
};

void* Thread128(ThreadData128* threadData)
{
    int i;

    ++activeThreads;

    while ((i = counterIn.fetch_add(1)) < 128)
    {
        char* mappedData = mappedDataThread+i*768;

        madvise(mappedData, 767*4, MADV_WILLNEED);

        for(int j = 0; j < 128; ++j) {
            double d;
            char* endPtr = mappedData+5;
            std::from_chars(mappedData, endPtr, d);

            threadData->matrix[i].Add(d);
            mappedData = endPtr + 1;
        }
    }

    if(--activeThreads == 0)
    {
        munmap(mappedDataThread, inFileSize);
        close(inFile);
    }

    CorrelationCoefficients128Threaded(threadData->matrix, threadData->data);

    int x = 0;

    ++activeThreads;

    while((x = counterOut.fetch_add(1)) < 8128)
        WriteThreaded(mappedOutThread, threadData->data[x], x);

    --activeThreads;

    if(x == 8128)
    {
        msync(mappedOutThread, 164383, MS_SYNC);
        munmap(mappedOutThread, 164383);
        close(outFile);
    }

    return nullptr;
}

void* Thread256(ThreadData256* threadData)
{
    int i;

    ++activeThreads;

    while ((i = counterIn.fetch_add(1)) < 256)
    {
        char* mappedData = mappedDataThread+i*1536;

        madvise(mappedData, 1535*4, MADV_WILLNEED);

        for(int j = 0; j < 256; ++j) {
            double d;
            char* endPtr = mappedData+5;
            std::from_chars(mappedData, endPtr, d);
            threadData->matrix[i].Add(d);

            mappedData = endPtr + 1;
        }
    }

    if(--activeThreads == 0)
    {
        munmap(mappedDataThread, inFileSize);
        close(inFile);
    }

    CorrelationCoefficients256Threaded(threadData->matrix, threadData->data);

    int x = 0;

    while((x = counterOut.fetch_add(1)) < 32640)
        WriteThreaded(mappedOutThread, threadData->data[x], x);

    if(x == 32640)
    {
        msync(mappedOutThread, 666539, MS_SYNC);
        munmap(mappedOutThread, 666539);
        close(outFile);
    }

    return nullptr;
}

void* Thread512(ThreadData512* threadData)
{
    int i = 0;

    ++activeThreads;

    while ((i = counterIn.fetch_add(1)) < 512)
    {
        char* mappedData = mappedDataThread+i*3072;

        //madvise(mappedData, 3071*4, MADV_WILLNEED);

        for(int j = 0; j < 512; ++j) {
            double d;
            char* endPtr = mappedData+5;
            std::from_chars(mappedData, endPtr, d);
            threadData->matrix[i].Add(d);

            mappedData = endPtr + 1;
        }
    }

    if(i == 512)
    {
        munmap(mappedDataThread, inFileSize);
        close(inFile);
    }

    CorrelationCoefficients512Threaded(threadData->matrix, threadData->data);

    int x = 0;

    while((x = counterOut.fetch_add(1)) < 130816)
        WriteThreaded(mappedOutThread, threadData->data[x], x);

    if(x == 130816)
    {
        msync(mappedOutThread, 2689713, MS_SYNC);
        munmap(mappedOutThread, 2689713);
        close(outFile);
    }

    return nullptr;
}

void* Thread1024(ThreadData1024* threadData)
{
    int i;

    ++activeThreads;
    
    while ((i = counterIn.fetch_add(1)) < 1024)
    {
        char* mappedData = mappedDataThread+i*6144;

        //madvise(mappedData, 6143*4, MADV_WILLNEED);

        for(int j = 0; j < 1024; ++j) {
            double d;
            char* endPtr = mappedData+5;
            std::from_chars(mappedData, endPtr, d);

            threadData->matrix[i].Add(d);
            mappedData = endPtr + 1;
        }
    }

    if(--activeThreads == 0)
    {
        munmap(mappedDataThread, inFileSize);
        close(inFile);
    }

    return nullptr;
    
    CorrelationCoefficients1024Threaded(threadData->matrix, threadData->data);

    int x = 0;

    while((x = counterOut.fetch_add(1)) < 523776)
        WriteThreaded(mappedOutThread, threadData->data[x], x);

    if(x == 523776)
    {
        msync(mappedOutThread, 10822467, MS_SYNC);
        munmap(mappedOutThread, 10822467);
        close(outFile);
    }

    return nullptr;
}

int main(int argc, char const* argv[])
{
    int file = open(argv[1], O_RDONLY);

    off_t size = lseek(file, 0, SEEK_END);

    char* dataPtr = static_cast<char*>(mmap(nullptr, size, PROT_READ, MAP_PRIVATE, file, 0));

    char* mappedData = dataPtr;

    madvise(mappedData, size, MADV_SEQUENTIAL);

    int dimension = 0;

    while(*mappedData != '\n') {
        dimension = dimension * 10 + (*mappedData - '0');
        ++mappedData;
    }

    ++mappedData;

    if(dimension == 128) {
        if(argv[3] != "1")
        {
            outFile = open(argv[2], O_RDWR | O_CREAT, 0666);

            size_t outFileSize = 164383;
            ftruncate(outFile, outFileSize);

            char* mappedOut = static_cast<char*>(mmap(nullptr, outFileSize, PROT_WRITE, MAP_SHARED, outFile, 0));

            //create thread array
            pthread_t threads[32];

            int nrThreads;
            if (argv[3][1] == '\0') {
                nrThreads = argv[3][0] - '0'; // Convert first character to integer
            } else {
                nrThreads = (argv[3][0] - '0') * 10 + (argv[3][1] - '0'); // Convert first two characters
            }

            mappedDataThread = mappedData;
            mappedOutThread = mappedOut;

            ThreadData128 threadData;

            for(int i = 0; i < nrThreads-1; ++i)
                pthread_create(&threads[i], nullptr, (void * (*)(void *))Thread128, &threadData);

            Thread128(&threadData);

            for(int i = 0; i < nrThreads-1; ++i)
                pthread_join(threads[i], nullptr);

            return 0;
        }

        std::array<Vector128, 128> matrix;

        for(int i = 0; i < dimension; ++i)
            for(int j = 0; j < dimension; ++j) {
                double d;
                char* endPtr = mappedData+5;
                std::from_chars(mappedData, endPtr, d);
                matrix[i].Add(d);

                mappedData = endPtr;
                ++mappedData;
            }

        munmap(mappedData, size);
        close(file);

        std::array<double, 8128> data;

        CorrelationCoefficients128(matrix, data);

        Write128(argv[2], data);

        return 0;
    }

    if(dimension == 256)
    {
        if(argv[3] != "1")
        {
            outFile = open(argv[2], O_RDWR | O_CREAT, 0666);

            size_t outFileSize = 666539;
            ftruncate(outFile, outFileSize);

            char* mappedOut = static_cast<char*>(mmap(nullptr, outFileSize, PROT_WRITE, MAP_SHARED, outFile, 0));

            //create thread array
            pthread_t threads[32];

            int nrThreads;
            if (argv[3][1] == '\0') {
                nrThreads = argv[3][0] - '0'; // Convert first character to integer
            } else {
                nrThreads = (argv[3][0] - '0') * 10 + (argv[3][1] - '0'); // Convert first two characters
            }

            mappedDataThread = mappedData;
            mappedOutThread = mappedOut;

            ThreadData256 threadData;

            for(int i = 0; i < nrThreads-1; ++i)
                pthread_create(&threads[i], nullptr, (void * (*)(void *))Thread256, &threadData);

            Thread256(&threadData);

            for(int i = 0; i < nrThreads-1; ++i)
                pthread_join(threads[i], nullptr);

            return 0;
        }

        std::array<Vector256, 256> matrix;

        for(int i = 0; i < dimension; ++i)
            for(int j = 0; j < dimension; ++j) {
                double d;
                char* endPtr = mappedData+5;
                std::from_chars(mappedData, endPtr, d);
                matrix[i].Add(d);

                mappedData = endPtr;
                ++mappedData;
            }

        munmap(mappedData, size);
        close(file);

        std::array<double, 32640> data;

        CorrelationCoefficients256(matrix, data);

        Write256(argv[2], data);

        return 0;
    }

    if(dimension == 512)
    {
        if(argv[3] != "1")
        {
            outFile = open(argv[2], O_RDWR | O_CREAT, 0666);

            size_t outFileSize = 2689713;
            ftruncate(outFile, outFileSize);

            char* mappedOut = static_cast<char*>(mmap(nullptr, outFileSize, PROT_WRITE, MAP_SHARED, outFile, 0));

            pthread_t threads[32];

            int nrThreads;
            if (argv[3][1] == '\0') {
                nrThreads = argv[3][0] - '0'; // Convert first character to integer
            } else {
                nrThreads = (argv[3][0] - '0') * 10 + (argv[3][1] - '0'); // Convert first two characters
            }

            mappedDataThread = mappedData;
            mappedOutThread = mappedOut;

            ThreadData512 threadData;

            for(int i = 0; i < nrThreads-1; ++i)
                pthread_create(&threads[i], nullptr, (void * (*)(void *))Thread512, &threadData);

            Thread512(&threadData);

            for(int i = 0; i < nrThreads-1; ++i)
                pthread_join(threads[i], nullptr);

            return 0;
        }


        std::array<Vector512, 512> matrix;

        for(int i = 0; i < dimension; ++i)
            for(int j = 0; j < dimension; ++j) {
                double d;
                char* endPtr = mappedData+5;
                std::from_chars(mappedData, endPtr, d);
                matrix[i].Add(d);

                mappedData = endPtr;
                ++mappedData;
            }

        munmap(mappedData, size);
        close(file);

        std::array<double, 130816> data;

        CorrelationCoefficients512(matrix, data);

        Write512(argv[2], data);

        return 0;
    }

    if(argv[3] != "1")
    {
        outFile = open(argv[2], O_RDWR | O_CREAT, 0666);

        size_t outFileSize = 10822467;
        ftruncate(outFile, outFileSize);

        char* mappedOut = static_cast<char*>(mmap(nullptr, outFileSize, PROT_WRITE, MAP_SHARED, outFile, 0));

        madvise(mappedOut, outFileSize, MADV_RANDOM);

        pthread_t threads[32];

        int nrThreads;
        if (argv[3][1] == '\0') {
            nrThreads = argv[3][0] - '0'; // Convert first character to integer
        } else {
            nrThreads = (argv[3][0] - '0') * 10 + (argv[3][1] - '0'); // Convert first two characters
        }

        mappedDataThread = mappedData;
        mappedOutThread = mappedOut;

        ThreadData1024 threadData;

        for(int i = 0; i < nrThreads-1; ++i)
            pthread_create(&threads[i], nullptr, (void * (*)(void *))Thread1024, &threadData);

        Thread1024(&threadData);

        for(int i = 0; i < nrThreads-1; ++i)
            pthread_join(threads[i], nullptr);

        return 0;
    }

    alignas(32) Vector1024* matrix = new Vector1024[1024];

    for(int i = 0; i < dimension; ++i)
        for(int j = 0; j < dimension; ++j) {
            double d;
            char* endPtr = mappedData+5;
            std::from_chars(mappedData, endPtr, d);
            matrix[i].Add(d);

            mappedData = endPtr;
            ++mappedData;
        }

    std::array<double, 523776> data;

    CorrelationCoefficients1024(matrix, data);

    Write1024(argv[2], data);

    return 0;
}
