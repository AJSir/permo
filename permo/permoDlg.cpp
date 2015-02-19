// permoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "permo.h"
#include "permoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CpermoDlg �Ի���
#define IDM_TOPMOST                     10001
#define IDM_GREEN                       10002
#define IDM_BLUE                        10003
#define IDM_BLACK                       10004
#define IDM_RED                         10005
#define IDM_ORANGE                      10006
#define IDM_EXIT                        10007
#define IDM_AUTOHIDE					10008

__int64 CompareFileTime(FILETIME time1, FILETIME time2)
{
	__int64 a = time1.dwHighDateTime << 32 | time1.dwLowDateTime;
	__int64 b = time2.dwHighDateTime << 32 | time2.dwLowDateTime;

	return   (b - a);
}

CpermoDlg::CpermoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CpermoDlg::IDD, pParent)
	, SelectedInterface(0)
	, nCPU(0)
	, nMem(0)
	, fNetUp(0.0)
	, fNetDown(0.0)
	, bTopmost(TRUE)
	, nSkin(0)
	, total_net_up(0)
	, total_net_down(0)
	, isOnline(FALSE)
	, bAutoHide(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CpermoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CpermoDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_COMMAND(IDM_TOPMOST, &CpermoDlg::OnTopmost)
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(IDM_GREEN, &CpermoDlg::OnGreen)
	ON_COMMAND(IDM_BLUE, &CpermoDlg::OnBlue)
	ON_COMMAND(IDM_BLACK, &CpermoDlg::OnBlack)
	ON_COMMAND(IDM_RED, &CpermoDlg::OnRed)
	ON_COMMAND(IDM_ORANGE, &CpermoDlg::OnOrange)
	ON_WM_MOVE()
	ON_COMMAND(IDM_EXIT, &CpermoDlg::OnExit)
	//}}AFX_MSG_MAP
	ON_WM_NCHITTEST()
	ON_WM_MOUSEHOVER()
	ON_WM_MOUSELEAVE()
END_MESSAGE_MAP()


// CpermoDlg ��Ϣ�������

BOOL CpermoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	::SystemParametersInfo(SPI_GETWORKAREA, 0, &rWorkArea, 0);   // ��ù�������С
	BOOL bRet = OpenConfig();
	InitSize(bRet);
	GetWindowRect(&rCurPos);
	if (!::GetSystemTimes(&preidleTime, &prekernelTime, &preuserTime))
	{
		return -1;
	}
	MFNetTraffic m_cTrafficClassTemp;
	//���£����Ӷ����з��ֵ�����ӿڼ�ر�������ֹ��صĽӿڷ���������Ľӿ�
	int nCount = m_cTrafficClassTemp.GetNetworkInterfacesCount();
	for (int i = 0; i <= nCount; i++)
	{
		if (m_cTrafficClassTemp.GetInterfaceTotalTraffic(i) > 0)
		{
			SelectedInterface = i;
			isOnline = TRUE;
			break;
		}
	}
	//�����˵�
	InitPopMenu(nCount);
	//Ĭ���ö�
	if (bTopmost)
	{
		SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		m_Menu.CheckMenuItem(IDM_TOPMOST, MF_BYCOMMAND | MF_CHECKED);
	}
	//Ĭ����ɫ
	if (!bRet)
	{
		nSkin = IDM_GREEN;
	}
	if (bAutoHide)
	{
		m_Menu.CheckMenuItem(IDM_AUTOHIDE, MF_BYCOMMAND | MF_CHECKED);
	}
	m_Menu.CheckMenuItem(nSkin, MF_BYCOMMAND | MF_CHECKED); // ��ǰ��� 

	//��������������
	m_cTrafficClassDown.SetTrafficType(MFNetTraffic::IncomingTraffic);
	m_cTrafficClassUp.SetTrafficType(MFNetTraffic::OutGoingTraffic);

	//ȡ����������ʾ
	SetWindowLong(GetSafeHwnd(), GWL_EXSTYLE, WS_EX_TOOLWINDOW);
	//ÿ��һ��ˢ�¸�����Ϣ
	SetTimer(1, 1000, NULL);


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CpermoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this);
		//�����˸����,˫�����ͼ
		//���Ե�������һ���ڴ�DC��
		RECT rcClient;
		this->GetClientRect(&rcClient);
		CDC MemDC;
		CBitmap bitmap;
		MemDC.CreateCompatibleDC(&dc);
		bitmap.CreateCompatibleBitmap(&dc, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top);
		MemDC.SelectObject(&bitmap);
		DrawBackground(&MemDC);
		DrawInfo(&MemDC);
		dc.BitBlt(0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, &MemDC,
			0, 0, SRCCOPY);
		bitmap.DeleteObject();
		MemDC.DeleteDC();
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CpermoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CpermoDlg::InitSize(BOOL bInit)
{
	if (!bInit)
	{
		rCurPos.top = rWorkArea.bottom - 20;
		rCurPos.left = rWorkArea.right - 170;
	}
	rCurPos.right = rCurPos.left + 150;
	rCurPos.bottom = rCurPos.top + 20;
	MoveWindow(&rCurPos, TRUE);
}


