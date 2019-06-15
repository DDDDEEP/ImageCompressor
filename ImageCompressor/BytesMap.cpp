#include "pch.h"
#include "toolkit.h"
#include "BytesMap.h"
using namespace std;

bool BytesMap::loadFile(const string &filepath)
{
    file_ptr fmap(filepath.c_str(), "rb");
    if (!FilePtrIsOpened(fmap, filepath.c_str()))
        return false;

    //每次从文件中读取一个字节，统计各种类字节出现的次数
    unsigned char ch = fgetc(fmap.get());
    while (!feof(fmap.get()))
    {
        ++bytes[ch].first;
        ch = fgetc(fmap.get());
    }

    return true;
}

vector<unsigned int> BytesMap::getBytesCount() const
{
    vector<unsigned int> res(256);
    for (size_t i = 0; i < bytes.size(); ++i)
    {
        res[i] = bytes[i].first;
    }
    return res;
}

void BytesMap::setBytesCount(const std::vector<unsigned int>& counts)
{
    for (size_t i = 0; i < counts.size() && i < bytes.size(); ++i)
    {
        bytes[i].first = counts[i];
    }
}

std::vector<std::string> BytesMap::getCodes() const
{
    std::vector<std::string> res(256);
    for (unsigned int i = 0; i < 256; ++i)
    {
        res[i] = bytes[i].second;
    }
    return res;
}

void BytesMap::setCodes(const vector<string> &codes)
{
    for (size_t i = 0; i < codes.size() && i < bytes.size(); ++i)
    {
        bytes[i].second = codes[i];
    }
}

void BytesMap::print() const
{
    cout << "Byte" << left << setfill(' ') << setw(10) << " Weight" <<  "Code" << endl;
    for (unsigned int i = 0; i < bytes.size(); ++i)
    {
        printf("0x%02X%- 10d%s\n", i, bytes[i].first, bytes[i].second.c_str());
    }
}
