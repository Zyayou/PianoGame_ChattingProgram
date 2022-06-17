// Piano.cpp: 구현 파일
//

#include "pch.h"
#include "MFCApplication2.h"
#include "afxdialogex.h"
#include "Piano.h"
#include "MFCApplication2Dlg.h"
#include "Mycomm.h"

// Piano 대화 상자

IMPLEMENT_DYNAMIC(Piano, CDialogEx)

Piano::Piano(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
	, m_str_comport(_T(""))
	, m_str_baudrate(_T(""))
{
	srand((unsigned int)time(NULL)); // 매번 다른 rand 수 설정
	for (int i = 0; i < MAX_ROW; i++) { //타일의 좌표 지정
		m_rect[i].SetRect(100, i * TILE_HEIGHT * (-1) - TILE_HEIGHT, 100 + TILE_WIDTH, i * TILE_HEIGHT * (-1)); //타일의 좌표 지정식
		m_key_table[i] = rand() % MAX_COL; // 초기 위치 어디서 나올 것인가
		if (i > 0 && m_key_table[i] == m_key_table[i - 1]) {
			m_key_table[i] = (m_key_table[i] + 3) % MAX_COL; //연속적이지 않게 하는 랜덤
		}
	}
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

}

Piano::~Piano()
{
}

void Piano::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, arduino);
	DDX_Text(pDX, IDC_EDIT1, arduino2);
	DDX_Control(pDX, IDC_BUTTON1, b_stop);
	DDX_Control(pDX, IDC_BUTTON3, b_share);
	DDX_Control(pDX, IDOK, b_home);
	DDX_Control(pDX, IDC_BUTTON2, b_restart);
}


BEGIN_MESSAGE_MAP(Piano, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_EN_CHANGE(IDC_EDIT1, &Piano::OnEnChangeEdit1)
	ON_BN_CLICKED(IDOK, &Piano::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &Piano::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &Piano::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &Piano::OnBnClickedButton3)
	ON_WM_CLOSE()

	ON_MESSAGE(WM_MYCLOSE, &Piano::OnThreadClosed)
	ON_MESSAGE(WM_MYRECEIVE, &Piano::OnReceive)

END_MESSAGE_MAP()


// Piano 메시지 처리기

BOOL Piano::OnInitDialog()
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
	SetTimer(1, 80, NULL); //1에 0.08초마다 시간업뎃
	SetTimer(2, 1000, NULL); //2에 1초마다 시간업뎃

	//폰트 설정
	m_font.CreateFont(40, 0, 0, 0, 1, 0, 0, 0, 0, OUT_DEFAULT_PRECIS, 0, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("굴림"));
	m_font_b.CreateFont(15, 0, 0, 0, 1, 0, 0, 0, 0, OUT_DEFAULT_PRECIS, 0, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("Microsoft Sans Serif"));
	//PlaySound(szSoundPath, AfxGetInstanceHandle(), SND_ASYNC | SND_LOOP);
	PlaySound(MAKEINTRESOURCE(IDR_WAVE2), NULL, SND_ASYNC | SND_RESOURCE | SND_LOOP); // 노래2 재생

	b_stop.LoadBitmaps(IDB_STOP, NULL, NULL, NULL);
	b_stop.SizeToContent();
	b_share.LoadBitmaps(IDB_SHARE, NULL, NULL, NULL);
	b_share.SizeToContent();
	b_home.LoadBitmaps(IDB_HOME, NULL, NULL, NULL);
	b_home.SizeToContent();
	b_restart.LoadBitmaps(IDB_RESTART, NULL, NULL, NULL);
	b_restart.SizeToContent();

	m_str_comport = _T("COM4");
	m_str_baudrate = _T("115200");
	m_comm = new CMycomm(_T("\\\\.\\") + m_str_comport, m_str_baudrate, _T("None"), _T("8 Bit"), _T("1 Bit")); //inital comm port
	if (m_comm->Create(GetSafeHwnd()) != 0) {
	}
	else {
		AfxMessageBox(_T("ERROR!"));
	}

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void Piano::OnPaint()
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
		CRect r; //타일 객체
		CPaintDC dc(this); // 클라이언트 영역에 출력할 시 씀

		dc.FillSolidRect(CRect(0, 0, LIMIT_WIDTH, LIMIT_HEIGHT), RGB(0, 0, 0)); // 배경 색칠
		for (int y = 0; y < MAX_ROW; y++) { //행
			for (int x = 0; x < MAX_COL; x++) { //열
				r.SetRect(x * TILE_WIDTH, m_rect[y].top, (x + 1) * TILE_WIDTH, m_rect[y].bottom); //타일 크기 설정
				if (m_key_table[y] == x) dc.FillSolidRect(r, RGB(255, 255, 0)); //타일 색칠하기
				else {
					if (r.bottom >= LIMIT_HEIGHT) dc.FillSolidRect(r, RGB(68, 68, 0)); //클릭할 타일 행에 색칠
				}
			}
		}
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR Piano::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




// 연결되어 처리될 함수를 구현 
LRESULT Piano::OnThreadClosed(WPARAM length, LPARAM lpara)
{
	//overlapped i/o 핸들을닫는다.
	((CMycomm*)lpara)->HandleClose();
	delete ((CMycomm*)lpara);
	return 0;
}

LRESULT Piano::OnReceive(WPARAM length, LPARAM lpara)
{
	CString str;
	char data[20000];
	if (m_comm)
	{
		m_comm->Receive(data, length); //length 길이만큼데이터를받는다.
		data[length] = _T('\0');
		str += _T("\r\n");
		for (int i = 0; i < length; i++)
		{
			str += data[i];
			arduino2 = _ttoi(str);
		}

		if (0 < arduino2 && arduino2 < 11) {
			event = 0;
		}
		else if (11 < arduino2 && arduino2 < 21) {
			event = 1;
		}
		else if (21 < arduino2 && arduino2 < 31) {
			event = 2;
		}
		else if (31 < arduino2 && arduino2 < 41) {
			event = 3;
		}
		OnEnChangeEdit1();

		str = "";
	}
	return 0;
}



//WM_TIMER 죽이는 함수
void Piano::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	KillTimer(1); //1을 죽임
	KillTimer(2); //2를 죽임
}

