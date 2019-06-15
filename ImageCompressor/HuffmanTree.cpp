#include "pch.h"
#include "BytesMap.h"
#include "HuffmanTree.h"
using namespace std;

bool HuffmanNode::isLeaf()
{
    return left == nullptr && right == nullptr;
}

HuffmanTree::HuffmanTree(const vector<unsigned int> &bytesCount)
{
    //����Ҷ���б�
    vector<HuffmanPtr> nodeList;
    for (unsigned int i = 0 ; i < bytesCount.size(); ++i)
    {
        if (bytesCount[i] != 0)
        {
            HuffmanPtr node = new HuffmanNode();
            node->val = bytesCount[i];
            node->byte = i;
            nodeList.push_back(node);
        }
    }

    //��������
    sort(nodeList.begin(), nodeList.end(),
        [](const HuffmanPtr &lhs, const HuffmanPtr &rhs)
    {
        return lhs->val > rhs->val;
    });


    //�Ӻ���ǰ���ݹ�ϲ�Ҷ�ӽ��
    while (nodeList.size() != 1)
    {
        //�������ڽ��
        HuffmanPtr node = new HuffmanNode();
        node->left = nodeList[nodeList.size() - 2];
        node->right = nodeList.back();
        node->left->parent = node->right->parent = node;
        node->val = node->left->val + node->right->val;

        nodeList.pop_back();
        nodeList.pop_back();

        //���µ��ڽ����뵽����������б���
        vector<HuffmanPtr>::iterator pos =
            lower_bound(nodeList.begin(), nodeList.end(), node,
            [](const HuffmanPtr &lhs, const HuffmanPtr &rhs)
        {
            return lhs->val >= rhs->val;
        });
        if (pos == nodeList.end())
            nodeList.push_back(node);
        else
            nodeList.insert(pos + 1, node);
    }

    root = nodeList.front();
}

HuffmanTree::~HuffmanTree()
{
    if (root == nullptr)
        return;

    queue<HuffmanPtr> queue;
    queue.push(root);
    while (queue.size() != 0)
    {
        HuffmanPtr node = queue.front();
        if (!node->isLeaf())
        {
            queue.push(node->left);
            queue.push(node->right);
        }
        delete node;
        node = nullptr;
        queue.pop();
    }
}

vector<string> HuffmanTree::getCodes() const
{
    vector<string> res(BYTE_TYPES);
    queue<HuffmanPtr> queue;
    queue.push(root);
    while (queue.size() != 0)
    {
        HuffmanPtr node = queue.front();
        if (node->isLeaf())
        {
            res[node->byte] = node->code;
        }
        else
        {
            node->left->code = node->code + "0";
            node->right->code = node->code + "1";
            queue.push(node->left);
            queue.push(node->right);
        }
        queue.pop();
    }
    return res;
}
