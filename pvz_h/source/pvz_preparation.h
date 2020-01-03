/*
 * @coding: utf-8
 * @Author: Chu Wenlong
 * @FilePath: \pvz_h\source\pvz_preparation.h
 * @Date: 2019-10-10 23:50:46
 * @LastEditTime : 2019-12-28 23:03:40
 * @Description: 程序进入 main 和退出 main 的一些准备
 */

#ifndef PVZ_PREPARATION_H
#define PVZ_PREPARATION_H

namespace pvz
{

//工具人
class CvZPreparation
{
private:
    void find_pvz_window();
    void get_pvz_window_dpi();
    void some_preparations();

public:
    CvZPreparation();
    ~CvZPreparation();
};
} // namespace pvz

#endif