void CpermoDlg::DrawBackground(CDC* pDC)
{
	switch (nSkin)
	{
	case IDM_GREEN:
		{
			CPen MyPen(PS_SOLID, 1, RGB(255, 255, 255));
			CBrush RLBrush(RGB(2, 200, 20));
			CBrush MiBrush(RGB(150, 240, 150));
			CPen *pOldPen = pDC->SelectObject(&MyPen);
			CBrush *pOldBrush = pDC->SelectObject(&RLBrush);

			//������
			pDC->Rectangle(0, 0, 20, 20);
			//�Ҳ����
			pDC->Rectangle(130, 0, 150, 20);

			pDC->SelectObject(&MiBrush);
			//�м����
			pDC->Rectangle(20, 0, 130, 20);

			pDC->SelectObject(pOldPen);
			pDC->SelectObject(pOldBrush);
		}
		break;
	case IDM_BLUE:
		{
			CPen MyPen(PS_SOLID, 1, RGB(255, 255, 255));
			CBrush RLBrush(RGB(20, 80, 180));
			CBrush MiBrush(RGB(150, 150, 240));
			CPen *pOldPen = pDC->SelectObject(&MyPen);
			CBrush *pOldBrush = pDC->SelectObject(&RLBrush);

			//������
			pDC->Rectangle(0, 0, 20, 20);
			//�Ҳ����
			pDC->Rectangle(130, 0, 150, 20);

			pDC->SelectObject(&MiBrush);
			//�м����
			pDC->Rectangle(20, 0, 130, 20);

			pDC->SelectObject(pOldPen);
			pDC->SelectObject(pOldBrush);
		}
		break;
	case IDM_BLACK:
		{
			CPen MyPen(PS_SOLID, 1, RGB(255, 255, 255));
			CBrush RLBrush(RGB(50, 50, 50));
			CBrush MiBrush(RGB(100, 100, 100));
			CPen *pOldPen = pDC->SelectObject(&MyPen);
			CBrush *pOldBrush = pDC->SelectObject(&RLBrush);

			//������
			pDC->Rectangle(0, 0, 20, 20);
			//�Ҳ����
			pDC->Rectangle(130, 0, 150, 20);

			pDC->SelectObject(&MiBrush);
			//�м����
			pDC->Rectangle(20, 0, 130, 20);

			pDC->SelectObject(pOldPen);
			pDC->SelectObject(pOldBrush);
		}
		break;
	case IDM_RED:
		{
			CPen MyPen(PS_SOLID, 1, RGB(255, 255, 255));
			CBrush RLBrush(RGB(180, 20, 20));
			CBrush MiBrush(RGB(240, 150, 150));
			CPen *pOldPen = pDC->SelectObject(&MyPen);
			CBrush *pOldBrush = pDC->SelectObject(&RLBrush);

			//������
			pDC->Rectangle(0, 0, 20, 20);
			//�Ҳ����
			pDC->Rectangle(130, 0, 150, 20);

			pDC->SelectObject(&MiBrush);
			//�м����
			pDC->Rectangle(20, 0, 130, 20);

			pDC->SelectObject(pOldPen);
			pDC->SelectObject(pOldBrush);
		}
		break;
	case IDM_ORANGE:
		{
			CPen MyPen(PS_SOLID, 1, RGB(255, 255, 255));
			CBrush RLBrush(RGB(150, 100, 20));
			CBrush MiBrush(RGB(200, 180, 50));
			CPen *pOldPen = pDC->SelectObject(&MyPen);
			CBrush *pOldBrush = pDC->SelectObject(&RLBrush);

			//������
			pDC->Rectangle(0, 0, 20, 20);
			//�Ҳ����
			pDC->Rectangle(130, 0, 150, 20);

			pDC->SelectObject(&MiBrush);
			//�м����
			pDC->Rectangle(20, 0, 130, 20);

			pDC->SelectObject(pOldPen);
			pDC->SelectObject(pOldBrush);
		}
		break;
	default:
		{
			CPen MyPen(PS_SOLID, 1, RGB(255, 255, 255));
			CBrush RLBrush(RGB(2, 200, 20));
			CBrush MiBrush(RGB(150, 240, 150));
			CPen *pOldPen = pDC->SelectObject(&MyPen);
			CBrush *pOldBrush = pDC->SelectObject(&RLBrush);

			//������
			pDC->Rectangle(0, 0, 20, 20);
			//�Ҳ����
			pDC->Rectangle(130, 0, 150, 20);

			pDC->SelectObject(&MiBrush);
			//�м����
			pDC->Rectangle(20, 0, 130, 20);

			pDC->SelectObject(pOldPen);
			pDC->SelectObject(pOldBrush);
		}
		break;
	}
	//����������Բ�Ҫ,�ֲ��������������
	// 	WhitePen.DeleteObject();
	// 	GreenBrush.DeleteObject();
	// 	WhiteBrush.DeleteObject();
}

