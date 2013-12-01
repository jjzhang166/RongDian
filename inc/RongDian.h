#pragma once

// default BkColor
const DWORD			 kDefBackground = 0xFF32506E;	
const DWORD			 kDefBkColor	= 0x00FFFFFF;
const DWORD			 kTextColor		= 0xFFE0E0E0;

// App Signature
const wchar_t* const kAdminSignature			= L"RongDianTools_Admin";
const wchar_t* const kUserSignature				= L"RongDianTools_User";

// Frame Class
const wchar_t* const kRDToolsClass				= L"RDTools_Class";
const wchar_t* const kRDToolsTray_Class			= L"RDTrayIcon_Class";
const wchar_t* const kRDToolsStaticClass		= L"RDTools_Static_Class";
const wchar_t* const kSnapShotClass				= L"SnapShot_Class";
const wchar_t* const kZoomInClass				= L"ZoomIn_Class";

typedef BOOL (WINAPI *PFNChangeWindowMessageFilter)(UINT message, DWORD dwFlag);

#define WM_SNAPSHOT_MSG				WM_USER + 100 // ����ģ�齻����Ϣ
#define WM_ACTIVE_MAIN				WM_USER + 101 // ����������
#define WM_ICON_NOTIFY				WM_USER + 102 // ������Ϣ
#define WM_ZOOMIN_UPDATE_MSG		WM_USER	+ 103 // ȡɫģ�齻����Ϣ
#define WM_ZOOMIN_COLOR_MSG			WM_USER + 104 // ȥɫģ�飬֪ͨ���÷���¼��ǰ��ɫ
#define WM_ZOOMIN_QUIT_MSG			WM_USER + 105 // ֪ͨRDZoomInģ���˳�
#define WM_ZOOMIN_ZOOM_MSG			WM_USER + 106 // ֪ͨRDZoomIn�Ŵ󾵷Ŵ�
#define WM_ZOOMIN_REDUCE_MSG		WM_USER + 107 // ֪ͨRDZoomIn��С
#define WM_ZOOMIN_MOVE_UP			WM_USER + 108 // �Ŵ�����1������
#define WM_ZOOMIN_MOVE_DOWN			WM_USER + 109 // �Ŵ�����1������
#define WM_ZOOMIN_MOVE_LEFT			WM_USER + 110 // �Ŵ�����1������
#define WM_ZOOMIN_MOVE_RIGHT		WM_USER + 111 // �Ŵ�����1������
#define WM_CMD_COMPLETE				WM_USER + 112 // cmd����ɹ�ִ�н���
#define WM_VERSION_NOTIFY			WM_USER + 113 // ��ȡ���µİ汾��

typedef struct _tagSNAPSHOT_INFO
{
	DWORD dwColor;
	wchar_t szPath[1024];
} SNAPSHOT_INFO, *PSNAPSHOT_INFO, *LPSNAPSHOT_INFO;

typedef struct _tagZOOMIN_IFNO
{
	DWORD dwColor;
	POINT ptCursor;
} ZOOMIN_INFO, *PZOOMIN_INFO, *LPZOOMIN_INFO;
