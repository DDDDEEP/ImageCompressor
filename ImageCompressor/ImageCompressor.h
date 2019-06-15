#pragma once
#include "pch.h"

class WeightMap;
class HuffmanTree;
class ImageCompressor
{
public:
    ImageCompressor();
    ~ImageCompressor();
    void compress(const std::string &filepath);
    void decompress(const std::string &filepath);
    void printData(const std::string &filepath, unsigned int count);
private:
    std::unique_ptr<WeightMap> map;
    std::unique_ptr<HuffmanTree> tree;

    std::string getMapPath(const std::string &filepath);
    std::string getCompressPath(const std::string &filepath);
    std::string getDecompressPath(const std::string &filepath);
    void saveMap(const std::string &filepath);
    void readMap(const std::string &filepath);
};

