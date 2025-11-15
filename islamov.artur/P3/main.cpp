#include <iostream>
#include <fstream>
#include <cstdlib>
#include <climits>
#include <cstring>
namespace Islamov{
    static int CNT_COL_NSM(const int* arr, int rows, int cols);
    static int CNT_NZR_DIG(const int* arr, int rows, int cols);
    static bool IntArg(const char* s, int &out);
}
int main(int argc, char** argv)
{

    if (argc != 4)
    {
        std::cerr << "Error: Incorrect arguments\n";
        return 1;
    }
    int m;
    if (!Islamov::IntArg(argv[1], m))
    {
        std::cerr << "Error: first parameter is not a number\n";
        return 1;
    }
    if (m != 1 && m != 2)
    {
        std::cerr << "Error: first parameter is out of range\n";
        return 1;
    }
    const char* inputName  = argv[2];
    const char* outputName = argv[3];
    std::ifstream fin(inputName, std::ios::binary);
    if (!fin)
    {
        std::cerr << "Error: cannot open input file: " << inputName << "\n";
        return 2;
    }
    char byte;
    while (fin.get(byte))
    {
        if (byte == '\0')
        {
            std::cerr << "Error: null byte in input\n";
            return 2;
        }
    }
    fin.clear();
    fin.seekg(0);
    long long r64 = 0, c64 = 0;
    if (!(fin >> r64 >> c64))
    {
        std::cerr << "Error: input file content is not a valid matrix\n";
        return 2;
    }
    if (r64 < 0 || c64 < 0)
    {
        std::cerr << "Error: input file content is not a valid matrix\n";
        return 2;
    }
    long long tLL = r64*c64;
    if (r64 != 0 && tLL/r64 != c64)
    {
        std::cerr << "Error: input file content is not a valid matrix\n";
        return 2;
    }
    int rows = static_cast<int>(r64);
    int cols = static_cast<int>(c64);
    long long t = tLL;
    int res1 = 0, res2 = 0;
    if (m == 1)
    {
        if (t > 10000)
        {
            std::cerr << "Error: matrix too large\n";
            return 1;
        }
        int stackArr[10000];
        for (long long i = 0; i < t; ++i)
        {
            long long val;
            if (!(fin >> val))
            {
                std::cerr << "Error: input file content is not a valid matrix\n";
                return 2;
            }
            stackArr[i] = static_cast<int>(val);
        }
        long long dummy;
        if (fin >> dummy)
        {
            std::cerr << "Error: input file content is not a valid matrix\n";
            return 2;
        }
        res1 = Islamov::CNT_COL_NSM(stackArr, rows, cols);
        res2 = Islamov::CNT_NZR_DIG(stackArr, rows, cols);
        std::ofstream fout(outputName, std::ios::binary);
        if (!fout)
        {
            std::cerr << "Error: cannot open output file: " << outputName << "\n";
            return 2;
        }
        fout << res1 << "\n" << res2 << "\n";
        return 0;
    }else{
        int* dynArr=new int[static_cast<size_t>(t)];
        for (long long i = 0; i < t; ++i)
        {
            long long val;
            if (!(fin >> val))
            {
                delete[] dynArr;
                std::cerr << "Error: input file content is not a valid matrix\n";
                return 2;
            }
            dynArr[i] = static_cast<int>(val);
        }
        long long dummy;
        if (fin >> dummy)
        {
            delete[] dynArr;
            std::cerr << "Error: input file content is not a valid matrix\n";
            return 2;
        }
        fin.close();
        res1 = Islamov::CNT_COL_NSM(dynArr, rows, cols);
        res2 = Islamov::CNT_NZR_DIG(dynArr, rows, cols);
        std::ofstream out(outputName, std::ios::binary);
        if (!out)
        {
            delete[] dynArr;
            std::cerr << "Error: cannot open output file: " << outputName << "\n";
            return 2;
        }
        out << res1 << "\n" << res2 << "\n";
        delete[] dynArr;
        return 0;
    }
}
static int Islamov::CNT_COL_NSM(const int* arr, int rows, int cols)
{
    int count = 0;
    for (int j = 0; j < cols; ++j)
    {
        bool equal = false;
        for (int i = 0; i+1 < rows; ++i)
        {
            if (arr[i*cols+j] == arr[(i+1)*cols+j])
            {
                equal = true;
                break;
            }
        }
        if (!equal)
        {
            ++count;
        }
    }
    return count;
}
static int Islamov::CNT_NZR_DIG(const int* arr, int rows, int cols)
{
    if (rows<=0 || cols<=0) return 0;
    int count=0;
    for (int d = -(rows-1); d <= (cols-1); ++d)
    {
        int start= (d<0) ? -d : 0;
        int end=std::min(rows-1, cols-1-d);
        bool Zero=false;
        for (int i = start; i <= end; ++i)
        {
            int j = i+d;
            if (arr[i*cols+j] == 0)
            {
                Zero=true;
                break;
            }
        }
        if (!Zero)
        {
            ++count;
        }
    }
    return count;
}
static bool Islamov::IntArg(const char* s, int &out)
{
    char* endptr = nullptr;
    long val = std::strtol(s, &endptr, 10);
    if (endptr == s || *endptr != '\0')
    {
        return false;
    }
    if (val<INT_MIN || val>INT_MAX)
    {
        return false;
    }
    out = static_cast<int>(val);
    return true;
}
