#pragma comment(lib,"d3d11.lib")
#include "FSLib.h"

//グローバル変数
HWND hWnd = NULL;
ID3D11Device* Device = NULL;
ID3D11DeviceContext* DeviceContext = NULL;
IDXGISwapChain* SwapChain = NULL;
FRAME_RATE FrameRateManager;
KEYBOARD KeyBoard;
CAMERA Camera;
LIGHT Light;
CONSTANT_MANAGER ConstantManager;
FONT_MANAGER FontManager;
FbxManager* fbxManager = NULL;
MSG msg;

WINDOW_CONTROLLER wndCont("TITLE", GetColor(0.f,0.f,0.f,1.f), 800, 600);
RENDER_INFO RenderInfo;

//ID3D11RasterizerState* RasterizerState;

//ダイレクト3Dの初期化関数
HRESULT InitD3D(HWND hWnd)
{
	// デバイスとスワップチェーンの作成
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;         //バックバッファの数
	sd.BufferDesc.Width = wndCont.GetWidth();     //バッファの幅
	sd.BufferDesc.Height = wndCont.GetHeight();;    //バッファの高さ
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //バッファのフォーマット
	sd.BufferDesc.RefreshRate.Numerator = 60;   //リフレッシュレート
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	D3D_FEATURE_LEVEL  FeatureLevel = D3D_FEATURE_LEVEL_11_0;


	if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0,
		&FeatureLevel, 1,
		D3D11_SDK_VERSION, &sd, &SwapChain, &Device, NULL, &DeviceContext))){
		return FALSE;
	}

	return S_OK;
}

//終了時解放処理
void Cleanup()
{
	SwapChain->Release();
	DeviceContext->Release();
	Device->Release();
	fbxManager->Destroy();
}

//メッセージプロシージャ
LRESULT CALLBACK MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY://終了時
		PostQuitMessage(0);
		break;
		
	case WM_ACTIVATEAPP:
		Keyboard::ProcessMessage(msg, wParam, lParam);
		break;

	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
		Keyboard::ProcessMessage(msg, wParam, lParam);
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

bool WindowInitialize(void) {
	bool flag;
	// ロケール設定
	locale::global(locale("japanese"));

	ZeroMemory(&msg, sizeof(msg));

	//ウインドウクラスの登録
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
		GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
		"Window1", NULL };
	RegisterClassEx(&wc);
	//タイトルバーとウインドウ枠の分を含めてウインドウサイズを設定
	RECT rect;
	SetRect(&rect, 0, 0, wndCont.GetWidth(), wndCont.GetHeight());
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX, FALSE);
	rect.right = rect.right - rect.left;
	rect.bottom = rect.bottom - rect.top;
	rect.top = 0;
	rect.left = 0;
	//ウインドウの生成
	hWnd = CreateWindow("Window1", wndCont.GetTitle(),
		WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, rect.right, rect.bottom,
		NULL, NULL, wc.hInstance, NULL);

	if (flag = SUCCEEDED(InitD3D(hWnd))) {
		//ウインドウ表示
		ShowWindow(hWnd, SW_SHOW);
		UpdateWindow(hWnd);
	}

	return flag;
}

bool FSLibInitialize(WINDOW_CONTROLLER const& wCont) {
	wndCont = wCont;
	
	WindowInitialize();

	// FBX SDK
	fbxManager = FbxManager::Create();
	FbxIOSettings* IOS = FbxIOSettings::Create(fbxManager, IOSROOT);
	fbxManager->SetIOSettings(IOS);

	RenderInfo.Init();
	RenderInfo.Rasterizer()->SetDefault();

	Camera.Init(wndCont.GetWidth(), wndCont.GetHeight());
	ConstantManager.Init(Device);
	FontManager.Init();
	
	return true;
}

bool ProcessMessage(void) {
	int a = 0;
	a++;

	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{

		TranslateMessage(&msg);
		DispatchMessage(&msg);

		if (msg.message == WM_QUIT)return false;
	}

	return true;
}

bool ScreenFlip(void) {
	SwapChain->Present(1, 0);//画面更新(バックバッファをフロントに)
	FrameRateManager.Anchor();
	return true;
}

bool FSLibFinalize(void) {
	Cleanup();
	return true;
}

CAMERA* GetCamera(void) { return &Camera; }

LIGHT* GetLight(void) { return &Light; }

CONSTANT_MANAGER* GetConstantManager(void) { return &ConstantManager; }

KEYBOARD* GetKeyboard(void) { return &KeyBoard; }

FONT_MANAGER* GetFontManager(void) { return &FontManager; }

FRAME_RATE* GetFrameRateManager(void) { return &FrameRateManager; }

RENDER_INFO* GetRenderInfo(void) { return &RenderInfo; }
