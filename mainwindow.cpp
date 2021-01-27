#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <Windows.h>
#include <QDesktopServices>
#include <QUrl>
#pragma comment (lib,"Advapi32.lib")



// 监控线程的刷新时间
#define MONITOR_REFRESH_TIME_INTERVAL 1000

// 植物大战僵尸的进程句柄
static HANDLE g_processHandle;
// 用来监控植物大战僵尸的线程句柄
static HANDLE g_monitoringThreadHandle;
// 辅助窗口
static MainWindow* wid;

// 将某个值写入植物大战僵尸内存（后面的可变参数是地址链，要以-1结尾）
void WriteMemory(void *value, DWORD valueSize, ...)
{
	if (value == NULL || valueSize == 0 || g_processHandle == NULL) return;

	DWORD tempValue = 0;

	va_list addresses;
	va_start(addresses, valueSize);
	DWORD offset = 0;
	DWORD lastAddress = 0;
	while ((offset = va_arg(addresses, DWORD)) != -1)
	{
		lastAddress = tempValue + offset;
		::ReadProcessMemory(g_processHandle, (LPCVOID)lastAddress, &tempValue, sizeof(DWORD), NULL);
	}
	va_end(addresses);

	::WriteProcessMemory(g_processHandle, (LPVOID)lastAddress, value, valueSize, NULL);
}

void WriteMemory(void *value, DWORD valueSize, DWORD address) {
	WriteMemory(value, valueSize, address, -1);
}

// 用来监控植物大战僵尸的线程
DWORD WINAPI MonitoringThreadProc(LPVOID lpParam)
{
	while (1)
	{
		// 找窗口
		HWND hwnd = ::FindWindow(TEXT("MainWindow"), TEXT("植物大战僵尸中文版"));
		if (hwnd == NULL)
		{
			::CloseHandle(g_processHandle);
			g_processHandle = NULL;
            
//            //设置按钮禁止点击
            
            //设置无限阳光取消选中,不可点击
            wid->ui->max_sun->setChecked(false);
            wid->ui->max_sun->setEnabled(false);
            
            //设置秒杀僵尸取消选中,不可点击
            wid->ui->kill_js->setChecked(false);
            wid->ui->kill_js->setEnabled(false);
            wid->KillJs();
            
            //设置禁用CD取消选中,不可点击
            wid->ui->ban_cd->setChecked(false);
            wid->ui->ban_cd->setEnabled(false);
            wid->BanCd();
            
            //设置植物无敌取消选中,不可点击
            wid->ui->god_zw->setChecked(false);
            wid->ui->god_zw->setEnabled(false);
            wid->GodZw();
            
            //设置后台运行取消选中,不可点击
            wid->ui->runing->setChecked(false);
            wid->ui->runing->setEnabled(false);
            wid->Runing();
            
            //设置僵尸无敌取消选中,不可点击
            wid->ui->god_js->setChecked(false);
            wid->ui->god_js->setEnabled(false);
            wid->GodJs();
            
            //设置无限金钱不可点击
            wid->ui->max_money->setEnabled(false);
            
            //设置一键取消不可点击
            wid->ui->off_all->setEnabled(false);
            
            
            
		}
		else if (g_processHandle == NULL)
		{
			// 找进程ID
			DWORD pid = NULL;
			::GetWindowThreadProcessId(hwnd, &pid);

			// 找进程句柄
			g_processHandle = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

            //设置可以点击
            wid->ui->max_sun->setEnabled(true);
            wid->ui->kill_js->setEnabled(true);
            wid->ui->ban_cd->setEnabled(true);
            wid->ui->god_zw->setEnabled(true);
            wid->ui->runing->setEnabled(true);
            wid->ui->god_js->setEnabled(true);
            wid->ui->max_money->setEnabled(true);
            wid->ui->off_all->setEnabled(true);
            
		}
        if(wid->ui->max_sun->isChecked())
        {
            DWORD value = 9990;
            WriteMemory(&value, sizeof(value), 0x6A9EC0, 0x320, 0x8, 0x0, 0x8, 0x144, 0x2c, 0x5560, -1);
        }

		::Sleep(MONITOR_REFRESH_TIME_INTERVAL);
	}
	return 0;
}

