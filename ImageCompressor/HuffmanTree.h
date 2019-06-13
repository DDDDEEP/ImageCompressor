#pragma once
#include "pch.h"

struct HuffmanNode
{
    bool isLeaf();
    unsigned int weight;
    unsigned char byte;
    std::string code;
    HuffmanNode* parent = nullptr;
    HuffmanNode* left = nullptr;
    HuffmanNode* right = nullptr;
};
typedef HuffmanNode* HuffmanPtr;

class HuffmanTree
{
public:
    HuffmanTree(const std::vector<unsigned int> &weight);
    ~HuffmanTree();

    std::vector<std::string> getCode();
    void print();
private:
    HuffmanNode* root;
    
};

template<class T>
class TreePrinter
{
public:
    TreePrinter(T* root) : root(root)
    {}
    void print(std::string prefix, T* node, bool isLeft);
private:
    T* root;
};

template<class T>
inline void TreePrinter<T>::print(std::string prefix, T* node, bool isLeft)
{
    if (node != nullptr)
    {
        std::cout << prefix << (isLeft ? "|-- " : "\\-- ") << node->weight << std::endl;
        print(prefix + (isLeft ? "|   " : "    "), node->left, true);
        print(prefix + (isLeft ? "|   " : "    "), node->right, false);
    }
}
