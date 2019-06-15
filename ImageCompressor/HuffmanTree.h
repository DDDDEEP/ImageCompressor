#pragma once
#include "pch.h"
#include "toolkit.h"

struct HuffmanNode
{
    bool isLeaf();
    unsigned int val;   //权重值
    unsigned char byte; //对于叶子，其对应的字节种类
    std::string code;   //编码
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

    /* 根据各种类字节的出现次数，构建哈夫曼树 */
    HuffmanTree(const std::vector<unsigned int> &bytesCount);

    /* 获取各种类字节的编码值 */
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