void CpermoDlg::DrawInfo(CDC* pDC)
{
	CFont font, *pOldFont;
	LOGFONT logFont;
	pDC->GetCurrentFont()->GetLogFont(&logFont);
	logFont.lfWidth = 5;
	logFont.lfHeight = 16;
	logFont.lfWeight = 0;
	lstrcpy(logFont.lfFaceName, _T("΢���ź�"));
	font.CreateFontIndirect(&logFont);
	pOldFont = pDC->SelectObject(&font);
	COLORREF cOldTextColor;
	if (IDM_GREEN == nSkin || IDM_ORANGE == nSkin)
	{
		cOldTextColor = pDC->SetTextColor(RGB(0, 0, 0));
	}
	else
	{
		cOldTextColor = pDC->SetTextColor(RGB(255, 255, 255));
	}
	int nOldBkMode = pDC->SetBkMode(TRANSPARENT);
	CString strCPU, strMem, strNetUp, strNetDown;
	strCPU.Format(_T("%d"), nCPU);
	strMem.Format(_T("%d"), nMem);
	strNetUp.Format(_T("%.1fKB/s"), fNetUp);
	strNetDown.Format(_T("%.1fKB/s"), fNetDown);
	//pDC->TextOut(1, 1, strCPU);
	CRect rText;
	rText.left = 1;
	rText.right = 18;
	rText.top = 2;
	rText.bottom = 18;
	pDC->DrawText(strCPU, &rText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	rText.left = 131;
	rText.right = 149;
	pDC->DrawText(strMem, &rText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	pDC->SetTextColor(RGB(0, 0, 0));
	rText.left = 23;
	rText.right = 73;
	pDC->DrawText(strNetDown, &rText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	rText.left = 77;
	rText.right = 127;
	pDC->DrawText(strNetUp, &rText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);


	pDC->SetTextColor(cOldTextColor);
	pDC->SetBkMode(nOldBkMode);
	pDC->SelectObject(pOldFont);
	font.DeleteObject();
}

void CpermoDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
	CDialog::OnLButtonDown(nFlags, point);
}


void CpermoDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (1 == nIDEvent)
	{
		{
			if (!(bAutoHide && rCurPos.left < 0))
			{
				//��ȡCPUռ��
				::GetSystemTimes(&idleTime, &kernelTime, &userTime);
				int idle = (int)CompareFileTime(preidleTime, idleTime);
				int kernel = (int)CompareFileTime(prekernelTime, kernelTime);
				int user = (int)CompareFileTime(preuserTime, userTime);
				int cpu = (kernel + user - idle) * 100 / (kernel + user);
				//int cpuidle = (idle)* 100 / (kernel + user);
				preidleTime = idleTime;
				prekernelTime = kernelTime;
				preuserTime = userTime;
				if (cpu < 0)
				{
					cpu = -cpu;
				}
				nCPU = cpu;
			}
		}
		{
			if (!(bAutoHide && rCurPos.right > rWorkArea.right))
			{
				//��ȡ�ڴ�ʹ����
				memStatex.dwLength = sizeof(memStatex);
				::GlobalMemoryStatusEx(&memStatex);
				nMem = memStatex.dwMemoryLoad;
			}
			// 			//�ڴ�ʹ���ʡ�
			// 			printf("�ڴ�ʹ����:\t%d%%\r\n", memStatex.dwMemoryLoad);
			// 			//�ܹ������ڴ档
			// 			printf("�ܹ������ڴ�:\t%I64d\r\n", memStatex.ullTotalPhys);
			// 			//���������ڴ档
			// 			printf("���������ڴ�:\t%I64d\r\n", memStatex.ullAvailPhys);
			// 			//ȫ���ڴ档
			// 			printf("ȫ���ڴ�:\t%I64d\r\n", memStatex.ullTotalPageFile);
			// 			//ȫ�����õ��ڴ档
			// 			printf("ȫ�����õ��ڴ�:\t%I64d\r\n", memStatex.ullAvailPageFile);
			// 			//ȫ���������ڴ档
			// 			printf("ȫ�����ڴ�:\t%I64d\r\n", memStatex.ullTotalVirtual);
		}
		{
			if (isOnline)
			{
				if (!(bAutoHide && (rCurPos.left < 0 || rCurPos.right > rWorkArea.right)))
				{
					//��ȡ������Ϣ
					double traffic1 = m_cTrafficClassDown.GetTraffic(SelectedInterface);
					double traffic2 = m_cTrafficClassUp.GetTraffic(SelectedInterface);
					//total_net_down = m_cTrafficClassDown.GetInterfaceTotalTraffic(SelectedInterface);
					//total_net_up = m_cTrafficClassUp.GetInterfaceTotalTraffic(SelectedInterface);
					fNetDown = traffic1 / 1024.0;
					fNetUp = traffic2 / 1024.0;
				}
			}
		}
		Invalidate(FALSE);
	}
	CDialog::OnTimer(nIDEvent);
}


void CpermoDlg::OnTopmost()
{
	// TODO:  �ڴ���������������
	if (bTopmost)
	{
		SetWindowPos(&wndNoTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		bTopmost = FALSE;
		m_Menu.CheckMenuItem(IDM_TOPMOST, MF_BYCOMMAND | MF_UNCHECKED);
	}
	else
	{
		SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		bTopmost = TRUE;
		m_Menu.CheckMenuItem(IDM_TOPMOST, MF_BYCOMMAND | MF_CHECKED); // ��ǰ��� 
	}
}

void CpermoDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CPoint p;
	//���ݹ���������Ϊ����ڴ������Ͻǵ����꣬WM_CONTEXTMENU���ݹ���������Ļ����
	GetCursorPos(&p);//�������Ļ����
	int nID = m_Menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RETURNCMD, p.x, p.y, this);
	switch (nID)
	{
	case IDM_TOPMOST:
		OnTopmost();
		break;
	case IDM_GREEN:
		OnGreen();
		break;
	case IDM_BLUE:
		OnBlue();
		break;
	case IDM_BLACK:
		OnBlack();
		break;
	case IDM_RED:
		OnRed();
		break;
	case IDM_ORANGE:
		OnOrange();
		break;
	case IDM_EXIT:
		OnExit();
		break;
	case IDM_AUTOHIDE:
		OnAutoHide();
		break;
	case 0:
		return;
	default:
		break;
	}

	CDialog::OnRButtonDown(nFlags, point);
}


