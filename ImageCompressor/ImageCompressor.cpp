#include "pch.h"
#include "WeightMap.h"
#include "HuffmanTree.h"
#include "ImageCompressor.h"
using namespace std;

typedef unique_ptr<FILE, decltype(std::fclose)*> file_ptr;

ImageCompressor::ImageCompressor() :
    map(make_unique<WeightMap>()),
    tree(make_unique<HuffmanTree>())
{
}

ImageCompressor::~ImageCompressor()
{
}

void ImageCompressor::compress(const string &filepath)
{
    map->loadFile(filepath);
    tree = make_unique<HuffmanTree>(map->getBytesCount());
    map->setCodes(tree->getCode());

    FILE* fin;
    string compressPath = move(getCompressPath(filepath));
    fopen_s(&fin, compressPath.c_str(), "wb");
    file_ptr pfin(fin, fclose);
    if (!(pfin.get()))
    {
        string errmsg = move(string("fopen ") + compressPath + " failed");
        perror(errmsg.c_str());
        exit(-1);
    }
    file_ptr fout(fopen(filepath.c_str(), "rb"), fclose);
    if (!(fout.get()))
    {
        string errmsg = move(string("fopen ") + filepath + " failed");
        perror(errmsg.c_str());
        exit(-1);
    }
    
    unsigned int pos = 0;
    unsigned char newByte = 0;
    unsigned char rawByte = fgetc(fout.get());
    vector<string> codes = std::move(map->getCodes());

    while (!feof(fout.get()))
    {
        //将字符转为二进制位，满8位就写入
        string code = codes[static_cast<unsigned int>(rawByte)];
        for (size_t i = 0; i < code.size(); ++i)
        {
            newByte <<= 1;
            newByte |= (code[i] == '1') ? 1 : 0;

            ++pos;
            if (pos == 8)
            {
                fputc(newByte, pfin.get());
                pos = 0;
                newByte = 0;
            }
        }
        rawByte = fgetc(fout.get());
    }

    if (pos)//pos==0，说明刚好存完8位；pos!=0，说明还有多余的位
    {
        newByte <<= (8 - pos);
        fputc(newByte, pfin.get());
    }
    saveMap(getMapPath(filepath));
}

void ImageCompressor::decompress(const string &filepath)
{
    readMap(getMapPath(filepath));
    tree = make_unique<HuffmanTree>(map->getBytesCount());
    tree->getCode();
    map->setCodes(tree->getCode());
    //压缩文件
    string decompressPath = getDecompressPath(filepath);
    string compressPath = getCompressPath(filepath);

    FILE *fin, *fout;
    fopen_s(&fin, decompressPath.c_str(), "wb");
    fopen_s(&fout, compressPath.c_str(), "rb");
    file_ptr pfin(fin, fclose);
    file_ptr pfout(fout, std::fclose);
    if (!(pfin.get()))
    {
        string errmsg = move(string("fopen ") + decompressPath + " failed");
        perror(errmsg.c_str());
        exit(-1);
    }
    if (!(pfout.get()))
    {
        string errmsg = move(string("fopen ") + compressPath + " failed");
        perror(errmsg.c_str());
        exit(-1);
    }

    tree->print();
    unsigned int pos = 0;
    unsigned char ch = fgetc(pfout.get());
    HuffmanPtr root = tree->getRoot();
    unsigned int charCount = root->weight;
    HuffmanPtr cur = root;
    int n = 0;
    while (charCount && !feof(pfout.get()))
    {
        //到叶子节点说明找到字符，放入解压文件
        if (cur->isLeaf())
        {
            fputc(cur->byte, pfin.get());
            --charCount;
            cur = root;
        }
        else
        {
            if (ch & (1 << (7 - pos)))
            {
                cur = cur->right;
            }
            else
            {
                cur = cur->left;
            }
            ++pos;
            //当pos<0时，说明此时已经解压完一个字节，开始解压下一个字节
            if (pos == 8)
            {
                pos = 0;
                ch = fgetc(pfout.get());

            }
        }

    }
}

