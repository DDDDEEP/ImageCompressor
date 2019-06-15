#pragma once
#include "pch.h"

const unsigned int BYTE_TYPES = 256; //�ֽ���������

/* �ֽڱ����б��� */
class BytesMap
{
public:
    BytesMap() : bytes(BYTE_TYPES) {}
    void print() const;

    /* ��ȡ�ļ�����¼ÿ���ֽڵĳ��ִ��� */
    bool loadFile(const std::string &filepath);

    /* ��ȡ�������ֽڳ��ִ��� */
    std::vector<unsigned int> getBytesCount() const;
    void setBytesCount(const std::vector<unsigned int> &counts);

    /* ��ȡ�����ñ���ֵ */
    std::vector<std::string> getCodes() const;
    void setCodes(const std::vector<std::string> &codes);

private:
    std::vector<std::pair<unsigned int, std::string>> bytes; //256���ֽڵ��б���¼<���ִ���, ����ֵ>
};