void CpermoDlg::OnGreen()
{
	// TODO:  �ڴ���������������
	if (IDM_GREEN == nSkin)
	{
		return;
	}
	m_Menu.CheckMenuItem(nSkin, MF_BYCOMMAND | MF_UNCHECKED);
	nSkin = IDM_GREEN;
	m_Menu.CheckMenuItem(nSkin, MF_BYCOMMAND | MF_CHECKED);
	Invalidate(FALSE);
}


void CpermoDlg::OnBlue()
{
	// TODO:  �ڴ���������������
	if (IDM_BLUE == nSkin)
	{
		return;
	}
	m_Menu.CheckMenuItem(nSkin, MF_BYCOMMAND | MF_UNCHECKED);
	nSkin = IDM_BLUE;
	m_Menu.CheckMenuItem(nSkin, MF_BYCOMMAND | MF_CHECKED);
	Invalidate(FALSE);
}


void CpermoDlg::OnBlack()
{
	// TODO:  �ڴ���������������
	if (IDM_BLACK == nSkin)
	{
		return;
	}
	m_Menu.CheckMenuItem(nSkin, MF_BYCOMMAND | MF_UNCHECKED);
	nSkin = IDM_BLACK;
	m_Menu.CheckMenuItem(nSkin, MF_BYCOMMAND | MF_CHECKED);
	Invalidate(FALSE);
}


