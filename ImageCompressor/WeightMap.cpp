#include "pch.h"
#include "WeightMap.h"
using namespace std;

typedef unique_ptr<FILE, decltype(std::fclose)*> file_ptr;

void WeightMap::loadFile(const string &filepath)
{
    // 每次从文件中读取一个字符，统计各字符出现的次数
    FILE *fout;
    fopen_s(&fout, filepath.c_str(), "rb");
    file_ptr pfout(fout, fclose);
    if (!(pfout.get()))
    {
        string errmsg = move(string("fopen ") + filepath + " failed");
        perror(errmsg.c_str());
        exit(-1);
    }
    if (errno)
    {
        string errmsg = string("fopen ") + filepath + " failed";
        perror(errmsg.c_str());
        exit(-1);
    }
    unsigned char ch = fgetc(pfout.get());

    while (!feof(pfout.get()))
    {
        ++weights[ch].first;
        ch = fgetc(pfout.get());
    }
}

void WeightMap::setBytesCount(const std::vector<unsigned int>& counts)
{
    for (unsigned i = 0; i < counts.size() && i < weights.size(); ++i)
    {
        weights[i].first = counts[i];
    }
}


vector<unsigned int> WeightMap::getBytesCount() const
{
    vector<unsigned int> res(256);
    for (unsigned i = 0; i < weights.size(); ++i)
    {
        res[i] = weights[i].first;
    }
    return res;
}

void WeightMap::setCodes(const vector<std::string> &codes)
{
    for (unsigned i = 0; i < codes.size() && i < weights.size(); ++i)
    {
        weights[i].second = codes[i];
    }
}

std::vector<std::string> WeightMap::getCodes() const
{
    std::vector<std::string> res(256);
    for (unsigned int i = 0; i < 256; i++)
    {
        res[i] = weights[i].second;
    }
    return res;
}

void WeightMap::print()
{
    cout << "Byte" << left << setfill(' ') << setw(10) << " Weight" <<  "Code" << endl;
    for (unsigned int i = 0; i < 256; i++)
    {
        printf("0x%02X%- 10d%s\n", i, weights[i].first, weights[i].second.c_str());
    }
}
