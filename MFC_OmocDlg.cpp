
// MFC_OmocDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "MFC_Omoc.h"
#include "MFC_OmocDlg.h"
#include "afxdialogex.h"
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMFCOmocDlg 대화 상자


CMFCOmocDlg::CMFCOmocDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFC_OMOC_DIALOG, pParent), m_grid_pen(PS_SOLID, 1, RGB(144, 90, 40))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	memset(m_dol, 0, sizeof(m_dol));
}

void CMFCOmocDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCOmocDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_REGAME_BTN00, &CMFCOmocDlg::OnBnClickedRegameBtn00)
	ON_BN_CLICKED(IDC_EXIT_GAME_BTN, &CMFCOmocDlg::OnBnClickedExitGameBtn)
END_MESSAGE_MAP()


// CMFCOmocDlg 메시지 처리기

BOOL CMFCOmocDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	SetBackgroundColor(RGB(255, 176, 77));
	CFont mFont;
	mFont.CreatePointFont(100, L"굴림");
	GetDlgItem(IDC_WHO_WIN)->SetFont(&mFont, 1);
	

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CMFCOmocDlg::OnPaint()
{
	CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.
	if (IsIconic())
	{
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CBrush* p_old_brush = dc.GetCurrentBrush();
		CPen* p_old_pen = dc.SelectObject(&m_grid_pen);
		for (int i = 0; i < X_COUNT; i++) {
			dc.MoveTo(i * G_LEN + G_LEN / 2, G_LEN / 2);
			dc.LineTo(i * G_LEN + G_LEN / 2, G_LEN / 2 + (Y_COUNT - 1) * G_LEN);

			dc.MoveTo(G_LEN / 2, G_LEN / 2 + i * G_LEN);
			dc.LineTo((X_COUNT - 1) * G_LEN + G_LEN / 2, G_LEN / 2 + i * G_LEN);
		}
		for (int y = 0; y < Y_COUNT; y++) {
			for (int x = 0; x < X_COUNT; x++) {
				if (m_dol[y][x]) {
					if (m_dol[y][x] == 1) dc.SelectStockObject(BLACK_BRUSH);
					else dc.SelectStockObject(WHITE_BRUSH);
					dc.Ellipse(x * G_LEN, y * G_LEN, G_LEN + x * G_LEN, G_LEN + y * G_LEN);

				}
			}
		}
		dc.SelectObject(p_old_brush);
		dc.SelectObject(p_old_pen);
		//CDialogEx::OnPaint();
	}
}

