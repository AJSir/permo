// InfoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "permo.h"
#include "InfoDlg.h"

extern int nTempDisk;		//Ӳ���¶�
extern int nTempCpu;
extern BOOL gIsMsr;
extern unsigned int nSkin;

extern vector<CProInfo*> vecProInfo;

extern vector<CProInfo*> vecCpu;
extern vector<CProInfo*> vecMem;
extern vector<CProInfo*> vecNet;

extern bool bShowNetInfo;
extern unsigned int nFontSize;
extern int processor_count_;
DWORD id;
// CInfoDlg �Ի���

bool comT(CProInfo* pProInfo)
{
	if (pProInfo->id == id)
	{
		return true;
	}
	return false;
}

//�Զ���������
//ע�⣺�������Ĳ���������һ��Ҫ��vector��Ԫ�ص�����һ��
bool SortByCpu(const CProInfo * v1, const CProInfo * v2)
{
	return v1->cpu > v2->cpu;//��������
}

bool SortByMem(const CProInfo * v1, const CProInfo * v2)
{
	return v1->mem > v2->mem;//��������
}

bool SortByNet(const CProInfo * v1, const CProInfo * v2)
{
	return (v1->prevTxRate + v1->prevRxRate) > (v2->prevTxRate + v2->prevRxRate);//��������
}

IMPLEMENT_DYNAMIC(CInfoDlg, CDialog)

CInfoDlg::CInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInfoDlg::IDD, pParent)
	, m_pParent(NULL)
	, nProNum(0)
{
	m_pParent = pParent;
}

CInfoDlg::~CInfoDlg()
{
	FreeVec();
}

void CInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CInfoDlg, CDialog)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CInfoDlg ��Ϣ�������

void CInfoDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (1 == nIDEvent)
	{
		//���������ڴ����
		vecCpu.clear();
		vecMem.clear();
		vecNet.clear();
		GetProInfoVec();
		Invalidate(FALSE);
	}

	CDialog::OnTimer(nIDEvent);
}

void CInfoDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
	RECT rcClient;
	this->GetClientRect(&rcClient);
	CDC MemDC;
	CBitmap bitmap;
	BITMAP  m_bitmap;
	bitmap.LoadBitmap(IDB_PROINFO_BACK);
	bitmap.GetBitmap(&m_bitmap);
	MemDC.CreateCompatibleDC(&dc);
	MemDC.SelectObject(&bitmap);
	DrawInfo(&MemDC);
	dc.BitBlt(0, 0, m_bitmap.bmWidth,m_bitmap.bmHeight, &MemDC,
		0, 0, SRCCOPY);
	bitmap.DeleteObject();
	MemDC.DeleteDC();
}

BOOL CInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	nTempDisk = 0;
	//����Ȩ��
	DebugPrivilege(true);

	GetProInfoVec();
	SetTimer(1, 1100, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CInfoDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
	CDialog::OnLButtonDown(nFlags, point);
}


void CInfoDlg::PostNcDestroy()
{
	// TODO: �ڴ����ר�ô����/����û���

	CDialog::PostNcDestroy();
	delete this;
	CoUninitialize();
	//Ȩ�޻�ԭ
	DebugPrivilege(false);
}

void CInfoDlg::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���
//��ģ̬�Ի�����Ҫ���غ���OnCanel����������������е���DestroyWindow�����Ҳ��ܵ��û����OnCancel����Ϊ�����OnCancel������EndDialog���������������������ģ̬�Ի���ġ� ���OnOk��Ҫ�رնԻ���ҲҪ�������أ��������ơ�
	DestroyWindow();
}