void CpermoDlg::OnRed()
{
	// TODO:  �ڴ���������������
	if (IDM_RED == nSkin)
	{
		return;
	}
	m_Menu.CheckMenuItem(nSkin, MF_BYCOMMAND | MF_UNCHECKED);
	nSkin = IDM_RED;
	m_Menu.CheckMenuItem(nSkin, MF_BYCOMMAND | MF_CHECKED);
	Invalidate(FALSE);
}


void CpermoDlg::OnOrange()
{
	// TODO:  �ڴ���������������
	if (IDM_ORANGE == nSkin)
	{
		return;
	}
	m_Menu.CheckMenuItem(nSkin, MF_BYCOMMAND | MF_UNCHECKED);
	nSkin = IDM_ORANGE;
	m_Menu.CheckMenuItem(nSkin, MF_BYCOMMAND | MF_CHECKED);
	Invalidate(FALSE);
}


//��ֹ�ƶ�������֮��
void CpermoDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);
	GetWindowRect(&rCurPos);
	if (rCurPos.bottom > rWorkArea.bottom)
	{
		rCurPos.bottom = rWorkArea.bottom;
		rCurPos.top = rCurPos.bottom - 20;
		MoveWindow(&rCurPos, TRUE);
	}
}


void CpermoDlg::OnExit()
{
	// TODO:  �ڴ���������������
	SaveConfig();
	OnOK();
}


void CpermoDlg::InitPopMenu(int nCount)
{
	BOOL bRet = m_Menu.CreatePopupMenu();
	ASSERT(bRet);
	bRet = m_SubMenu_Skin.CreatePopupMenu();
	ASSERT(bRet);
	//MENUITEM mi1;
	mi1.strText = _T("�����ö�");
	mi1.hIcon = ::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_TOPMOST));
	m_Menu.AppendMenuW(MF_OWNERDRAW | MF_BYCOMMAND, IDM_TOPMOST, (LPCTSTR)&mi1);

	mi9.strText = _T("��������");
	mi9.hIcon = ::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_AUTOHIDE));
	m_Menu.AppendMenuW(MF_OWNERDRAW | MF_BYCOMMAND, IDM_AUTOHIDE, (LPCTSTR)&mi9);

	//MENUITEM mi3;
	mi3.strText = _T("��ɫ");
	mi3.hIcon = ::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_GREEN));
	m_SubMenu_Skin.AppendMenuW(MF_OWNERDRAW | MF_BYCOMMAND, IDM_GREEN, (LPCTSTR)&mi3);
	//MENUITEM mi4;
	mi4.strText = _T("��ɫ");
	mi4.hIcon = ::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_BLUE));
	m_SubMenu_Skin.AppendMenuW(MF_OWNERDRAW | MF_BYCOMMAND, IDM_BLUE, (LPCTSTR)&mi4);
	//MENUITEM mi5;
	mi5.strText = _T("��ɫ");
	mi5.hIcon = ::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_BLACK));
	m_SubMenu_Skin.AppendMenuW(MF_OWNERDRAW | MF_BYCOMMAND, IDM_BLACK, (LPCTSTR)&mi5);
	//MENUITEM mi6;
	mi6.strText = _T("��ɫ");
	mi6.hIcon = ::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_RED));
	m_SubMenu_Skin.AppendMenuW(MF_OWNERDRAW | MF_BYCOMMAND, IDM_RED, (LPCTSTR)&mi6);
	//MENUITEM mi7;
	mi7.strText = _T("��ɫ");
	mi7.hIcon = ::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ORANGE));
	m_SubMenu_Skin.AppendMenuW(MF_OWNERDRAW | MF_BYCOMMAND, IDM_ORANGE, (LPCTSTR)&mi7);

	mi2.strText = _T("Ƥ��");
	mi2.hIcon = ::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_SKIN));
	m_Menu.AppendMenuW(MF_BYPOSITION | MF_POPUP | MF_OWNERDRAW,
		(UINT)m_SubMenu_Skin.m_hMenu, (LPCTSTR)&mi2);
	//MENUITEM mi8;
	mi8.strText = _T("�˳�");
	mi8.hIcon = ::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_EXIT));
	m_Menu.AppendMenuW(MF_OWNERDRAW | MF_BYCOMMAND, IDM_EXIT, (LPCTSTR)&mi8);
}


