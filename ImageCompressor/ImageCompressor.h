#pragma once
#include "pch.h"

class BytesMap;
class HuffmanTree;
class ImageCompressor
{
public:
    ImageCompressor();
    ~ImageCompressor();

    /* ѹ������ѹ */
    bool compress(const std::string &filepath);
    bool decompress(const std::string &filepath);

    /* �ֱ����Դ�ļ���ѹ���ļ��ͽ�ѹ����ļ������� */
    void printData(const std::string &filepath, unsigned int count);

    /* �����ѹ�ļ���Ϣ */
    void printInfo(const std::string &filepath);

private:
    std::unique_ptr<BytesMap> map;
    std::unique_ptr<HuffmanTree> tree;

    /* ��ȡ�б��ļ���ѹ���ļ��ͽ�ѹ�ļ���·�� */
    std::string getMapPath(const std::string &filepath);
    std::string getCompressPath(const std::string &filepath);
    std::string getDecompressPath(const std::string &filepath);

    /* ���桢��ȡ�б��ļ� */
    bool saveMap(const std::string &mappath);
    bool readMap(const std::string &mappath);
};