void CInfoDlg::DrawInfo(CDC * pDC)
{
// 	CRect rcIcon;
// 	rcIcon.left = 0;
// 	rcIcon.top = 0;
// 	rcIcon.right = 20;
// 	rcIcon.bottom = 20;
// 	rcIcon.DeflateRect(2, 2, 2, 2);
	//DrawIconEx(pDC->GetSafeHdc(), rcIcon.left, rcIcon.top, LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME)), rcIcon.Width(), rcIcon.Height(), 0, NULL, DI_NORMAL);
	int i=0;
	CFont font, *pOldFont;
	LOGFONT logFont;
	pDC->GetCurrentFont()->GetLogFont(&logFont);
	logFont.lfWidth = 0;
	logFont.lfHeight = nFontSize;
	logFont.lfWeight = 0;
	lstrcpy(logFont.lfFaceName, _T("΢���ź�"));
	font.CreateFontIndirect(&logFont);
	pOldFont = pDC->SelectObject(&font);
	COLORREF cOldTextColor;
	cOldTextColor = pDC->SetTextColor(RGB(255, 255, 255));
	int nOldBkMode = pDC->SetBkMode(TRANSPARENT);
	//�����������ڱ���
	CString strTemp = _T("��ϸ��Ϣ������");
	CRect rcLeftText, rcRightText, rcTitleText;
	rcTitleText.left = 25;
	rcTitleText.right = 155;
	rcTitleText.top = 3;
	rcTitleText.bottom = 20;
	pDC->DrawText(strTemp, &rcTitleText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

	pDC->SetTextColor(RGB(0, 0, 0));
	//���Ʊ����·����ĸ�С����
	//MessageBox(strTemp);
	rcTitleText.top = 30;
	rcTitleText.bottom = 50;

	strTemp.Format(_T("��������: %d"), nProNum);
	
	pDC->DrawText(strTemp, &rcTitleText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	rcTitleText.left = 155 + 10;
	rcTitleText.right = 230;
	
	if (gIsMsr)
	{
		strTemp.Format(_T("CPU: %d��"), nTempCpu);
		pDC->DrawText(strTemp, &rcTitleText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	}
	else
	{
		strTemp = _T("Ԥ��");
		pDC->DrawText(strTemp, &rcTitleText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	}
	
	rcTitleText.left = 25;
	rcTitleText.right = 155;
	rcTitleText.top = 50;
	rcTitleText.bottom = 70;

	strTemp.Format(_T("Ӳ���¶�: %d��"), nTempDisk);
	pDC->DrawText(strTemp, &rcTitleText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	rcTitleText.left = 155 + 10;
	rcTitleText.right = 230;
	strTemp = _T("Ԥ��");
	pDC->DrawText(strTemp, &rcTitleText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

	//�б�����С����
	rcTitleText.left = 5;
	rcTitleText.right = 155;
	rcTitleText.top = 80;
	rcTitleText.bottom = 100;
	strTemp = _T("��������");
	pDC->DrawText(strTemp, &rcTitleText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	rcTitleText.left = 155 + 10;
	rcTitleText.right = 240;
	strTemp = _T("��Դռ��");
	pDC->DrawText(strTemp, &rcTitleText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

	//��������ռ�ý�����Ϣ
 	CRect	rcIcon;
 	rcIcon.left = 6;
 	rcIcon.top = 124;
 	rcIcon.bottom = 140;
 	rcIcon.right = 22;
// 	DrawIconEx(pDC->GetSafeHdc(), rcIcon.left, rcIcon.top, LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_PROCESS)), rcIcon.Width(), rcIcon.Height(), 0, NULL, DI_NORMAL);
// 	rcLeftText.top = 122;
// 	rcLeftText.bottom = 142;
// 	rcLeftText.left = 26;
// 	rcLeftText.right = 160;
// 	strTemp = _T("explorer.exe");
// 	pDC->DrawText(strTemp, &rcLeftText, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
// 	
// 	rcIcon.top = 144;
// 	rcIcon.bottom = 160;
// 	DrawIconEx(pDC->GetSafeHdc(), rcIcon.left, rcIcon.top, LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_PROCESS)), rcIcon.Width(), rcIcon.Height(), 0, NULL, DI_NORMAL);
// 	rcLeftText.top = 142;
// 	rcLeftText.bottom = 162;
// 	strTemp = _T("thunderplatform.exe");
// 	pDC->DrawText(strTemp, &rcLeftText, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
// 	
// 	rcIcon.top = 164;
// 	rcIcon.bottom = 180;
// 	DrawIconEx(pDC->GetSafeHdc(), rcIcon.left, rcIcon.top, LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_PROCESS)), rcIcon.Width(), rcIcon.Height(), 0, NULL, DI_NORMAL);
// 	rcLeftText.top = 162;
// 	rcLeftText.bottom = 182;
// 	strTemp = _T("QQ.exe");
// 	pDC->DrawText(strTemp, &rcLeftText, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);

	//����CPUռ�ý�����Ϣ
	if (bShowNetInfo)
	{
		//���Ȼ�����������
		rcTitleText.left = 5;
		rcTitleText.right = 200;
		rcTitleText.top = 100;
		rcTitleText.bottom = 120;

		strTemp = _T("��ǰ��ռ����ĳ���");
		pDC->DrawText(strTemp, &rcTitleText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

		rcTitleText.top += 20*4;
		rcTitleText.bottom = rcTitleText.top + 20;
		strTemp = _T("��ǰ��ռCPU�ĳ���");
		pDC->DrawText(strTemp, &rcTitleText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

		rcTitleText.top += 20*4;
		rcTitleText.bottom = rcTitleText.top + 20;
		strTemp = _T("��ǰ��ռ�ڴ�ĳ���");
		pDC->DrawText(strTemp, &rcTitleText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

		//���Ż���ÿ���������
		pDC->SetTextColor(RGB(80, 80, 80));

		rcLeftText.left = 30;
		rcLeftText.right = 155;
		rcLeftText.top = 120;
		rcLeftText.bottom = 140;
		for (i=0; i<3; i++)
		{
			rcIcon.top = rcLeftText.top + 2;
			rcIcon.bottom = rcLeftText.bottom -2;
			if (vecNet[i]->hIcon)
			{
				DrawIconEx(pDC->GetSafeHdc(), rcIcon.left, rcIcon.top, vecNet[i]->hIcon, rcIcon.Width(), rcIcon.Height(), 0, NULL, DI_NORMAL);
			}
			else
			{
				DrawIconEx(pDC->GetSafeHdc(), rcIcon.left, rcIcon.top, LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_PROCESS)), rcIcon.Width(), rcIcon.Height(), 0, NULL, DI_NORMAL);
			}
			strTemp.Format(_T("%s"), vecNet[i]->szExeFile);
			pDC->DrawText(strTemp, &rcLeftText, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
			rcRightText = rcLeftText;
			rcRightText.left = 165;
			rcRightText.right = 240;
			double net;
			net = (vecNet[i]->prevRxRate + vecNet[i]->prevTxRate) / 1024.0;
			if (net > 1000)
			{
				strTemp.Format(_T("%.1fM/s"), net/1024);
			}
			else
			{
				strTemp.Format(_T("%.1fK/s"), net);
			}
			pDC->DrawText(strTemp, &rcRightText, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
			rcLeftText.top += 20;
			rcLeftText.bottom += 20;
		}

		rcLeftText.left = 30;
		rcLeftText.right = 155;
		rcLeftText.top = 200;
		rcLeftText.bottom = 220;

		for(i=0;i<3;i++)
		{
			rcIcon.top = rcLeftText.top + 2;
			rcIcon.bottom = rcLeftText.bottom -2;
			if (vecCpu[i]->hIcon)
			{
				DrawIconEx(pDC->GetSafeHdc(), rcIcon.left, rcIcon.top, vecCpu[i]->hIcon, rcIcon.Width(), rcIcon.Height(), 0, NULL, DI_NORMAL);
			}
			else
			{
				DrawIconEx(pDC->GetSafeHdc(), rcIcon.left, rcIcon.top, LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_PROCESS)), rcIcon.Width(), rcIcon.Height(), 0, NULL, DI_NORMAL);
			}
			strTemp.Format(_T("%s"), vecCpu[i]->szExeFile);
			pDC->DrawText(strTemp, &rcLeftText, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
			rcRightText = rcLeftText;
			rcRightText.left = 165;
			rcRightText.right = 240;
			strTemp.Format(_T("%d%%"), vecCpu[i]->cpu);
			pDC->DrawText(strTemp, &rcRightText, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
			rcLeftText.top += 20;
			rcLeftText.bottom += 20;
		}
		rcLeftText.top += 20;
		rcLeftText.bottom += 20;
		//�����ڴ�ռ�ý�����Ϣ
		for(i=0; i<3; i++)
		{
			rcIcon.top = rcLeftText.top + 2;
			rcIcon.bottom = rcLeftText.bottom -2;
			if (vecMem[i]->hIcon)
			{
				DrawIconEx(pDC->GetSafeHdc(), rcIcon.left, rcIcon.top, vecMem[i]->hIcon, rcIcon.Width(), rcIcon.Height(), 0, NULL, DI_NORMAL);
			}
			else
			{
				DrawIconEx(pDC->GetSafeHdc(), rcIcon.left, rcIcon.top, LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_PROCESS)), rcIcon.Width(), rcIcon.Height(), 0, NULL, DI_NORMAL);
			}
			strTemp.Format(_T("%s"), vecMem[i]->szExeFile);
			pDC->DrawText(strTemp, &rcLeftText, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
			rcRightText = rcLeftText;
			rcRightText.left = 165;
			rcRightText.right = 240;
			if (vecMem[i]->mem < 1000)
			{
				strTemp.Format(_T("%.1fMB"), vecMem[i]->mem);
			}
			else
			{
				strTemp.Format(_T("%.1fGB"), vecMem[i]->mem / 1024.0);
			}
			pDC->DrawText(strTemp, &rcRightText, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
			rcLeftText.top += 20;
			rcLeftText.bottom += 20;
		}
	}
	else
	{
		rcTitleText.left = 5;
		rcTitleText.right = 200;
		rcTitleText.top = 100;
		rcTitleText.bottom = 120;

		strTemp = _T("��ǰ��ռCPU�ĳ���");
		pDC->DrawText(strTemp, &rcTitleText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

		rcTitleText.top += 20*6;
		rcTitleText.bottom = rcTitleText.top + 20;
		strTemp = _T("��ǰ��ռ�ڴ�ĳ���");
		pDC->DrawText(strTemp, &rcTitleText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

		//���Ż���ÿ���������
		pDC->SetTextColor(RGB(80, 80, 80));

		rcLeftText.left = 30;
		rcLeftText.right = 155;
		rcLeftText.top = 120;
		rcLeftText.bottom = 140;
		//����CPUռ����Ϣ
		vector<CProInfo*>::iterator iter;
		for(iter=vecCpu.begin();iter!=vecCpu.end();iter++)
		{
			rcIcon.top = rcLeftText.top + 2;
			rcIcon.bottom = rcLeftText.bottom -2;
			if ((*iter)->hIcon)
			{
				DrawIconEx(pDC->GetSafeHdc(), rcIcon.left, rcIcon.top, (*iter)->hIcon, rcIcon.Width(), rcIcon.Height(), 0, NULL, DI_NORMAL);
			}
			else
			{
				DrawIconEx(pDC->GetSafeHdc(), rcIcon.left, rcIcon.top, LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_PROCESS)), rcIcon.Width(), rcIcon.Height(), 0, NULL, DI_NORMAL);
			}
			strTemp.Format(_T("%s"), (*iter)->szExeFile);
			pDC->DrawText(strTemp, &rcLeftText, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
			rcRightText = rcLeftText;
			rcRightText.left = 165;
			rcRightText.right = 240;
			strTemp.Format(_T("%d%%"), (*iter)->cpu);
			pDC->DrawText(strTemp, &rcRightText, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
			rcLeftText.top += 20;
			rcLeftText.bottom += 20;
		}
		rcLeftText.top += 20;
		rcLeftText.bottom += 20;
		//�����ڴ�ռ�ý�����Ϣ
		vector<CProInfo*>::iterator iter2;
		for(iter2=vecMem.begin();iter2!=vecMem.end();iter2++)
		{
			rcIcon.top = rcLeftText.top + 2;
			rcIcon.bottom = rcLeftText.bottom -2;
			if ((*iter2)->hIcon)
			{
				DrawIconEx(pDC->GetSafeHdc(), rcIcon.left, rcIcon.top, (*iter2)->hIcon, rcIcon.Width(), rcIcon.Height(), 0, NULL, DI_NORMAL);
			}
			else
			{
				DrawIconEx(pDC->GetSafeHdc(), rcIcon.left, rcIcon.top, LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_PROCESS)), rcIcon.Width(), rcIcon.Height(), 0, NULL, DI_NORMAL);
			}
			strTemp.Format(_T("%s"), (*iter2)->szExeFile);
			pDC->DrawText(strTemp, &rcLeftText, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
			rcRightText = rcLeftText;
			rcRightText.left = 165;
			rcRightText.right = 240;
			if ((*iter2)->mem < 1000)
			{
				strTemp.Format(_T("%.1fMB"), (*iter2)->mem);
			}
			else
			{
				strTemp.Format(_T("%.1fGB"), (*iter2)->mem / 1024.0);
			}
			pDC->DrawText(strTemp, &rcRightText, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
			rcLeftText.top += 20;
			rcLeftText.bottom += 20;
		}
	}
	
	pDC->SetTextColor(cOldTextColor);
	pDC->SetBkMode(nOldBkMode);
	pDC->SelectObject(pOldFont);
	font.DeleteObject();
}

// ����Ȩ��
bool CInfoDlg::DebugPrivilege(bool bEnable)
{
	bool              bResult = true;
	HANDLE            hToken;
	TOKEN_PRIVILEGES  TokenPrivileges;

	if(OpenProcessToken(GetCurrentProcess(),TOKEN_QUERY | TOKEN_ADJUST_PRIVILEGES,&hToken) == 0)
	{
		//printf("OpenProcessToken Error: %d\n",GetLastError());
		bResult = FALSE;
	}
	TokenPrivileges.PrivilegeCount           = 1;
	TokenPrivileges.Privileges[0].Attributes = bEnable ? SE_PRIVILEGE_ENABLED : 0;
	LookupPrivilegeValue(NULL,SE_DEBUG_NAME,&TokenPrivileges.Privileges[0].Luid);
	AdjustTokenPrivileges(hToken,FALSE,&TokenPrivileges,sizeof(TOKEN_PRIVILEGES),NULL,NULL);
	if(GetLastError() != ERROR_SUCCESS)
	{
		bResult = false;
	}
	CloseHandle(hToken);

	return bResult;
}

//ʱ��ת��
uint64_t CInfoDlg::file_time_2_utc(const FILETIME* ftime)
{
	LARGE_INTEGER li;  

	li.LowPart = ftime->dwLowDateTime;  
	li.HighPart = ftime->dwHighDateTime;  
	return li.QuadPart;
}

double CInfoDlg::get_cpu_usage(HANDLE hProcess, CProInfo* pProInfo)
{
	FILETIME now;  
	FILETIME creation_time;  
	FILETIME exit_time;  
	FILETIME kernel_time;  
	FILETIME user_time;  
	int64_t system_time;  
	int64_t time;  
	int64_t system_time_delta;  
	int64_t time_delta;  

	int cpu = 0;  

	GetSystemTimeAsFileTime(&now);  

	if (!GetProcessTimes(hProcess, &creation_time, &exit_time,  
		&kernel_time, &user_time))  
	{  
		// We don't assert here because in some cases (such as in the Task   

		//Manager)  
		// we may call this function on a process that has just exited but   

		//we have  
		// not yet received the notification.  
		return 0;  
	}  
	system_time = (file_time_2_utc(&kernel_time) + file_time_2_utc(&user_time))  /  processor_count_;  
	time = file_time_2_utc(&now);  

	if ((pProInfo->last_system_time_ == 0) || (pProInfo->last_time_ == 0))  
	{  
		// First call, just set the last values.  
		pProInfo->last_system_time_ = system_time;  
		pProInfo->last_time_ = time;  
		pProInfo->cpu = 0;
		return 0;  
	}  

	system_time_delta = system_time - pProInfo->last_system_time_;  
	time_delta = time - pProInfo->last_time_;  

	if (time_delta == 0)  
	{
		pProInfo->cpu = 0;
		return 0;  
	}

	// We add time_delta / 2 so the result is rounded.  
	cpu = (int)((system_time_delta * 100 + time_delta / 2) / time_delta);  
	pProInfo->last_system_time_ = system_time;  
	pProInfo->last_time_ = time;  
	pProInfo->cpu = cpu;
	return cpu; 
}

//��ȡ������Ϣ������vector
void CInfoDlg::GetProInfoVec(void)
{
	//WinExec("cmd /c tasklist /v >d:\\tasklist.txt",SW_HIDE);
	PROCESSENTRY32 pe32;
	//���ô�С
	pe32.dwSize=sizeof(pe32);
	//����ϵͳ���̿���
	HANDLE hProcessSnap=::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	if(hProcessSnap==INVALID_HANDLE_VALUE)
	{
		//��ȡϵͳ���̿���ʧ��
		return;
	}
	nProNum = 0;
	//��������
	BOOL bMORE=::Process32First(hProcessSnap,&pe32);
	while(bMORE)
	{
		nProNum++;
		id = pe32.th32ProcessID;
		//ȡ�ý��̵��ڴ�ʹ����Ϣ
		HANDLE hProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,FALSE, pe32.th32ProcessID);
		if (NULL == hProcess)
		{
			//System��System idle����һ����ʧ�ܣ���ʱ�����⼸��ϵͳ����
			bMORE=::Process32Next(hProcessSnap,&pe32);
			continue;
		}
		else
		{
			PROCESS_MEMORY_COUNTERS processMemCounters ;
			if( ::GetProcessMemoryInfo (hProcess, &processMemCounters, sizeof(processMemCounters) ) )
			{
				double memtmp = processMemCounters.WorkingSetSize/(1024.0*1024.0);

				vector<CProInfo*>::iterator iter = find_if(vecProInfo.begin(),vecProInfo.end(),&comT);
				if(iter!=vecProInfo.end())
				{
					//�ҵ��ˣ�����Ҫ���µĽ���
					get_cpu_usage(hProcess, (*iter));
					(*iter)->mem = memtmp;
					(*iter)->bExit = FALSE;
					//��ǰ���̵��ڴ�ʹ������λ��byte���ýṹ���������Ա��������ο�MSDN
				}
				else
				{
					//�Ҳ���������Ҫ�����Ľ���
					CProInfo *p = new CProInfo();
					p->id = pe32.th32ProcessID;
					p->mem = memtmp;
					p->bExit = FALSE;
					wcscpy(p->szExeFile, pe32.szExeFile);
					GetProcessFullPath(hProcess, p->szExeFilePath);
					if (!p->hIcon)
					{
						ExtractIconEx(p->szExeFilePath, 0, NULL, &(p->hIcon), 1);
					}
					//p->szExeFile = pe32.szExeFile;
					vecProInfo.push_back(p);
				}
			}
			else
			{
				//GetProcessMemoryInfo failed.
				GetLastError();
			}
		}
		bMORE=::Process32Next(hProcessSnap,&pe32);
	}
	//ȥ���Ѿ��˳��Ľ��̣�����bExitΪTRUE
	RemoveExitPro();
	//�Ѿ���ȡ�����н�����Ϣ
	//����cpuռ�ý������򣨽���
	sort(vecProInfo.begin(),vecProInfo.end(),SortByCpu);
	vecCpu.push_back(vecProInfo[0]);
	vecCpu.push_back(vecProInfo[1]);
	vecCpu.push_back(vecProInfo[2]);
	vecCpu.push_back(vecProInfo[3]);
	vecCpu.push_back(vecProInfo[4]);
	//���ڴ����򣨽���
	sort(vecProInfo.begin(),vecProInfo.end(),SortByMem);
	vecMem.push_back(vecProInfo[0]);
	vecMem.push_back(vecProInfo[1]);
	vecMem.push_back(vecProInfo[2]);
	vecMem.push_back(vecProInfo[3]);
	vecMem.push_back(vecProInfo[4]);
	//���������򣨽���
	sort(vecProInfo.begin(),vecProInfo.end(),SortByNet);
	vecNet.push_back(vecProInfo[0]);
	vecNet.push_back(vecProInfo[1]);
	vecNet.push_back(vecProInfo[2]);
	vecNet.push_back(vecProInfo[3]);
	vecNet.push_back(vecProInfo[4]);
	//vector<CProInfo*>::iterator iter;
	//for(iter=vecProInfo.begin();iter!=vecProInfo.end();iter++)
	//{
	//	printf("��������%s ID:%d MEM:%.1f CPU:%d\n", (*iter)->szExeFile, (*iter)->id, (*iter)->mem, (*iter)->cpu);
	//}  
	CloseHandle(hProcessSnap);
}

// �����ڴ�
void CInfoDlg::FreeVec(void)
{
	//���������ڴ����
	vector<CProInfo*>::iterator iter;
	for(iter=vecProInfo.begin();iter!=vecProInfo.end();iter++)
	{  
		if ((*iter) != NULL)
		{
			delete *iter;  
		}
	}
}

//ɾ���Ѿ��˳��Ľ���
void CInfoDlg::RemoveExitPro(void)
{
	vector<CProInfo*>::iterator iter;
	for(iter=vecProInfo.begin();iter!=vecProInfo.end();)
	{  
		if ((*iter)->bExit)
		{
			delete *iter;
			iter = vecProInfo.erase(iter);
		}
		else
		{
			(*iter)->bExit = TRUE;
			iter++;
		}
	}
}

// ��ȡ��������·�� 
BOOL CInfoDlg::GetProcessFullPath(HANDLE hProcess, TCHAR * pszFullPath)
{
	TCHAR       szImagePath[MAX_PATH];  

	if(!pszFullPath)  
		return FALSE;  

	pszFullPath[0] = '\0';  
	if(!hProcess)  
		return FALSE;  

	if(!GetProcessImageFileName(hProcess, szImagePath, MAX_PATH))  
	{  
		CloseHandle(hProcess);  
		return FALSE;  
	}  

	if(!DosPathToNtPath(szImagePath, pszFullPath))  
	{  
		CloseHandle(hProcess);  
		return FALSE;  
	}  

	CloseHandle(hProcess);  

	return TRUE; 
}

BOOL CInfoDlg::DosPathToNtPath(LPTSTR pszDosPath, LPTSTR pszNtPath)
{
	TCHAR           szDriveStr[500];  
	TCHAR           szDrive[3];  
	TCHAR           szDevName[100];  
	INT             cchDevName;  
	INT             i;  

	//������  
	if(!pszDosPath || !pszNtPath )  
		return FALSE;  

	//��ȡ���ش����ַ���  
	if(GetLogicalDriveStrings(sizeof(szDriveStr), szDriveStr))  
	{  
		for(i = 0; szDriveStr[i]; i += 4)  
		{  
			if(!lstrcmpi(&(szDriveStr[i]), TEXT("A:\\")) || !lstrcmpi(&(szDriveStr[i]), TEXT("B:\\")))  
				continue;  

			szDrive[0] = szDriveStr[i];  
			szDrive[1] = szDriveStr[i + 1];  
			szDrive[2] = '\0';  
			if(!QueryDosDevice(szDrive, szDevName, 100))//��ѯ Dos �豸��  
				return FALSE;  

			cchDevName = lstrlen(szDevName);  
			if(_tcsnicmp(pszDosPath, szDevName, cchDevName) == 0)//����  
			{  
				lstrcpy(pszNtPath, szDrive);//����������  
				lstrcat(pszNtPath, pszDosPath + cchDevName);//����·�� 

				return TRUE;  
			}             
		}  
	}  

	lstrcpy(pszNtPath, pszDosPath);  

	return FALSE;
}

BOOL CInfoDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	return TRUE;
}