//WM_TIMER작동하는 동안 쓰이는 함수
void Piano::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 2) {
		deadline--;
	}
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nIDEvent == 1) { // 1에 대해서
		for (int i = 0; i < MAX_ROW; i++) {
			if (m_rect[i].top < LIMIT_HEIGHT) { //top이 500되기 전까지
				m_rect[i].top += MOVE_STEP; //top을 7픽셀씩 아래로 이동
				if (m_rect[i].bottom < LIMIT_HEIGHT) { //bottom이 500되기 전까지
					m_rect[i].bottom += MOVE_STEP; //bottom을 7픽셀씩 아래로 이동
				}
			}
			else { // top이 300이상이 될경우
				if (m_key_table[i] != MAX_COL) { // 만약 그냥 넘긴다면
					m_miss_count++; //놓칠 시 miss 카운트
					SetDlgItemTextW(IDC_STATIC1, _T("MISS"));//miss 출력
					if (m_miss_count >= 5) { // 5번 놓쳤을 경우
	
						GetDlgItem(IDC_STATIC1)->SetFont(&m_font); //폰트크기 지정
						SetDlgItemTextW(IDC_STATIC1, _T("GAME OVER")); //Game over 출력
						GAMEOVER = true; // 게임오버 이벤트 true
						//PlaySound(NULL, AfxGetInstanceHandle(), NULL); // 노래2 끄기
						PlaySound(MAKEINTRESOURCE(IDR_WAVE1), NULL, SND_ASYNC | SND_RESOURCE | SND_LOOP); //노래1 재생
						KillTimer(1); //5번 이상 실패시 게임 종료
					}
				}
				m_rect[i].top = TILE_HEIGHT * (-1) + MOVE_STEP; //top을 타일의 폭+7로 초기화
				m_rect[i].bottom = MOVE_STEP; //bottom을 7로 초기화
				m_key_table[i] = rand() % MAX_COL; //시간 갱신할때마다 어디서 나올 것인가
				if (i > 0 && m_key_table[i] == m_key_table[i - 1]) {
					m_key_table[i] = (m_key_table[i] + 3) % MAX_COL; //연속적이지 않게 하는 랜덤
				}
			}
		}
		Invalidate(); //매번 그림을 갱신
	}
	else CDialogEx::OnTimer(nIDEvent);
}

