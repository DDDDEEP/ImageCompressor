#pragma once
#include "pch.h"

// �ֽڱ����б���
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
    // <���ִ���, ����ֵ>
    std::vector<std::pair<unsigned int, std::string>> weights;
};

