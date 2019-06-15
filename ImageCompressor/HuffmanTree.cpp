#include "pch.h"
#include "HuffmanTree.h"
using namespace std;

bool HuffmanNode::isLeaf()
{
    return left == nullptr && right == nullptr;
}

HuffmanTree::HuffmanTree(const vector<unsigned int>& weight)
{
    vector<HuffmanPtr> node_list;
    // 创建叶子结点列表
    for (unsigned int i = 0 ; i < weight.size(); ++i)
    {
        if (weight[i] != 0)
        {
            HuffmanPtr node = new HuffmanNode();
            node->weight = weight[i];
            node->byte = i;
            node_list.push_back(node);
        }
    }

    // 降序排序
    sort(node_list.begin(), node_list.end(),
        [](const HuffmanPtr &lhs, const HuffmanPtr &rhs)
    {
        return lhs->weight > rhs->weight;
    });

    //// 打印
    //for (auto i : node_list)
    //{
    //    cout << i->weight << endl;
    //}

    // 递归合并叶子结点
    while (node_list.size() != 1)
    {
        HuffmanPtr node = new HuffmanNode();
        node->left = node_list[node_list.size() - 2];
        node->right = node_list.back();
        node->left->parent = node->right->parent = node;
        node->weight = node->left->weight + node->right->weight;

        node_list.pop_back();
        node_list.pop_back();

        vector<HuffmanPtr>::iterator pos =
            lower_bound(node_list.begin(), node_list.end(), node,
            [](const HuffmanPtr &lhs, const HuffmanPtr &rhs)
        {
            return lhs->weight >= rhs->weight;
        });
        if (pos == node_list.end())
            node_list.push_back(node);
        else
            node_list.insert(pos + 1, node);

    }

    root = node_list.front();
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

vector<string> HuffmanTree::getCode() const
{
    std::vector<std::string> res(256);
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

void HuffmanTree::print()
{
    TreePrinter<HuffmanNode> tpr(root);
    tpr.print("", root, false);
}



