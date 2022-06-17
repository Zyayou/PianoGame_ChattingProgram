
// MFCApplication2Dlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "MFCApplication2.h"
#include "MFCApplication2Dlg.h"
#include "afxdialogex.h"
#include "Piano.h"
//#include "Mycomm.h"//시리얼 통신 관련


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define PORT 9999


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

//대화 상자의 배경 색과 배경 이미지를 지정
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

//dialog(화면)와 data(코드속 변수)간의 데이터 교환
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

//메시지 맵(메시지와 메시지 핸들러를 매핑하는 테이블)을 지원하기위한 매크로입니다.
BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx) //메시지 맵의 정의를 시작합니다.
END_MESSAGE_MAP() //메시지 맵의 정의를 종료합니다.


// CMFCApplication2Dlg 대화 상자


//아두이노 관련으로 무언가가 증가된 듯 다름
CMFCApplication2Dlg::CMFCApplication2Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCAPPLICATION2_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplication2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDIT2, m_edit_send);		//IDC_EDIT_SEND 컨트롤 변수 추가
	DDX_Control(pDX, IDC_LIST1, m_list_msg);			//IDC_LIST_MSG 컨트롤 변수 추가
	//DDX_Control(pDX, IDC_LIST2, m_list_client);
	//DDX_Control(pDX, IDC_COMBO_COMPORT, m_combo_comport_list);
	//DDX_Control(pDX, IDC_COMBO_BAUDRATE, m_combo_baudrate_list);
	//DDX_CBString은 콤보 상자 컨트롤의 편집 필등의 현재 내용을 초기화하거나 검색
	//콤보 상자 컨트롤 관련인듯 하다
	//DDX_CBString(pDX, IDC_COMBO_COMPORT, m_str_comport);
	//DDX_CBString(pDX, IDC_COMBO_BAUDRATE, m_str_baudrate);
	DDX_Control(pDX, IDC_EDIT1, m_name_set);
	DDX_Control(pDX, IDC_BUTTON2, b_piano);
	DDX_Control(pDX, IDC_BUTTON3, b_end);
}

BEGIN_MESSAGE_MAP(CMFCApplication2Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_CLIENT_RECV, &CMFCApplication2Dlg::OnClientRecv)
	//ON_BN_CLICKED(IDC_BUTTON1, &CMFCApplication2Dlg::OnBnClickedButton1)
	//ON_LBN_SELCHANGE(IDC_LIST2, &CMFCApplication2Dlg::OnLbnSelchangeList2)
	ON_BN_CLICKED(IDC_BUTTON2, &CMFCApplication2Dlg::OnBnClickedButton2)

	ON_BN_CLICKED(IDC_BUTTON3, &CMFCApplication2Dlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDOK, &CMFCApplication2Dlg::OnBnClickedOk)


	//  수신 데이터 발생 시 전달되는 메시지와 스레드 종료 메시지 처리 구현 
	//ON_MESSAGE(WM_MYCLOSE, &CMFCApplication2Dlg::OnThreadClosed)
	//ON_MESSAGE(WM_MYRECEIVE, &CMFCApplication2Dlg::OnReceive)
		
	
	//ON_BN_CLICKED(IDC_BT_CLEAR, &CserialDlg::OnBnClickedBtClear)
	//ON_BN_CLICKED(IDC_BT_SEND, &CserialDlg::OnBnClickedBtSend)

	//ON_BN_CLICKED(IDC_BT_CONNECT, &CMFCApplication2Dlg::OnBnClickedBtConnect)
	//ON_CBN_SELCHANGE(IDC_COMBO_COMPORT, &CMFCApplication2Dlg::OnCbnSelchangeComboComport)
	//ON_CBN_SELCHANGE(IDC_COMBO_BAUDRATE, &CMFCApplication2Dlg::OnCbnSelchangeComboBaudrate)
	//ON_LBN_SELCHANGE(IDC_LIST1, &CMFCApplication2Dlg::OnLbnSelchangeList1)
	ON_BN_CLICKED(IDC_BUTTON4, &CMFCApplication2Dlg::OnBnClickedButton4)
	ON_LBN_SELCHANGE(IDC_LIST1, &CMFCApplication2Dlg::OnLbnSelchangeList1)
END_MESSAGE_MAP()


