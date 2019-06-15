#pragma once
#include "pch.h"

// �ֽڱ����б���
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
    // <���ִ���, ����ֵ>
    std::vector<std::pair<unsigned int, std::string>> weights;
};