// 버튼 클릭 이벤트
void Piano::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (STOP == false) { //일시정지 상태에서 작동되는 것을 방지
		if (GAMEOVER == false) { //게임오버 상태에서 작동되는 것을 방지
			for (int i = 0; i < MAX_ROW; i++) { // 행만큼 반복
				if (m_rect[i].bottom >= LIMIT_HEIGHT) { //가장 아래쪽에 있는 타일
					if (m_key_table[i] != MAX_COL) { //타일이 제거되지 않을 줄일경우
						if (m_key_table[i] * TILE_WIDTH <= point.x && m_rect[i].top <= point.y //마우스 가능 범위
							&& (m_key_table[i] + 1) * TILE_WIDTH >= point.x && m_rect[i].bottom >= point.y) {
							m_key_table[i] = MAX_COL; //클릭한 것이 아예 안보이게 화면 밖으로 출력
							if (m_rect[i].top <= 360) {   //타이밍좋게 눌렀을 경우
								m_score += 10; //점수 높이기
								SetDlgItemTextW(IDC_STATIC1, _T("PERFECT")); //여기까지 글씨 출력 관련
							}
							else {  //좀 뒤늦게 눌렀을 경우
								m_score += 5; //점수 높이기
								SetDlgItemTextW(IDC_STATIC1, _T("GOOD")); //여기까지 글씨 출력 관련
							}
							SetDlgItemInt(IDC_STATIC3, m_score); //점수 출력
						}
						else {
							m_miss_count++; //잘못 클릭시 miss카운트
							SetDlgItemTextW(IDC_STATIC1, _T("MISS"));//miss 출력
							if (m_miss_count >= 5) {
								GetDlgItem(IDC_STATIC1)->SetFont(&m_font); //폰트크기 지정
								SetDlgItemTextW(IDC_STATIC1, _T("GAME OVER")); //game over 출력
								GAMEOVER = true; //게임오버 이벤트 true
								//PlaySound(NULL, AfxGetInstanceHandle(), NULL); //노래2 끄기
								PlaySound(MAKEINTRESOURCE(IDR_WAVE1), NULL, SND_ASYNC | SND_RESOURCE | SND_LOOP); //노래1 재생
								KillTimer(1); //5번 이상 실패시 게임 종료
							}
						}
					}
					break;
				}
			}
		}
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}

//아두이노랑 시리얼 통신할때 발생할 이벤트 예시
//현재 작동 안함
void Piano::OnEnChangeEdit1()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (deadline < 0) {
		overmiss = 7;
	}
	if (STOP == false) { //일시정지 상태에서 작동되는 것을 방지
		if (GAMEOVER == false) { //게임오버 상태에서 작동되는 것을 방지
			for (int i = 0; i < MAX_ROW; i++) { // 행만큼 반복
				if (m_rect[i].bottom >= LIMIT_HEIGHT) { //가장 아래쪽에 있는 타일
					if (m_key_table[i] != MAX_COL) { //타일이 제거되지 않을 줄일경우
						if (m_key_table[i] == event) {
							m_key_table[i] = MAX_COL; //클릭한 것이 아예 안보이게 화면 밖으로 출력
							if (m_rect[i].top <= 360) {   //타이밍좋게 눌렀을 경우
								m_score += 10; //점수 높이기
								SetDlgItemTextW(IDC_STATIC1, _T("PERFECT")); //perfect 출력
							}
							else {  //좀 뒤늦게 눌렀을 경우
								m_score += 5; //점수 높이기
								SetDlgItemTextW(IDC_STATIC1, _T("GOOD")); //Good 출력
							}
							SetDlgItemInt(IDC_STATIC3, m_score); //점수 출력
						}
						else {
							if (overmiss != event) {
								overmiss = event;
								deadline = 1;
								m_miss_count++; //잘못 클릭시 miss카운트
								SetDlgItemTextW(IDC_STATIC1, _T("MISS")); //miss 출력
								if (m_miss_count >= 5) {
									GetDlgItem(IDC_STATIC1)->SetFont(&m_font); //폰트크기 지정
									SetDlgItemTextW(IDC_STATIC1, _T("GAME OVER")); //Game over 출력
									GAMEOVER = true; //게임오버 이벤트 true
									//PlaySound(NULL, AfxGetInstanceHandle(), NULL); //노래2 끄기
									PlaySound(MAKEINTRESOURCE(IDR_WAVE1), NULL, SND_ASYNC | SND_RESOURCE | SND_LOOP); //노래1 재생
									KillTimer(1); //5번 이상 실패시 게임 종료
								}
							}
						}
					}
					break;
				}
			}
		}
	}
}