// CMFCApplication2Dlg 메시지 처리기
// 맵을 열었을때 제일 먼저 초기화되는 곳이다
BOOL CMFCApplication2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	// arduino comport select
	/*m_combo_comport_list.AddString(_T("COM1"));
	m_combo_comport_list.AddString(_T("COM2"));
	m_combo_comport_list.AddString(_T("COM3"));
	m_combo_comport_list.AddString(_T("COM4"));
	m_combo_comport_list.AddString(_T("COM5"));
	m_combo_comport_list.AddString(_T("COM6"));

	// arduino baudrate select
	m_combo_baudrate_list.AddString(_T("9600"));
	m_combo_baudrate_list.AddString(_T("19200"));
	m_combo_baudrate_list.AddString(_T("115200"));
	
	//초기 보여주는 값이다
	// comport_state는 초기에 false로 놓고 버튼은 OPEN 으로 보여주며, com5속도는 115200bps
	comport_state = false;
	GetDlgItem(IDC_BT_CONNECT)->SetWindowText(_T("OPEN"));
	m_str_comport = _T("COM4");
	m_str_baudrate = _T("9600");
	UpdateData(FALSE);
	*/
	// socket --
	m_ClientSocket.SetWnd(this->m_hWnd);
	m_ClientSocket.Create();
	if (m_ClientSocket.Connect(_T("127.0.0.1"), PORT) == FALSE) {
		AfxMessageBox(_T("ERROR : Failed to connect Server"));
		PostQuitMessage(0);
		return FALSE;
	}

	b_piano.LoadBitmaps(IDB_PIANO, NULL, NULL, NULL);
	b_piano.SizeToContent();
	b_end.LoadBitmaps(IDB_END, NULL, NULL, NULL);
	b_end.SizeToContent();

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

//시스템 메뉴 처리
void CMFCApplication2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.
// 여긴 진짜 건든게 없네 기본 그자체
void CMFCApplication2Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

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
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMFCApplication2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//아두이노 관련
// 연결되어 처리될 함수를 구현 
//WPARAM == WORD PARAMETER (핸들 or 정수), LPARAM == LONG PARAMETER (포인터 값)
/*
LRESULT CMFCApplication2Dlg::OnThreadClosed(WPARAM length, LPARAM lpara)
{
	//overlapped i/o 핸들을닫는다.
	((CMycomm*)lpara)->HandleClose();
	delete ((CMycomm*)lpara);
	return 0;
}

//아두이노 관련
//함수에서 설정된 WPARAM length에 임계값을 초과하는 스택 사용량이 검색되었기에 경고하는 것이다
LRESULT CMFCApplication2Dlg::OnReceive(WPARAM length, LPARAM lpara)
{
	CString str;
	char data[20000];
	int arduino2; // 얜 왜 선언하신겨
	if (m_comm)
	{
		m_comm->Receive(data, length); //length 길이만큼데이터를받는다.
		data[length] = _T('\0'); //NULL
		str += _T("\r\n"); //윈도우용 줄바꿈
		for (int i = 0; i < length; i++)
		{
			str += data[i];
		}

		m_list_client.InsertString(-1, str);
		m_list_client.SetCurSel(m_list_client.GetCount() - 1);
		//m_list_msg.ReplaceSel(str); //에디트박스에표시하기위함
		str = "";
	
		//UpdateData(FALSE);
		//m_list_msg.LineScroll(m_list_msg.GetLineCount());
		
	}
	return 0;
}
*/

afx_msg LRESULT CMFCApplication2Dlg::OnClientRecv(WPARAM wParam, LPARAM lParam)
{
	LPCTSTR lpszStr = (LPCTSTR)lParam;

	m_list_msg.InsertString(-1, lpszStr);
	m_list_msg.SetCurSel(m_list_msg.GetCount() - 1);
	return 0;
	
}


/*
//아두이노 관련
LRESULT CMFCApplication2Dlg::OnReceive(WPARAM length, LPARAM lpara)
{
	CString str;
	char data[20000];
	int arduino2;
	if (m_comm)
	{
		m_comm->Receive(data, length); //length 길이만큼데이터를받는다.
		data[length] = _T('\0');
		str += _T("\r\n");
		for (int i = 0; i < length; i++)
		{
			str += data[i];
		}

		//m_edit_rcv_view.ReplaceSel(str); //에디트박스에표시하기위함
		//str = "";
		//arduino2 = _ttoi(str);

		str.Format(_T("%d"),arduino2);
		str = "";

		//UpdateData(FALSE);
		//m_edit_rcv_view.LineScroll(m_edit_rcv_view.GetLineCount());
	}
	return 0;
}



//아두이노 관련
afx_msg LRESULT CMFCApplication2Dlg::OnClientRecv(WPARAM wParam, LPARAM lParam)
{
	LPCTSTR lpszStr = (LPCTSTR)lParam;


	m_list_msg.InsertString(-1, lpszStr);
	m_list_msg.SetCurSel(m_list_msg.GetCount() - 1);
	return 0;
}

*/










