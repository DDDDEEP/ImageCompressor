#pragma once
#include "pch.h"

const unsigned int BYTE_TYPES = 256; //字节种类总数

/* 字节编码列表类 */
class BytesMap
{
public:
    BytesMap() : bytes(BYTE_TYPES) {}
    void print() const;

    /* 读取文件，记录每种字节的出现次数 */
    bool loadFile(const std::string &filepath);

    /* 获取、设置字节出现次数 */
    std::vector<unsigned int> getBytesCount() const;
    void setBytesCount(const std::vector<unsigned int> &counts);

    /* 获取、设置编码值 */
    std::vector<std::string> getCodes() const;
    void setCodes(const std::vector<std::string> &codes);

private:
    std::vector<std::pair<unsigned int, std::string>> bytes; //256种字节的列表，记录<出现次数, 编码值>
};
