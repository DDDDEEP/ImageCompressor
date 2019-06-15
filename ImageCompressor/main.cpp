#include "pch.h"
#include "ImageCompressor.h"
using namespace std;

int main()
{
    string filepath = "D:\\repos\\ImageCompressor\\ImageCompressor\\img\\img_white.bmp";
    //cout << "请输入bmp图片路径：" << endl;
    //cin >> pathname;
    ImageCompressor ic;
    ic.compress(filepath);
    ic.decompress(filepath);
    //ic.printData(filepath, 10);
    ic.printInfo(filepath);
}