void CMFCApplication2Dlg::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nameSet) {
		CString str;
		LPCTSTR lpctstr;
		UpdateData(TRUE);
		m_edit_send.GetWindowTextW(str);
		str.Format(_T("%s : %s"), name, str);
		lpctstr = (LPCTSTR)str;
		m_ClientSocket.Send(lpctstr, lstrlen(lpctstr) * 2);
		m_edit_send.SetWindowTextW(_T(""));
		UpdateData(FALSE);
	}
	else {
		CString msg;
		msg.Format(_T("닉네임을 입력해 주세요"));
		m_list_msg.InsertString(-1, msg);
	}
}

void CMFCApplication2Dlg::OnBnClickedButton1(int score)
{
	if (nameSet) {
		CString str;
		LPCTSTR lpctstr;
		UpdateData(TRUE);
		str.Format(_T("%s님이 %d 점을 기록하였습니다!"), name, score);
		lpctstr = (LPCTSTR)str;
		m_ClientSocket.Send(lpctstr, lstrlen(lpctstr) * 2);
		m_edit_send.SetWindowTextW(_T(""));
		UpdateData(FALSE);
	}
	else {
		CString msg;
		msg.Format(_T("닉네임을 입력해 주세요"));
		m_list_msg.InsertString(-1, msg);
	}
}
/*
void CMFCApplication2Dlg::OnLbnSelchangeList2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
*/

void CMFCApplication2Dlg::OnBnClickedButton2()
{// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nameSet) {
		Piano piano;
		piano.DoModal();
	}
	else {
		CString msg;
		msg.Format(_T("닉네임을 입력해 주세요"));
		m_list_msg.InsertString(-1, msg);
	}
}


void CMFCApplication2Dlg::OnBnClickedButton3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnOK();
}


void CMFCApplication2Dlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nameSet) {
		OnBnClickedButton1();
	}
	else {
		CString msg;
		msg.Format(_T("닉네임을 입력해 주세요"));
		m_list_msg.InsertString(-1, msg);
	}
}
/*
//아두이노 관련
void CMFCApplication2Dlg::OnBnClickedBtConnect()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (comport_state)
	{
		if (m_comm)        //컴포트가존재하면
		{
			m_comm->Close();

			m_comm = NULL;

			AfxMessageBox(_T("COM 포트닫힘"));

			comport_state = false;

			GetDlgItem(IDC_BT_CONNECT)->SetWindowText(_T("OPEN"));

			//GetDlgItem(IDC_BT_SEND)->EnableWindow(false);
		}
	}
	else
	{
		m_comm = new CMycomm(_T("\\\\.\\") + m_str_comport, m_str_baudrate, _T("None"), _T("8 Bit"), _T("1 Bit"));         // initial Comm port
		if (m_comm->Create(GetSafeHwnd()) != 0) //통신포트를열고윈도우의핸들을넘긴다.
		{
			AfxMessageBox(_T("COM 포트열림"));
			comport_state = true;
			GetDlgItem(IDC_BT_CONNECT)->SetWindowText(_T("CLOSE"));
			//GetDlgItem(IDC_BT_SEND)->EnableWindow(true);
		}
		else
		{
			AfxMessageBox(_T("ERROR!"));
		}
	}
}

//아두이노 관련
void CMFCApplication2Dlg::OnCbnSelchangeComboComport()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData();
}

//아두이노 관련
void CMFCApplication2Dlg::OnCbnSelchangeComboBaudrate()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData();
}


//아두이노 관련
void CMFCApplication2Dlg::OnLbnSelchangeList1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

*/
void CMFCApplication2Dlg::OnBnClickedButton4()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString str;
	m_name_set.GetWindowTextW(str);
	name = str;
	nameSet = true;
}


void CMFCApplication2Dlg::OnLbnSelchangeList1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
