#pragma once
#include "pch.h"

// 字节编码列表类
class WeightMap
{
public:
    WeightMap() : weights(256) {}

    void loadFile(const std::string &filepath);
    std::vector<unsigned int> getBytesCount() const;
    void setBytesCount(const std::vector<unsigned int> &counts);
    std::vector<std::string> getCodes() const;
    void setCodes(const std::vector<std::string> &codes);
    void print();

private:
    // <出现次数, 编码值>
    std::vector<std::pair<unsigned int, std::string>> weights;
};

