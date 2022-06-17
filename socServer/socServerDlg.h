
// socServerDlg.h: 헤더 파일
//

#pragma once
#include "CServerSocket.h"


// CsocServerDlg 대화 상자
class CsocServerDlg : public CDialogEx
{
// 생성입니다.
public:
	CsocServerDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SOCSERVER_DIALOG };
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

	afx_msg LRESULT OnAcceptSocket(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnClientMsgRecv(WPARAM wParam, LPARAM lParam);
	//afx_msg LRESULT OnClientList(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnClientClose(WPARAM wParam, LPARAM lParam);
private:
	CClientSocket* m_pClientSocket;
	CServerSocket* m_pServerSocket;
public:
	afx_msg void OnLbnSelchangeList2();
	afx_msg void OnLbnSelchangeList1();
	CListBox m_list_client;
	CListBox m_list_msg;
	CPtrList m_ptrClientSocketList; //For manage Client Sockets
	afx_msg void OnBnClickedOk();
};