void ImageCompressor::printData(const std::string & filepath, unsigned int count)
{
    readMap(getMapPath(filepath));
    tree = make_unique<HuffmanTree>(map->getBytesCount());
    map->setCodes(tree->getCode());

    FILE *fsource, *fcompress, *fdecompress;
    string compressPath = move(getCompressPath(filepath));
    string decompressPath = move(getDecompressPath(filepath));
    fopen_s(&fsource, filepath.c_str(), "rb");
    fopen_s(&fcompress, compressPath.c_str(), "rb");
    fopen_s(&fdecompress, decompressPath.c_str(), "rb");
    file_ptr pfsource(fsource, fclose);
    file_ptr pfcompress(fcompress, std::fclose);
    file_ptr pfdecompress(fdecompress, std::fclose);
    if (!(pfsource.get()))
    {
        string errmsg = move(string("fopen ") + filepath + " failed");
        perror(errmsg.c_str());
        exit(-1);
    }
    if (!(pfcompress.get()))
    {
        string errmsg = move(string("fopen ") + compressPath + " failed");
        perror(errmsg.c_str());
        exit(-1);
    }
    if (!(pfdecompress.get()))
    {
        string errmsg = move(string("fopen ") + decompressPath + " failed");
        perror(errmsg.c_str());
        exit(-1);
    }

    unsigned int index = 0;
    unsigned char ch = fgetc(pfsource.get());
    cout << "source     : ";
    while (!feof(pfsource.get()) && ++index <= count)
    {
        cout << bitset<8>(ch);
        printf("(%3d) ", ch);
        ch = fgetc(pfsource.get());
    }
    index = 0;
    cout << endl;
    cout << "compress   : ";
    ch = fgetc(pfcompress.get());
    unsigned int pos = 0;
    HuffmanPtr root = tree->getRoot(), cur = root;
    while (!feof(pfcompress.get()) && index <= count)
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
                ch = fgetc(pfcompress.get());
            }
        }
    }
    index = 0;
    cout << endl;
    cout << "decompress : ";
    ch = fgetc(pfdecompress.get());
    while (!feof(pfdecompress.get()) && ++index <= count)
    {
        cout << bitset<8>(ch);
        printf("(%3d) ", ch);
        ch = fgetc(pfdecompress.get());
    }
    index = 0;
    cout << endl;

}

std::string ImageCompressor::getMapPath(const std::string & filepath)
{
    return filepath + ".map";
}

std::string ImageCompressor::getCompressPath(const std::string & filepath)
{
    return filepath + ".compress";
}

std::string ImageCompressor::getDecompressPath(const std::string & filepath)
{
    size_t deStrPos = filepath.rfind('\\');
    string decompressPath = filepath;
    decompressPath.insert(deStrPos + 1, "compress_");
    return decompressPath;
}

void ImageCompressor::saveMap(const string &mappath)
{
    vector<unsigned int> mapData = std::move(map->getBytesCount());
    FILE *fin;
    fopen_s(&fin, mappath.c_str(), "wb");
    file_ptr pfin(fin, fclose);
    if (!(pfin.get()))
    {
        string errmsg = move(string("fopen ") + mappath + " failed");
        perror(errmsg.c_str());
        exit(-1);
    }

    string line;
    char buff[128];

    for (size_t i = 0; i < 256; ++i)
    {
        if (mapData[i])
        {
            if (i == 0)
            {
                unsigned char zero = 0;
                fwrite(&zero, 1, 1, pfin.get());
            }
            else
            {
                line += static_cast<unsigned char>(i);
            }
            line += ',';
            line += _itoa(mapData[i], buff, 10);
            line += '\n';
            fputs(line.c_str(), pfin.get());
            line.clear();
        }
    }
}

void ImageCompressor::readMap(const string &filepath)
{

    FILE *fout;
    fopen_s(&fout, filepath.c_str(), "rb");
    file_ptr pfout(fout, fclose);
    if (!(pfout.get()))
    {
        string errmsg = move(string("fopen ") + filepath + " failed");
        perror(errmsg.c_str());
        exit(-1);
    }
    string line;
    unsigned char ch = 0;
    unsigned char byte = 0;
    bool isSpecialByte = false;
    vector<unsigned int> counts(256);
    while (true)
    {
        ch = fgetc(pfout.get());
        if (feof(pfout.get()))
            break;
        if (ch == '\0' || ch == '\n')
        {
            isSpecialByte = true;
            byte = ch;
            ch = fgetc(pfout.get());
        }
        while (!feof(pfout.get()) && ch != '\n')
        {
            line += ch;
            ch = fgetc(pfout.get());
        }
        if (!line.empty())
        {
            if (isSpecialByte)
            {
                counts[static_cast<unsigned int>(byte)] = atoi(line.substr(1).c_str());
            }
            else
            {
                byte = line[0];
                counts[static_cast<unsigned int>(byte)] = atoi(line.substr(2).c_str());
            }
            line.clear();
        }
        isSpecialByte = false;
    }
    map->setBytesCount(counts);
}