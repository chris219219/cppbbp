#include "../inc/pifunc.hpp"

#include <float.h>
#include <string>
#include <thread>

using std::string;
using std::thread;

char tohex(uint8_t a)
{
    if (a > 9) return a + 55;
    return a + 48;
}

long modpow(long b, long e, long m)
{
    long res = 1;
    while (e)
    {
        if (e & 1)
            res = (res * b) % m;
        e >>= 1;
        b = b * b % m;
    }
    return res;
}

double fp(double a)
{
    return a - ((long)(a + 10000) - 10000);
}

double sp(long n, long j)
{
    double sum = 0;
    long denomtop = j;
    double denom = j;

    while (n >= 0)
    {
        sum = fp(sum + ((double)modpow(16, n, denomtop) / denom));
        denomtop += 8;
        denom += 8.0;
        --n;
    }

    double num = 0.0625;
    num /= denom;

    while (num > DBL_EPSILON)
    {
        sum += num;
        num *= 0.0078125;
    }

    return sum;
}

uint8_t pidec(long n)
{
    double sum =
          4.0 * sp(n, 1)
        - 2.0 * sp(n, 4)
        - sp(n, 5)
        - sp(n, 6);
    return (uint8_t)(16.0 * fp(sum));
}

char pihex(long n)
{
    return tohex(pidec(n));
}

void pithread(char* char_arr, long start, long length, int threads, int id)
{
    for (long i = id; i < length; i += threads)
    {
        char_arr[i] = pihex(start + i);
    }
}

string pigroup(long start, long length, int threads)
{
    char* char_arr = new char[length];
    thread* thread_arr = new thread[threads];

    for (int i = 0; i < threads; ++i)
    {
        thread_arr[i] = std::thread(pithread, char_arr, start, length, threads, i);
    }

    for (int i = 0; i < threads; ++i)
        thread_arr[i].join();

    delete[] thread_arr;
    string ret(char_arr);
    delete[] char_arr;
    return ret;
}