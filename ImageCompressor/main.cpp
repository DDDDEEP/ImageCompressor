#include "pch.h"
#include "WeightMap.h"
#include "HuffmanTree.h"
using namespace std;

int main()
{
    string pathname = "D:\\repos\\ImageCompressor\\ImageCompressor\\img\\img_white.bmp";
    //cout << "请输入bmp图片路径：" << endl;
    //cin >> pathname;
    WeightMap map(pathname);
    HuffmanTree tree(map.getBytesCount());
    vector<string> codes = tree.getCode();
    map.setCodes(codes);
    map.print();
}
