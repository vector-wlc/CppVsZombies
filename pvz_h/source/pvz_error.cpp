/*
 * @coding: utf-8
 * @Author: Chu Wenlong
 * @FilePath: \pvz_h\source\pvz_error.cpp
 * @Date: 2019-10-10 23:08:08
 * @LastEditTime : 2019-12-31 23:10:36
 * @Description: 错误相关函数的实现
 */

#include "pvz_error.h"

namespace pvz

{
#ifndef GBK

void UTF8ToGBK(std::string &strUTF8)
{
    int len = MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, NULL, 0);
    wchar_t *wszGBK = new wchar_t[len + 1];
    memset(wszGBK, 0, len * 2 + 2);
    MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, wszGBK, len);
    len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
    char *szGBK = new char[len + 1];
    memset(szGBK, 0, len + 1);
    WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGBK, len, NULL, NULL);
    strUTF8 = szGBK;
    if (wszGBK)
        delete[] wszGBK;
    if (szGBK)
        delete[] szGBK;
}
#endif

} // namespace pvz
