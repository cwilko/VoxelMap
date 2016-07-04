
// InitWindows.cpp
//
// 


HWND InitWindows(HINSTANCE hThisInst, HINSTANCE hPrevInst,int nWinMode, HWND hwnd) {


	WNDCLASS wcl;

	wcl.hInstance = hThisInst;
	wcl.lpszClassName = WINDOW_CLASS_NAME;
	wcl.lpfnWndProc = WindowFunc;
	wcl.style = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;

	wcl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	//wcl.hIconSm = LoadIcon(NULL, IDI_WINLOGO);
	wcl.hCursor = LoadCursor(NULL, IDC_ARROW);

	wcl.lpszMenuName = WINDOW_CLASS_NAME;
	wcl.cbClsExtra = 0;
	wcl.cbWndExtra = 0;	

	wcl.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);
	

	if(!RegisterClass(&wcl)) return NULL;

	if(!(hwnd = CreateWindowEx (
		0,
		WINDOW_CLASS_NAME,
		"Landscape Generator v1.7",
		WS_VISIBLE | WS_OVERLAPPED,
		0,
		0,
		GetSystemMetrics(SM_CXSCREEN),
		GetSystemMetrics(SM_CYSCREEN),
		NULL,
		NULL,
		hThisInst,
		NULL
		))) return 0;

	ShowWindow(hwnd, nWinMode);
	UpdateWindow(hwnd);
	SetFocus(hwnd);
	ShowCursor(0);

	return hwnd;

}
