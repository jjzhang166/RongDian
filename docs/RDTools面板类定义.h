RDTools面板类定义

相关基类文件ITools.h

RDTools面板使用统一方式定义，所有面板类统一继承于CBaseTool

定义一个子面板类时，统一使用宏定义RD_DECLARE_BEGIN、RD_DECLARE_END定义
这两个宏定义会声明一个面板类，并会声明子面板必须要实现的基类成员方法
		BOOL IsCanQuit(HWND hWnd);
		void OnQuit();
		BOOL OnInit(WPARAM wParam, LPARAM lParam);
		BOOL SetLang(CPaintManagerUI* pManager, LPCWSTR lpszLang);
以上四个成员方法是子面板类必须实现的，其中
IsCanQuit是主面板退出时，由主面板主动调用，查询当前子面板是否可以退出，只有当所有子面板都返回TRUE时，主面板才可以退出
OnQuit是用于资源释放，由于目前子面板的管理方式较为特殊，子面板实例使用全局定义，所以子面板的析构函数可能会比主窗体的析构慢(也就是主窗口已经执行完自己的析构函数了，但是由于子面板实例为全局定义，此时子面板的析构函数可能还没有调用，而子面板的资源却又从属与主窗体的)，因此，为了释放子面板的资源，这里增加一个接口，由主窗体资源释放时主动调用，主动清理子面板所申请的资源
OnInit是子面板初始时由主窗体主动调用，子面板的初始化均在此初始化
SetLang是语言接口，设置语言信息时，在此成员函数设置

添加面板的消息处理，以及自定义的成员函数，成员变量，均使用RD_DECLARE_MEMBER宏进行添加
目前支持的消息处理有：
OnCreateControl：对应创建自定义控件
OnClick：对应控件点击事件
OnItemActive：对应List子节点双击事件
OnItemClick：对应List子节点点击事件
OnItemSelected：对应List(Combo)子节点选择事件
OnTextChanged：对应Edit文本内容改变事件
GetItemText：对应List自定义文本显示事件(多列List时会用到此事件)
HandleCustomMessage：对应自定义WINDOWS消息事件
OnCopyData：对应WM_COPYDATA消息事件

例：
定义一个Test子面板类：见Test.h、Test.cpp两个文件

添加到主窗体
在MainFrame.cpp中，
添加子面板头文件引用
#include "Test.h"

然后查找
RD_BEGIN_TOOLS_MAP(CMainFrame)
	RD_TOOL(L"ColorPicker", ColorPicker) // 对应CColorPicker类
	...
	RD_TOOL(L"Test", Test) // 第一个参数为子面板的描述名(目前还没使用)，第二个参数为子面板类名，跟使用RD_DECLARE_BEGIN(Test)宏声明子面板类时的参数一致
	RD_TOOL(L"About", About) // 对应CAbout类
RD_END_TOOLS_MAP()
在这两个宏中间添加子面板关联

添加顺序为主窗口进行消息分发时的先后顺序