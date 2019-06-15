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
    //��ȡ�ļ��������ֽڱ�͹�������
    map->loadFile(filepath);
    tree = make_unique<HuffmanTree>(map->getBytesCount());
    map->setCodes(tree->getCodes());

    //���ļ���
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
        //�ҵ�Դ�ֽڶ�Ӧ���±��룬��λд�����ֽ��У����ֽ���8λ��д��
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
        //��ȡ�µ�Դ�ֽ�
        srcByte = fgetc(fsrc.get());
    }

    //��pos==0��˵�����д��ı���Ƭ�θպ�Ϊ8λ
    //��pos!=0������ҪΪ���д��ı���Ƭ�β���8λ
    if (pos)
    {
        newByte <<= (8 - pos);
        fputc(newByte, fcom.get());
    }

    //�����ֽ��б�
    saveMap(getMapPath(filepath));

    cout << "Compress File Created" << endl;
    return true;
}

bool ImageCompressor::decompress(const string &filepath)
{
    //��ȡ�ļ��������ֽڱ�͹�������
    readMap(getMapPath(filepath));
    tree = make_unique<HuffmanTree>(map->getBytesCount());
    map->setCodes(tree->getCodes());


    //���ļ���
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
        //��Ҷ�ӽڵ�˵���ҵ������Ӧ��Դ�ֽڣ�д���ѹ�ļ���
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
    //��ȡ�ļ��������ֽڱ�͹�������
    readMap(getMapPath(filepath));
    tree = make_unique<HuffmanTree>(map->getBytesCount());
    map->setCodes(tree->getCodes());

    //���ļ���
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
    //���ļ���
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

    //���ļ���
    file_ptr fmap(mappath.c_str(), "wb");
    if (!FilePtrIsOpened(fmap, mappath.c_str()))
        return false;

    string line;
    char buff[128];
    for (size_t i = 0; i < mapData.size(); ++i)
    {
        //��ÿ�����ִ�����Ϊ0���ֽڣ���Ӧ�����ݣ���[�ֽ�],[���ִ���]\n��
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
    //���ļ���
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

        //���ַ��ͻ����ַ���Ҫ���⴦��
        if (ch == '\0' || ch == '\n')
        {
            isSpecialByte = true;
            byteType = ch;
            ch = fgetc(fmap.get());
        }

        //��ȡһ��
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