#include "pch.h"
#include "toolkit.h"
#include "BytesMap.h"
#include "HuffmanTree.h"
#include "ImageCompressor.h"
using namespace std;


ImageCompressor::ImageCompressor() :
    map(make_unique<BytesMap>()),
    tree(make_unique<HuffmanTree>())
{
}

ImageCompressor::~ImageCompressor()
{
}

bool ImageCompressor::compress(const string &filepath)
{
    //读取文件，生成字节表和哈夫曼树
    map->loadFile(filepath);
    tree = make_unique<HuffmanTree>(map->getBytesCount());
    map->setCodes(tree->getCodes());

    //打开文件流
    string compressPath = move(getCompressPath(filepath));
    file_ptr fcom(compressPath.c_str(), "wb"),
        fsrc(filepath.c_str(), "rb");
    if (!FilePtrIsOpened(fcom, compressPath.c_str())
        || !FilePtrIsOpened(fsrc, filepath.c_str()))
    {
        return false;
    }
    
    string code;
    unsigned int pos = 0;
    unsigned char newByte = 0;
    unsigned char srcByte = fgetc(fsrc.get());
    vector<string> codes = std::move(map->getCodes());
    while (!feof(fsrc.get()))
    {
        //找到源字节对应的新编码，逐位写入新字节中，新字节满8位就写入
        code = codes[static_cast<unsigned int>(srcByte)];
        for (size_t i = 0; i < code.size(); ++i)
        {
            newByte <<= 1;
            newByte |= (code[i] == '1') ? 1 : 0;

            ++pos;
            if (pos == 8)
            {
                fputc(newByte, fcom.get());
                pos = 0;
                newByte = 0;
            }
        }
        //读取新的源字节
        srcByte = fgetc(fsrc.get());
    }

    //若pos==0，说明最后写入的编码片段刚好为8位
    //若pos!=0，则需要为最后写入的编码片段补到8位
    if (pos)
    {
        newByte <<= (8 - pos);
        fputc(newByte, fcom.get());
    }

    //保存字节列表
    saveMap(getMapPath(filepath));

    cout << "Compress File Created" << endl;
    return true;
}

bool ImageCompressor::decompress(const string &filepath)
{
    //读取文件，生成字节表和哈夫曼树
    readMap(getMapPath(filepath));
    tree = make_unique<HuffmanTree>(map->getBytesCount());
    map->setCodes(tree->getCodes());


    //打开文件流
    string decompressPath = getDecompressPath(filepath),
        compressPath = getCompressPath(filepath);
    file_ptr fdecom(decompressPath.c_str(), "wb"),
        fcom(compressPath.c_str(), "rb");
    if (!FilePtrIsOpened(fdecom, decompressPath.c_str())
        || !FilePtrIsOpened(fcom, compressPath.c_str()))
    {
        return false;
    }

    HuffmanPtr root = tree->getRoot(),
        cur = root;
    unsigned int pos = 0;
    unsigned int charCount = root->val;

    unsigned char comByte = fgetc(fcom.get());
    while (charCount && !feof(fcom.get()))
    {
        //到叶子节点说明找到编码对应的源字节，写入解压文件中
        if (cur->isLeaf())
        {
            fputc(cur->byte, fdecom.get());
            --charCount;
            cur = root;
        }
        else
        {
            if (comByte & (1 << (7 - pos)))
            {
                cur = cur->right;
            }
            else
            {
                cur = cur->left;
            }
            ++pos;
            if (pos == 8)
            {
                pos = 0;
                comByte = fgetc(fcom.get());
            }
        }
    }

    cout << "Decompress File Created" << endl;
    return true;
}

void ImageCompressor::printData(const std::string &filepath, unsigned int count)
{
    //读取文件，生成字节表和哈夫曼树
    readMap(getMapPath(filepath));
    tree = make_unique<HuffmanTree>(map->getBytesCount());
    map->setCodes(tree->getCodes());

    //打开文件流
    string compressPath = move(getCompressPath(filepath));
    string decompressPath = move(getDecompressPath(filepath));
    file_ptr fsrc(filepath.c_str(), "rb"),
        fcom(compressPath.c_str(), "rb"),
        fdecom(decompressPath.c_str(), "rb");
    if (!FilePtrIsOpened(fsrc, filepath.c_str())
        || !FilePtrIsOpened(fcom, compressPath.c_str())
        || !FilePtrIsOpened(fdecom, decompressPath.c_str()))
    {
        return;
    }

    cout << "source     : ";
    unsigned int index = 0;
    unsigned char ch = fgetc(fsrc.get());
    while (!feof(fsrc.get()) && ++index <= count)
    {
        cout << bitset<8>(ch);
        printf("(%3d) ", ch);
        ch = fgetc(fsrc.get());
    }
    cout << endl;

    cout << "compress   : ";
    index = 0;
    unsigned int pos = 0;
    HuffmanPtr root = tree->getRoot(),
        cur = root;
    ch = fgetc(fcom.get());
    while (!feof(fcom.get()) && index <= count)
    {
        if (cur->isLeaf())
        {
            printf("(%3d) ", cur->byte);
            ++index;
            cur = root;
        }
        else
        {
            if (ch & (1 << (7 - pos)))
            {
                cur = cur->right;
                cout << "1";
            }
            else
            {
                cur = cur->left;
                cout << "0";
            }
            ++pos;
            if (pos == 8)
            {
                pos = 0;
                ch = fgetc(fcom.get());
            }
        }
    }
    cout << endl;

    cout << "decompress : ";
    index = 0;
    ch = fgetc(fdecom.get());
    while (!feof(fdecom.get()) && ++index <= count)
    {
        cout << bitset<8>(ch);
        printf("(%3d) ", ch);
        ch = fgetc(fdecom.get());
    }
    cout << endl;
}

