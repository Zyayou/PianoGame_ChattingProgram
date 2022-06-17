
// MFCApplication2Dlg.h: 헤더 파일
//

#pragma once
#include "CClientSocket.h"
#include "afxwin.h"

//#include "Mycomm.h"

// CMFCApplication2Dlg 대화 상자
class CMFCApplication2Dlg : public CDialogEx
{
// 생성입니다.
public:
	// seriaㅣ 
	//CMycomm* m_comm;
	//LRESULT      OnThreadClosed(WPARAM length, LPARAM lpara);
	//LRESULT      OnReceive(WPARAM length, LPARAM lpara);

	CMFCApplication2Dlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPPLICATION2_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnClientRecv(WPARAM wParam, LPARAM lParam);  //사용자 메시지 함수 추가
private:
	CClientSocket m_ClientSocket;
	CString name;
	BOOL nameSet = false;
public:
	CListBox m_list_msg;
	CEdit m_edit_send;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton1(int score);
	CListBox IDC_LIST_MSG;
	CEdit IDC_EDIT_SEND;
	CButton IDC_BTN_SEND;
	//afx_msg void OnLbnSelchangeList2();
	afx_msg void OnBnClickedButton2();
	CListBox m_list_client;
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedOk();
	/*
	BOOL comport_state;
	CComboBox m_combo_comport_list;
	CComboBox m_combo_baudrate_list;
	CString m_str_comport;
	CString m_str_baudrate;
	afx_msg void OnBnClickedBtConnect();
	afx_msg void OnCbnSelchangeComboComport();
	afx_msg void OnCbnSelchangeComboBaudrate();
	afx_msg void OnLbnSelchangeList1();
	*/
	CEdit m_name_set;
	CBitmapButton b_piano;
	CBitmapButton b_end;
	afx_msg void OnBnClickedButton4();
	afx_msg void OnLbnSelchangeList1();
};
