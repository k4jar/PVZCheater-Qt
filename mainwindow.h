#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <Windows.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    friend DWORD WINAPI MonitoringThreadProc(LPVOID lpParam);
    Q_OBJECT
    
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    //无限阳光
    void MaxSun();
    //秒杀僵尸
    void KillJs();
    //禁用CD
    void BanCd();
    //植物无敌
    void GodZw();
    //后台运行
    void Runing();
    //僵尸无敌
    void GodJs();
    //无限金钱
    void MaxMoney();
    //一键取消
    void OffAll();
    //联系作者
    void Chat();
protected:
    //关闭程序函数
    void closeEvent(QCloseEvent *event); 
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
