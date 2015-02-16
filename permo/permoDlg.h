
// permoDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "MFNetTraffic.h"
#include "MenuEx.h"


// CpermoDlg �Ի���
class CpermoDlg : public CDialog
{
// ����
public:
	CpermoDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_PERMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	void InitSize(BOOL bInit);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	void DrawBackground(CDC* pDC);
	void DrawInfo(CDC* pDC);
	// CPUռ��
	unsigned int nCPU;
	// �ڴ�ռ��
	unsigned int nMem;
	double fNetUp;
	double fNetDown;

	FILETIME preidleTime;
	FILETIME prekernelTime;
	FILETIME preuserTime;

	FILETIME idleTime;
	FILETIME kernelTime;
	FILETIME userTime;

	MEMORYSTATUSEX memStatex;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnTopmost();
	BOOL bTopmost;
//	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	CMenuEx m_Menu;
	CMenuEx m_SubMenu;
	unsigned int nSkin;
	afx_msg void OnGreen();
	afx_msg void OnBlue();
	afx_msg void OnBlack();
	afx_msg void OnRed();
	afx_msg void OnOrange();
	MFNetTraffic m_cTrafficClassUp;
	MFNetTraffic m_cTrafficClassDown;
	// �ܵ��ϴ�����
	DWORD total_net_up;
	// �ܵ���������
	DWORD total_net_down;
	BOOL isOnline;
	// �������Ĵ�С����ȥ��������
	RECT rWorkArea;
	afx_msg void OnMove(int x, int y);
	// ���ڵ�ǰλ��
	RECT rCurPos;
	afx_msg void OnExit();
	void InitPopMenu();
	MENUITEM mi1;
	MENUITEM mi2;
	MENUITEM mi3;
	MENUITEM mi4;
	MENUITEM mi5;
	MENUITEM mi6;
	MENUITEM mi7;
	MENUITEM mi8;
	BOOL GetNetStatus();
	BOOL OpenConfig();
	BOOL SaveConfig();
};
