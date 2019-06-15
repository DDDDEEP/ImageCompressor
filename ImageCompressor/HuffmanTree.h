#pragma once
#include "pch.h"
#include "toolkit.h"

struct HuffmanNode
{
    bool isLeaf();
    unsigned int val;   //Ȩ��ֵ
    unsigned char byte; //����Ҷ�ӣ����Ӧ���ֽ�����
    std::string code;   //����
    HuffmanNode* parent = nullptr;
    HuffmanNode* left = nullptr;
    HuffmanNode* right = nullptr;
};
typedef HuffmanNode* HuffmanPtr;

class HuffmanTree
{
public:
    HuffmanTree() {}
    ~HuffmanTree();

    /* ���ݸ������ֽڵĳ��ִ����������������� */
    HuffmanTree(const std::vector<unsigned int> &bytesCount);

    /* ��ȡ�������ֽڵı���ֵ */
    std::vector<std::string> getCodes() const;

    inline HuffmanPtr getRoot() const
    {
        return root;
    }
    inline void print()
    {
        PrintTree<HuffmanNode>("", root, true);
    }

private:
    HuffmanPtr root;
};


