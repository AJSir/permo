// permoDlg.h : ͷ�ļ�
//

#pragma once
#include "MFNetTraffic.h"
#include "InfoDlg.h"

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
	void InitSize();								//��ʼ�����ڴ�С��λ��
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);	
	void DrawBackground(CDC* pDC);							//���ƽ��汳��
	void DrawInfo(CDC* pDC);								//���ƽ�����Ϣ
	// CPUռ��
	unsigned int nCPU;
	// �ڴ�ռ��
	unsigned int nMem;
	unsigned int nTrans;									//͸����Ĭ��255
	double fNetUp;											//�ϴ��ٶ�
	double fNetDown;										//�����ٶ�

	FILETIME preidleTime;
	FILETIME prekernelTime;
	FILETIME preuserTime;

	FILETIME idleTime;
	FILETIME kernelTime;
	FILETIME userTime;

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnTopmost();
	BOOL bTopmost;
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	CMenu m_Menu;				//�Ҽ������˵�
	CMenu m_SubMenu_Skin;			//�Ҽ����������˵�(Ƥ���������˵�)
	CMenu m_SubMenu_NetPort;
	CMenu m_SubMenu_ShowWay;		//��ϸ������ʾ��ʽĬ��0����ƹ���ʾ1������ʾ
	CMenu m_SubMenu_Trans;
	CMenu m_SubMenu_FontSize;
	afx_msg void OnGreen();
	afx_msg void OnBlue();
	afx_msg void OnBlack();
	afx_msg void OnRed();
	afx_msg void OnOrange();
	MFNetTraffic m_cTrafficClassUp;
	MFNetTraffic m_cTrafficClassDown;
	BOOL isOnline;
	// �������Ĵ�С����ȥ��������
	RECT rWorkArea;
	// ���ڵ�ǰλ��
	CRect rCurPos;
	afx_msg void OnExit();
	void InitPopMenu(int nCount);
	//BOOL GetNetStatus();
	void OpenConfig();
	BOOL SaveConfig();
//	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	BOOL bAutoHide;
	void OnAutoHide(void);
	void OnTrans0(void);
	void OnTrans10(void);
	void OnTrans20(void);
	void OnTrans30(void);
	void OnTrans40(void);
	void OnTrans50(void);
	void OnTrans60(void);
	void OnTrans70(void);
	void OnTrans80(void);
	void OnTrans90(void);
	void SetAutoRun(void);
	void IfAutoRun(void);
	CInfoDlg *pInfoDlg;
	void CreateInfoDlg(void);
	void MoveInfoDlg(void);
	BOOL SetWorkDir(void);
	unsigned int nShowWay; //���鴰�ڵ�����ʽ0Ĭ����ͣ������1��������������
	void SetShowWay(void);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
//	afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	BOOL _bMouseTrack;
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void ShowNetInfo(void);
	void SetFontSize(UINT fontSize);
};
