// CClientSocket.h :헤더파일
#pragma once
#define WM_CLIENT_RECV WM_USER+1
// CClientSocket 명령 대상

class CClientSocket : public CSocket
{
public:
	CClientSocket();
	virtual ~CClientSocket();
	void SetWnd(HWND hWnd);
	HWND m_hWnd;
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
};


