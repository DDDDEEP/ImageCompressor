#include "pch.h"
#include "ImageCompressor.h"
using namespace std;

int main()
{
    string filepath;
    cout << "Please Input the Path of File：" << endl;
    cin >> filepath;

    ImageCompressor ic;
    ic.compress(filepath);
    ic.decompress(filepath);
    cout << endl;
    ic.printInfo(filepath);
}
