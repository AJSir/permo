// permoDlg.h : ͷ�ļ�
//

#pragma once
#include "InfoDlg.h"
#include "NProgressBar.h"
#define _WIN32_DCOM
#include <comdef.h>
#include <Wbemidl.h>
#include "MyTimer.h"
# pragma comment(lib, "wbemuuid.lib")

// CpermoDlg �Ի���
class CpermoDlg : public CDialog
{
// ����
public:
	CpermoDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CpermoDlg();

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
	afx_msg LRESULT OnBandMenu(WPARAM wparam,LPARAM lparam);
	afx_msg LRESULT OnReconnect(WPARAM wparam,LPARAM lparam);
	DECLARE_MESSAGE_MAP()

public:
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
	bool bInfoDlgShowing;									//��ϸ���ڵ�ǰ�Ƿ�����ʾ

	bool bShowBand;			//�Ƿ���ʾ���

	CString strNetUp;
	CString strNetDown;

	CString strCPU;
	CString strMem;

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
	CMenu m_SubMenu_ShowWay;		//��ϸ������ʾ��ʽĬ��0����ƹ���ʾ1������ʾ2����ʾ
	CMenu m_SubMenu_Trans;
	CMenu m_SubMenu_FontSize;
	afx_msg void OnGreen();
	afx_msg void OnBlue();
	afx_msg void OnBlack();
	afx_msg void OnRed();
	afx_msg void OnOrange();
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
	unsigned int nShowWay; //���鴰�ڵ�����ʽ0Ĭ����ͣ������1��������������2Ϊ������
	void SetShowWay(int nIndex);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
//	afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	BOOL _bMouseTrack;
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void ShowNetInfo(void);
	void SetFontSize(UINT fontSize);

private:
	CNProgressBar *pcoControl;
public:
	void ShowBand(void);
	// ��ʾ��������������������е��ã�
	void ShowHideWindow(void);
	void SetBandFontSize(int nBandFontSize);
	//��������ִ�С
	int nBandFontSize;
	bool bLockWndPos;
	unsigned int nBandWidth;
	unsigned int nBandHeight;
	void SetBandWidth(unsigned int bandwidth);
	void SetBandHeight(unsigned int bandheight);
	bool IsForegroundFullscreen(void);
	bool bIsWndVisable;	//��¼���ڵ�ǰ�Ƿ��ڿɼ�״̬
	bool bFullScreen;	//��¼�Ƿ���ȫ�������
	bool bHideWndSides;	//����������������Ϣ������ʾ�м��������Ϣ
	vector<int> vBandShow;
	bool IfExist(int nVal);
	void RemoveBandShow(int nVal);
	unsigned int nNowBandShowIndex;
	bool bBandShowCpu;
	bool bBandShowMem;
	bool bBandShowNetUp;
	bool bBandShowNetDown;
	bool bBandShowDiskTem;
	bool bBandShowCpuTem;
	unsigned int nCount;

	HRESULT hres;
	IWbemLocator *pLoc;
	IWbemServices *pSvc;
	IEnumWbemClassObject* pEnumerator;
	void GetDiskTem(void);
	void StopCapture(void);
	void StartCapture(void);
	bool bHadWinpcap;
	void DeleteFiles(void);
	// win7�Լ�����ϵͳ�����������ͨ������ƻ�
	void AddWin7SchTasks(void);
	void DelWin7SchTasks(void);
	bool bIsWindowsVistaOrGreater;
	bool bShowOneSideInfo;
	bool IsIntel(void);
	void GetCpuTemp(void);
	DWORD GetCpuInfo(DWORD veax);
	void get_processor_number(void);
	CHighResolutionTimer mm_Timer;
	static void TimerCallbackTemp(DWORD dwUser);
	void ShowTempInfo(void);
};