// 针对XP提高程序的访问权限
BOOL ImproveAccessPrivilege()
{
	HANDLE tokenHandle;
	LUID privilegeValue;

	if (!::OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &tokenHandle)) return FALSE;

	if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &privilegeValue))
	{
		::CloseHandle(tokenHandle);
		return FALSE;
	}

	TOKEN_PRIVILEGES privileges;
	privileges.PrivilegeCount = 1;
	privileges.Privileges[0].Luid = privilegeValue;
	privileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	if (!::AdjustTokenPrivileges(tokenHandle, FALSE, &privileges, sizeof(privileges), NULL, NULL))
	{
		::CloseHandle(tokenHandle);
		return FALSE;
	}

	return TRUE;
}



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(this->width(),this->height());
    
    // 针对XP系统提高程序的权限
	ImproveAccessPrivilege();

	// 辅助窗口
	wid = this;

	// 启动监控线程
	g_monitoringThreadHandle = ::CreateThread(NULL, 0, MonitoringThreadProc, NULL, 0, NULL);
    
    //链接信号和槽
    connect(ui->max_sun,&QCheckBox::clicked,this,&MainWindow::MaxSun);
    connect(ui->kill_js,&QCheckBox::clicked,this,&MainWindow::KillJs);
    connect(ui->ban_cd,&QCheckBox::clicked,this,&MainWindow::BanCd);
    connect(ui->god_zw,&QCheckBox::clicked,this,&MainWindow::GodZw);
    connect(ui->god_js,&QCheckBox::clicked,this,&MainWindow::GodJs);
    connect(ui->max_money,&QCheckBox::clicked,this,&MainWindow::MaxMoney);
    connect(ui->off_all,&QCheckBox::clicked,this,&MainWindow::OffAll);
    connect(ui->chat,&QCheckBox::clicked,this,&MainWindow::Chat);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//无限阳光
void MainWindow::MaxSun()
{
    if(ui->max_sun->isChecked())
    {
        DWORD value = 9990;
        WriteMemory(&value, sizeof(value), 0x6A9EC0, 0x320, 0x8, 0x0, 0x8, 0x144, 0x2c, 0x5560, -1);
    }
}

//秒杀僵尸
void MainWindow::KillJs()
{
    DWORD address1 = 0x53130F;
	DWORD address2 = 0x531066;
	DWORD address3 = 0x530CB1;
	if (ui->kill_js->isChecked())
	{ // 如果需要秒杀僵尸
	  // 去除僵尸不死
		ui->god_js->setChecked(false);
        GodJs();

		BYTE data1[] = { 0x2B, 0xFF, 0x90, 0x90 };
		WriteMemory(data1, sizeof(data1), address1);

		BYTE data2[] = { 0x90, 0x90 };
		WriteMemory(data2, sizeof(data2), address2);

		BYTE data3[] = { 0x90, 0x90 };
		WriteMemory(data3, sizeof(data3), address3);
	}
	else
	{ // 如果不需要秒杀僵尸
		BYTE data1[] = { 0x2B, 0x7C, 0x24, 0x20 };
		WriteMemory(data1, sizeof(data1), address1);

		BYTE data2[] = { 0x75, 0x11 };
		WriteMemory(data2, sizeof(data2), address2);

		BYTE data3[] = { 0x75, 0x17 };
		WriteMemory(data3, sizeof(data3), address3);
	}
}

//禁用CD
void MainWindow::BanCd()
{
    DWORD address = 0x487296;
        if (ui->ban_cd->isChecked())
        { // 如果需要无CD
            BYTE data[] = { 0x90, 0x90 };
            WriteMemory(data, sizeof(data), address);
        }
        else
        { // 如果不需要无CD
            BYTE data[] = { 0x7E, 0x14 };
            WriteMemory(data, sizeof(data), address);
        }
}