HCURSOR CMFCOmocDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCOmocDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	CClientDC dc(this);
	int x = point.x / G_LEN;
	int y = point.y / G_LEN;
	
	if (x < X_COUNT && y < Y_COUNT)
	{
		if (m_dol[y][x] == 0) {
			m_dol[y][x] = m_step + 1;
			CGdiObject* p_old_brush;
			CPen* p_old_pen = dc.SelectObject(&m_grid_pen);
			if (m_step == 0)
				p_old_brush = dc.SelectStockObject(BLACK_BRUSH);
			else
				p_old_brush = dc.SelectStockObject(WHITE_BRUSH);

			dc.Ellipse(x * G_LEN, y * G_LEN, G_LEN + x * G_LEN, G_LEN + y * G_LEN);

			dc.SelectObject(&p_old_pen);
			dc.SelectObject(p_old_brush);
			m_step = !m_step;
		}
	}
	if (m_step == 0) SetDlgItemText(IDC_TURN_STATIC, L"차례 : 흑돌");
	else SetDlgItemText(IDC_TURN_STATIC, L"차례 : 백돌");
	// 오목이 완성되었는지 판단
	for (int y = 0; y < Y_COUNT; y++) {
		for (int x = 0; x < X_COUNT; x++) {
			if (m_dol[y][x] != 0) {
				// 수평으로 오목 완성
				if (m_dol[y][x] == m_dol[y][x + 1] && m_dol[y][x] == m_dol[y][x + 2] && m_dol[y][x] == m_dol[y][x + 3] && m_dol[y][x] == m_dol[y][x + 4])
				{
					// 여기에 흑돌/백돌 판단로직 삽입. 
					CString tmp, tmp2;
					if (m_dol[y][x] == 1) {
						tmp = L"Horizontal (흑돌 승)";
						tmp2 = L"흑돌 승!";
					}
					else {
						tmp = L"Horizontal (백돌 승)";
						tmp2 = L"백돌 승!";
					}
					m_complite = true;
					m_is_gameover = true;
					SetDlgItemText(1000, tmp);
					SetDlgItemText(1006, tmp2);
				}
				// 수직으로 오목 완성
				if (m_dol[y][x] == m_dol[y + 1][x] && m_dol[y][x] == m_dol[y + 2][x] && m_dol[y][x] == m_dol[y + 3][x] && m_dol[y][x] == m_dol[y + 4][x])
				{
					CString tmp, tmp2;
					if (m_dol[y][x] == 1) {
						tmp = L"Vertical (흑돌 승)";
						tmp2 = L"흑돌 승!";
					}
					else {
						tmp = L"Vertical (백돌 승)";
						tmp2 = L"백돌 승!";
					}
					m_complite = true;
					m_is_gameover = true;
					SetDlgItemText(1000, tmp);
					SetDlgItemText(1006, tmp2);
				}
				// 우->좌 대각성으로 오목 완성
				if (m_dol[y][x] == m_dol[y - 1][x + 1] && m_dol[y][x] == m_dol[y - 2][x + 2] && m_dol[y][x] == m_dol[y - 3][x + 3] && m_dol[y][x] == m_dol[y - 4][x + 4])
				{
					CString tmp, tmp2;
					if (m_dol[y][x] == 1) {
						tmp = L"right->left (흑돌 승)";
						tmp2 = L"흑돌 승!";
					}
					else {
						tmp = L"right->left (백돌 승)";
						tmp2 = L"백돌 승!";
					}
					m_complite = true;
					m_is_gameover = true;
					SetDlgItemText(1000, tmp);
					SetDlgItemText(1006, tmp2);
				}
				// 좌->우 대각성으로 오목 완성
				if (m_dol[y][x] == m_dol[y - 1][x - 1] && m_dol[y][x] == m_dol[y - 2][x - 2] && m_dol[y][x] == m_dol[y - 3][x - 3] && m_dol[y][x] == m_dol[y - 4][x - 4])
				{
					CString tmp, tmp2;
					if (m_dol[y][x] == 1) {
						tmp = L"left->right (흑돌 승)";
						tmp2 = L"흑돌 승!";
					}
					else {
						tmp = L"left-right (백돌 승)";
						tmp2 = L"백돌 승!";
					}
					m_complite = true;
					m_is_gameover = true;
					SetDlgItemText(1000, tmp);
					SetDlgItemText(1006, tmp2);
				}
			}
		}
	}
	if (m_complite == true) {
		//memset(m_dol, 0, sizeof(m_dol));
		//Invalidate();
		GetDlgItem(IDC_REGAME_BTN00)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EXIT_GAME_BTN)->ShowWindow(SW_SHOW);

		m_complite = false;
	}
	
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CMFCOmocDlg::OnBnClickedRegameBtn00()
{
	GetDlgItem(IDC_REGAME_BTN00)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EXIT_GAME_BTN)->ShowWindow(SW_HIDE);
	SetDlgItemText(IDC_TEXT_EDIT, L"");
	SetDlgItemText(IDC_WHO_WIN, L"");
	m_is_gameover = false;

	memset(m_dol, 0, sizeof(m_dol));
	Invalidate();
}


void CMFCOmocDlg::OnBnClickedExitGameBtn()
{
	::SendMessage(this->m_hWnd,WM_CLOSE,NULL,NULL);
}
