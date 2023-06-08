
// MFC_OmocDlg.h: 헤더 파일
//

#pragma once

#define X_COUNT			19
#define Y_COUNT			19
#define G_LEN			30		// Grid Length
#define G_LEN_HF		15		// Grid Length / 2

// CMFCOmocDlg 대화 상자
class CMFCOmocDlg : public CDialogEx
{
private:
	CPen m_grid_pen;
	char m_step = 0;		// 0 : 흑돌 1 : 백돌
	char m_dol[Y_COUNT][X_COUNT];	// 0 : 돌없음  2 : 흑돌  3 : 백돌
	bool m_complite = false;
	bool m_is_gameover = false;
// 생성입니다.
public:
	CMFCOmocDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFC_OMOC_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedRegameBtn00();
	afx_msg void OnBnClickedExitGameBtn();
};
