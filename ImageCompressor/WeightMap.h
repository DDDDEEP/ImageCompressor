#pragma once
#include "pch.h"

// 字节编码列表类
class WeightMap
{
public:
    WeightMap(const std::string &filapath);

    std::vector<unsigned int> getBytesCount();
    void setCodes(const std::vector<std::string> &codes);
    void print();
    void compress();
    void decompress();
private:
    // <出现次数, 编码值>
    std::vector<std::pair<unsigned int, std::string>> weights;
};

