
// pvz_preparation.h
// 主要做一些main函数之前的准备工作和main函数之后的处理工作
// 1.在进入 main 之前，读取一些内存地址
// 2.在退出 main 之后，保持程序直到游戏退出战斗界面

#ifndef PVZ_PREPARATION_H
#define PVZ_PREPARATION_H

namespace pvz
{

//工具人
class CvZPreparation
{
private:
    void exit_thread();
    void find_pvz_window();
    void get_pvz_window_dpi();
    void some_preparations();

public:
    CvZPreparation();
    ~CvZPreparation();
};
} // namespace pvz

#endif