void ImageCompressor::printInfo(const string &filepath)
{
    //打开文件流
    string compressPath = move(getCompressPath(filepath));
    string decompressPath = move(getDecompressPath(filepath));
    file_ptr fsrc(filepath.c_str(), "rb"),
        fcom(compressPath.c_str(), "rb"),
        fdecom(decompressPath.c_str(), "rb");
    if (!FilePtrIsOpened(fsrc, filepath.c_str())
        || !FilePtrIsOpened(fcom, compressPath.c_str())
        || !FilePtrIsOpened(fdecom, decompressPath.c_str()))
    {
        return;
    }

    fseek(fsrc.get(), 0L, SEEK_END);
    fseek(fcom.get(), 0L, SEEK_END);
    fseek(fdecom.get(), 0L, SEEK_END);

    cout << "source     : " << ftell(fsrc.get()) << endl;
    cout << "compress   : " << ftell(fcom.get()) << endl;
    cout << "decompress : " << ftell(fdecom.get()) << endl;
}

string ImageCompressor::getMapPath(const string &filepath)
{
    return filepath + ".map";
}

string ImageCompressor::getCompressPath(const string &filepath)
{
    return filepath + ".compress";
}

string ImageCompressor::getDecompressPath(const string &filepath)
{
    size_t deStrPos = filepath.rfind('\\');
    string decompressPath = filepath;
    decompressPath.insert(deStrPos + 1, "decompress_");
    return decompressPath;
}

bool ImageCompressor::saveMap(const string &mappath)
{
    vector<unsigned int> mapData = move(map->getBytesCount());

    //打开文件流
    file_ptr fmap(mappath.c_str(), "wb");
    if (!FilePtrIsOpened(fmap, mappath.c_str()))
        return false;

    string line;
    char buff[128];
    for (size_t i = 0; i < mapData.size(); ++i)
    {
        //对每个出现次数不为0的字节，对应行数据：“[字节],[出现次数]\n”
        if (mapData[i])
        {
            if (i == 0)
            {
                unsigned char zero = 0;
                fputc(zero, fmap.get());
            }
            else
            {
                line += static_cast<unsigned char>(i);
            }
            line += ',';
            line += _itoa(mapData[i], buff, 10);
            line += '\n';
            fputs(line.c_str(), fmap.get());
            line.clear();
        }
    }
    cout << "Map File Created" << endl;
    return true;
}

bool ImageCompressor::readMap(const string &mappath)
{
    //打开文件流
    file_ptr fmap(mappath.c_str(), "rb");
    if (!FilePtrIsOpened(fmap, mappath.c_str()))
        return false;

    string line;
    unsigned char ch = 0;
    unsigned char byteType = 0;
    bool isSpecialByte = false;
    vector<unsigned int> counts(BYTE_TYPES);
    while (true)
    {
        ch = fgetc(fmap.get());
        if (feof(fmap.get()))
            break;

        //零字符和换行字符需要特殊处理
        if (ch == '\0' || ch == '\n')
        {
            isSpecialByte = true;
            byteType = ch;
            ch = fgetc(fmap.get());
        }

        //读取一行
        while (!feof(fmap.get()) && ch != '\n')
        {
            line += ch;
            ch = fgetc(fmap.get());
        }

        if (!line.empty())
        {
            if (isSpecialByte)
            {
                counts[static_cast<unsigned int>(byteType)] = atoi(line.substr(1).c_str());
            }
            else
            {
                byteType = line[0];
                counts[static_cast<unsigned int>(byteType)] = atoi(line.substr(2).c_str());
            }
            line.clear();
        }
        isSpecialByte = false;
    }
    map->setBytesCount(counts);

    return true;
}