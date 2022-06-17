#pragma once
#include "afxdialogex.h"
#pragma comment(lib,"winmm")
#include <mmsystem.h>
//#include "MFCApplication2Dlg.h"

#include "Mycomm.h"
//#include "MFCApplication2Dlg.h"

#pragma once
#define MAX_COL 4 // 행의 개수 4 (타일의 개수)
#define MAX_ROW 4 //열의 개수 4 (거의 타일의 높이)

#define MOVE_STEP 7 //7픽셀씩 이동
#define LIMIT_WIDTH 400 // 타일 영역에 대한 폭의 크기 400
#define LIMIT_HEIGHT 500 //사각형 최대 이동 거리 500
#define TILE_WIDTH LIMIT_WIDTH/MAX_COL //타일(사각형)의 폭
#define TILE_HEIGHT LIMIT_HEIGHT/(MAX_ROW - 1) //타일(사각형)의 높이


// Piano 대화 상자

class Piano : public CDialogEx
{
	DECLARE_DYNAMIC(Piano)

private:
	int m_miss_count = 0; // miss 횟수 저장
	CRect m_rect[MAX_ROW]; //타일의 개수
	int m_key_table[MAX_ROW]; //타일이 어디서 나올지 정함
	int overmiss = 7; //오버미스 나는 것을 방지
	int deadline = 2;
	int event = 7; // 아두이노로 클릭하기 위한 이벤트 값
	int m_score = 0; //점수
	CFont m_font; //GAME OVER 폰트 설정
	CFont m_font_b; //기본 폰트
	BOOL STOP = false; // 일시정지 재시작 이벤트 값
	BOOL RESTART = false; // 다시하기 이벤트 값
	BOOL GAMEOVER = false; // 게임오버 이벤트 값

// 생성입니다.

public:
	// seriaㅣ 
	CMycomm* m_comm;
	LRESULT      OnThreadClosed(WPARAM length, LPARAM lpara);
	LRESULT      OnReceive(WPARAM length, LPARAM lpara);

	Piano(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~Piano();

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.


	// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnEnChangeEdit1();
	CEdit arduino;
	int arduino2;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
	void Wait(DWORD dwMillisecond);
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnClose();

	CString m_str_comport; //com4
	CString m_str_baudrate; // 9600

	CBitmapButton b_stop;
	CBitmapButton b_share;
	CBitmapButton b_home;
	CBitmapButton b_restart;
};
