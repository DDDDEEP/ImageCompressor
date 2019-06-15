#pragma once
#include "pch.h"

class BytesMap;
class HuffmanTree;
class ImageCompressor
{
public:
    ImageCompressor();
    ~ImageCompressor();

    /* 压缩、解压 */
    bool compress(const std::string &filepath);
    bool decompress(const std::string &filepath);

    /* 分别输出源文件、压缩文件和解压后的文件的内容 */
    void printData(const std::string &filepath, unsigned int count);

    /* 输出解压文件信息 */
    void printInfo(const std::string &filepath);

private:
    std::unique_ptr<BytesMap> map;
    std::unique_ptr<HuffmanTree> tree;

    /* 获取列表文件、压缩文件和解压文件的路径 */
    std::string getMapPath(const std::string &filepath);
    std::string getCompressPath(const std::string &filepath);
    std::string getDecompressPath(const std::string &filepath);

    /* 保存、读取列表文件 */
    bool saveMap(const std::string &mappath);
    bool readMap(const std::string &mappath);
};

