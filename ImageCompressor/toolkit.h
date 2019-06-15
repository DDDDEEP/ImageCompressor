#pragma once

/* �ļ�����ָ�� */
class file_ptr : public std::unique_ptr<FILE, decltype(std::fclose)*>
{
public:
    file_ptr(const char *filename, const char *mode)
        : unique_ptr<FILE, decltype(std::fclose)*>(nullptr, std::fclose)
    {
        fopen_s(&f, filename, mode);
        this->reset(f);
    }
private:
    FILE* f;
};

/* ��ӡ�������ṹ */
template<class T>
void PrintTree(const char *prefix, const T *node, bool isLeft)
{
    if (node != nullptr)
    {
        std::cout << prefix << (isLeft ? "|-- " : "\\-- ") << node->val << std::endl;
        char str[32];
        strcat_s(str, sizeof(str), prefix);
        strcat_s(str, sizeof(str), (isLeft ? "|   " : "    "));
        PrintTree(str, node->left, true);
        PrintTree(str, node->right, false);
    }
}

/* �ж��ļ�ָ���Ƿ�򿪳ɹ� */
inline bool FilePtrIsOpened(const file_ptr &ptr, const char *filename)
{
    if (!(ptr.get()))
    {
        char errmsg[1024] = "fopen ";
        strcat_s(errmsg, sizeof(errmsg), filename);
        strcat_s(errmsg, sizeof(errmsg), " failed");
        perror(errmsg);
        return false;
    }
    else
        return true;
}