//홈버튼
void Piano::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	PlaySound(NULL, AfxGetInstanceHandle(), NULL); // 노래 끄기
	if (m_comm) {
		m_comm->Close();
	}
	SendMessage(WM_CLOSE, 0, 0);
}


//일시정지, 다시 재생 관련
void Piano::OnBnClickedButton1()
{
	if (RESTART == false) { //다시하기 작동중에 일시정지, 재시작이 작동되는걸 방지
		if (m_miss_count < 5) { //gameover이후 작동되는 것을 막음
			if (STOP == false) {
				//일시정지할 코드
				PlaySound(NULL, AfxGetInstanceHandle(), NULL); //노래2 끄기
				SetDlgItemTextW(IDC_STATIC1, _T("일시정지")); //일시정지 출력
				KillTimer(1); //멈추기
				STOP = true; //이벤트 stop값 조정
			}
			else if (STOP == true) {
				//다시 재생할 코드
				PlaySound(MAKEINTRESOURCE(IDR_WAVE2), NULL, SND_ASYNC | SND_RESOURCE | SND_LOOP); //노래2 재생
				SetDlgItemTextW(IDC_STATIC1, _T("재시작")); //재시작 출력
				SetTimer(1, 80, NULL); //재시작
				STOP = false; //이벤트 stop값 조정
			}
		}
	}
}

//wait를 위한 함수
void Piano::Wait(DWORD dwMillisecond) 
{
	MSG msg;
	DWORD dwStart;
	dwStart = GetTickCount();

	while (GetTickCount() - dwStart < dwMillisecond) {
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}

//다시하기
void Piano::OnBnClickedButton2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GAMEOVER = false;
	STOP = false;
	RESTART = true; //이벤트 restart값 조정
	m_miss_count = 0; //miss 초기화
	m_score = 0; //score 초기화
	GetDlgItem(IDC_STATIC1)->SetFont(&m_font_b); //폰트크기 지정
	SetDlgItemInt(IDC_STATIC3, m_score); //점수 출력
	srand((unsigned int)time(NULL)); // 매번 다른 rand 수 설정
	for (int i = 0; i < MAX_ROW; i++) { //타일의 좌표 지정
		m_rect[i].SetRect(100, i * TILE_HEIGHT * (-1) - TILE_HEIGHT, 100 + TILE_WIDTH, i * TILE_HEIGHT * (-1)); //타일의 좌표 지정식
		m_key_table[i] = rand() % MAX_COL; // 초기 위치 어디서 나올 것인가
		if (i > 0 && m_key_table[i] == m_key_table[i - 1]) {
			m_key_table[i] = (m_key_table[i] + 3) % MAX_COL; //연속적이지 않게 하는 랜덤
		}
	}
	SetDlgItemTextW(IDC_STATIC1, _T("3")); //3출력
	KillTimer(1); //멈추기
	Wait(1000); //딜레이 1초
	SetDlgItemTextW(IDC_STATIC1, _T("2")); //2출력
	Wait(1000); //딜레이 1초
	SetDlgItemTextW(IDC_STATIC1, _T("1")); //1출력
	Wait(1000); //딜레이 1초
	SetDlgItemTextW(IDC_STATIC1, _T("GO")); //GO출력
	SetTimer(1, 80, NULL); //재시작
	RESTART = false; //이벤트 restart값 조정

	PlaySound(MAKEINTRESOURCE(IDR_WAVE2), NULL, SND_ASYNC | SND_RESOURCE | SND_LOOP); //노래2 재생
}

//점수 공유하기
void Piano::OnBnClickedButton3()
{
	((CMFCApplication2Dlg*)GetParent())->OnBnClickedButton1(m_score);
}

//X버튼
void Piano::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	PlaySound(NULL, AfxGetInstanceHandle(), NULL); //노래 끄기
	if (m_comm) {
		m_comm->Close();
	}
	CDialogEx::OnClose();
}