BOOL CpermoDlg::OpenConfig()
{
	TCHAR direc[256];
	::GetCurrentDirectory(256, direc);//��ȡ��ǰĿ¼����
	TCHAR temp[256];
	wsprintf(temp, _T("%s\\config.ini"), direc);
	int left, top, topmost, skin, autohide;
	left = ::GetPrivateProfileInt(_T("Main"), _T("left"), -1, temp);
	top = ::GetPrivateProfileInt(_T("Main"), _T("top"), -1, temp);
	topmost = ::GetPrivateProfileInt(_T("Main"), _T("topmost"), -1, temp);
	skin = ::GetPrivateProfileInt(_T("Main"), _T("skin"), -1, temp);
	autohide = ::GetPrivateProfileInt(_T("Main"), _T("autohide"), -1, temp);
	if (-1 == left || left < 0)
	{
		return FALSE;
	}
	if (-1 == top || top < 0)
	{
		return FALSE;
	}
	if (-1 == topmost || (topmost != 0 && topmost != 1))
	{
		return FALSE;
	}
	if (skin != IDM_GREEN && skin != IDM_BLUE && skin != IDM_BLACK
		&& skin != IDM_RED && skin != IDM_ORANGE)
	{
		return FALSE;
	}
	if (-1 == autohide || (autohide != 0 && autohide != 1))
	{
		return FALSE;
	}
	rCurPos.left = left;
	rCurPos.top = top;
	bTopmost = topmost;
	nSkin = skin;
	bAutoHide = autohide;
	return TRUE;
}


BOOL CpermoDlg::SaveConfig()
{
	TCHAR direc[256];
	::GetCurrentDirectory(256, direc);//��ȡ��ǰĿ¼����
	TCHAR temp[256];
	wsprintf(temp, _T("%s\\config.ini"), direc);
	TCHAR cLeft[32], cTop[32], cTopMost[32], cSkin[32], cAutoHide[32];
	_itow_s(rCurPos.left, cLeft, 10);
	_itow_s(rCurPos.top, cTop, 10);
	_itow_s(bTopmost, cTopMost, 10);
	_itow_s(nSkin, cSkin, 10);
	_itow_s(bAutoHide, cAutoHide, 10);
	::WritePrivateProfileString(_T("Main"), _T("left"), cLeft, temp);
	::WritePrivateProfileString(_T("Main"), _T("top"), cTop, temp);
	::WritePrivateProfileString(_T("Main"), _T("topmost"), cTopMost, temp);
	::WritePrivateProfileString(_T("Main"), _T("skin"), cSkin, temp);
	::WritePrivateProfileString(_T("Main"), _T("autohide"), cAutoHide, temp);
	return TRUE;
}
LRESULT CpermoDlg::OnNcHitTest(CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (bAutoHide)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.dwFlags = TME_HOVER | TME_LEAVE; //ע��ǿͻ����뿪
		tme.hwndTrack = m_hWnd;
		tme.dwHoverTime = HOVER_DEFAULT; //ֻ��HOVER��Ч  
		::TrackMouseEvent(&tme);
	}
	return CDialog::OnNcHitTest(point);
}

void CpermoDlg::OnMouseHover(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (bAutoHide)
	{
		if (rCurPos.right == rWorkArea.right + 130)
		{
			rCurPos.right = rWorkArea.right;
			rCurPos.left = rCurPos.right - 150;
			MoveWindow(&rCurPos, TRUE);
		}
		if (rCurPos.left == rWorkArea.left - 130)
		{
			rCurPos.left = rWorkArea.left;
			rCurPos.right = rCurPos.left + 150;
			MoveWindow(&rCurPos, TRUE);
		}
	}
	CDialog::OnMouseHover(nFlags, point);
}

void CpermoDlg::OnMouseLeave()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (bAutoHide)
	{
		if (rCurPos.right >= rWorkArea.right)
		{
			rCurPos.right = rWorkArea.right + 130;
			rCurPos.left = rCurPos.right - 150;
			MoveWindow(&rCurPos, TRUE);
		}
		if (rCurPos.left <= rWorkArea.left)
		{
			rCurPos.left = rWorkArea.left - 130;
			rCurPos.right = rCurPos.left + 150;
			MoveWindow(&rCurPos, TRUE);
		}
	}
	CDialog::OnMouseLeave();
}

void CpermoDlg::OnAutoHide(void)
{
	if (bAutoHide)
	{
		bAutoHide = FALSE;
		m_Menu.CheckMenuItem(IDM_AUTOHIDE, MF_BYCOMMAND | MF_UNCHECKED);
	}
	else
	{
		bAutoHide = TRUE;
		m_Menu.CheckMenuItem(IDM_AUTOHIDE, MF_BYCOMMAND | MF_CHECKED); // ��ǰ��� 
	}
}
