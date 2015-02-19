// permoDlg.h : ͷ�ļ�
//

#pragma once
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
	int SelectedInterface;
	void InitSize(BOOL bInit);								//��ʼ�����ڴ�С��λ��
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);	
	void DrawBackground(CDC* pDC);							//���ƽ��汳��
	void DrawInfo(CDC* pDC);								//���ƽ�����Ϣ
	// CPUռ��
	unsigned int nCPU;
	// �ڴ�ռ��
	unsigned int nMem;
	double fNetUp;											//�ϴ��ٶ�
	double fNetDown;										//�����ٶ�

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
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	CMenuEx m_Menu;				//�Ҽ������˵�
	CMenuEx m_SubMenu_Skin;			//�Ҽ����������˵�(Ƥ���������˵�)
	unsigned int nSkin;			//Ƥ�����
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
	void InitPopMenu(int nCount);
	MENUITEM mi1;
	MENUITEM mi2;
	MENUITEM mi3;
	MENUITEM mi4;
	MENUITEM mi5;
	MENUITEM mi6;
	MENUITEM mi7;
	MENUITEM mi8;
	MENUITEM mi9;
	//BOOL GetNetStatus();
	BOOL OpenConfig();
	BOOL SaveConfig();
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	BOOL bAutoHide;
	void OnAutoHide(void);
};