//植物无敌
void MainWindow::GodZw()
{
    DWORD address1 = 0x52FCF0;
        DWORD address2 = 0x46D7A6;
        DWORD address3 = 0x45EC63;
        DWORD address4 = 0x46CFEB;
        if (ui->god_zw->isChecked())
        { // 如果需要植物不死
            BYTE data1[] = { 0x90, 0x90, 0x90, 0x90 };
            WriteMemory(data1, sizeof(data1), address1);
    
            BYTE data2[] = { 0x90, 0x90, 0x90 };
            WriteMemory(data2, sizeof(data2), address2);
    
            BYTE data3[] = { 0x90, 0x90, 0x90, 0x90 };
            WriteMemory(data3, sizeof(data3), address3);
    
            BYTE data4[] = { 0x90, 0x90, 0x90 };
            WriteMemory(data4, sizeof(data4), address4);
        }
        else
        { // 如果不需要植物不死
            BYTE data1[] = { 0x83, 0x46, 0x40, 0xFC };
            WriteMemory(data1, sizeof(data1), address1);
    
            BYTE data2[] = { 0x29, 0x4E, 0X40 };
            WriteMemory(data2, sizeof(data2), address2);
    
            BYTE data3[] = { 0x83, 0x46, 0x40, 0xCE };
            WriteMemory(data3, sizeof(data3), address3);
    
            BYTE data4[] = { 0x29, 0x50, 0x40 };
            WriteMemory(data4, sizeof(data4), address4);
        }
}

//后台运行
void MainWindow::Runing()
{
    DWORD address = 0x54E1C2;
        if (ui->runing->isChecked())
        { // 如果需要后台运行
            BYTE data[] = { 0x90, 0x90, 0x90 };
            WriteMemory(data, sizeof(data), address);
        }
        else
        { // 如果不需要后台运行
            BYTE data[] = { 0x0F, 0x95, 0xC0 };
            WriteMemory(data, sizeof(data), address);
        }
}

//僵尸无敌
void MainWindow::GodJs()
{
    DWORD address1 = 0x53130F;
	DWORD address2 = 0x531042;
	DWORD address3 = 0x530C9F;
	if (ui->god_js->isChecked())
	{ // 如果需要僵尸不死
	  // 去除秒杀僵尸
		ui->kill_js->setChecked(false);
        KillJs();

		BYTE data1[] = { 0x83, 0xEF, 0x00, 0x90 };
		WriteMemory(data1, sizeof(data1), address1);

		BYTE data2[] = { 0x90, 0x90, 0x90, 0x90 };
		WriteMemory(data2, sizeof(data2), address2);

		BYTE data3[] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
		WriteMemory(data3, sizeof(data3), address3, -1);
	}
	else
	{ // 如果不需要僵尸不死
		BYTE data1[] = { 0x2B, 0x7C, 0x24, 0x20 };
		WriteMemory(data1, sizeof(data1), address1);

		BYTE data2[] = { 0x2B, 0xF0, 0x2B, 0xC8 };
		WriteMemory(data2, sizeof(data2), address2);

		BYTE data3[] = { 0x2B, 0xD0, 0x29, 0x86, 0xDC, 0x00, 0x00, 0x00 };
		WriteMemory(data3, sizeof(data3), address3);
	}
}

//无限金钱
void MainWindow::MaxMoney()
{
    DWORD value = 99990;
    WriteMemory(&value, sizeof(value), 0x6A9EC0, 0x82C, 0x28, -1);
}

//一键取消
void MainWindow::OffAll()
{
    //取消无限阳光
    ui->max_sun->setChecked(false);
    
    //取消秒杀僵尸
    ui->kill_js->setChecked(false);
    KillJs();
    
    //取消禁用CD
    ui->ban_cd->setChecked(false);
    BanCd();
    
    //取消植物无敌
    ui->god_zw->setChecked(false);
    GodZw();

    
    //取消后台运行
    ui->runing->setChecked(false);
    Runing();

    
    //取消僵尸无敌
    ui->god_js->setChecked(false);
    GodJs();
    
}

//联系作者
void MainWindow::Chat()
{
    QDesktopServices::openUrl(QUrl("http://wpa.qq.com/msgrd?v=3&uin=3606264624&site=qq&menu=yes"));
}

//关闭程序
void MainWindow::closeEvent(QCloseEvent *event)
{
    // 去掉外挂效果
	OffAll();

	// 杀死线程
	::TerminateThread(g_monitoringThreadHandle, 0);
	::CloseHandle(g_monitoringThreadHandle);

	// 关闭句柄
	::CloseHandle(g_processHandle);
}

