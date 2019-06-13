#include "pch.h"
#include "WeightMap.h"
using namespace std;

WeightMap::WeightMap(const string &filepath) : weights(256)
{
    // 每次从文件中读取一个字符，统计各字符出现的次数
    FILE* fout = fopen(filepath.c_str(), "rb");
    unsigned char ch;

    while (!feof(fout))
    {
        ch = fgetc(fout);
        ++weights[ch].first;
    }

    fclose(fout);
}

vector<unsigned int> WeightMap::getBytesCount()
{
    vector<unsigned int> res(256);
    for (unsigned i = 0; i < weights.size(); ++i)
    {
        res.push_back(weights[i].first);
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

void WeightMap::print()
{
    cout << "Byte" << left << setfill(' ') << setw(10) << " Weight" <<  "Code" << endl;
    for (unsigned int i = 0; i < 256; i++)
    {
        printf("0x%02X%- 10d%s\n", i, weights[i].first, weights[i].second.c_str());
    }
}
