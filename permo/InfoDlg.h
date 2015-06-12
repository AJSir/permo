#pragma once
#include "afxcmn.h"
#include <TlHelp32.h>
#include <Psapi.h>
#pragma comment(lib, "psapi.lib")

bool comT(CProInfo* pProInfo);
// �Զ���������
bool SortByCpu(const CProInfo * v1, const CProInfo * v2);
bool SortByMem(const CProInfo * v1, const CProInfo * v2);

// CInfoDlg �Ի���

class CInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CInfoDlg)

public:
	CInfoDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CInfoDlg();

// �Ի�������
	enum { IDD = IDD_INFO_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
//	virtual BOOL DestroyWindow();
protected:
	virtual void PostNcDestroy();
	virtual void OnCancel();
public:
	CWnd *m_pParent;
	void DrawInfo(CDC * pDC);
	
	// ����Ȩ��
	bool DebugPrivilege(bool bEnable);
	static uint64_t file_time_2_utc(const FILETIME* ftime);
	double get_cpu_usage(HANDLE hProcess, CProInfo* pProInfo);
	void GetProInfoVec(void);
	// �����ڴ�
	void FreeVec(void);
	void RemoveExitPro(void);
	// ��ȡ��������·�� 
	BOOL GetProcessFullPath(HANDLE hProcess, TCHAR * pszFullPath);
	BOOL DosPathToNtPath(LPTSTR pszDosPath, LPTSTR pszNtPath);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	// ��������
	unsigned int nProNum;

};
