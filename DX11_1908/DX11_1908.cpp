// DX11_1908.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "Framework.h"
#include "Program/Program.h"
#include "DX11_1908.h"

#define MAX_LOADSTRING 100

// 전역 변수:
Program* program = nullptr;

HINSTANCE hInst;                                // 현재 인스턴스입니다.
HWND hWnd;
POINT M_position;
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void Step()
{
    ImGuiIO& io = ImGui::GetIO();

    Keyboard::Get()->Update();
    Timer::Get()->Update();

    program->Update();

    program->PreRender();

    Device::Get()->SetRenderTarget();

    Device::Get()->Clear();
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    program->Render();
    program->PostRender();

    ImGui::Render();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    Device::Get()->Present();
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.
    //IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    // Imgui_impl_dx11랑 imgui_impl_win32가 저희 거랑 doking모드 버전이랑 달라서 바꿨습니다
    // 업데이트가 살짝 덜 된 버전으로 하고 있었던 것 같습니다. 
    ImGuiIO& io = ImGui::GetIO(); //(void)io;

    //io.Fonts->AddFontFromFileTTF("NotoSansCJKjp-Medium.otf", 20.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
   // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\designhouseOTFLight.otf", 18.0f, NULL, io.Fonts->GetGlyphRangesKorean());

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_::ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_::ImGuiConfigFlags_ViewportsEnable;
#if 1
    io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;     // FIXME-DPI: THIS CURRENTLY DOESN'T WORK AS EXPECTED. DON'T USE IN USER APP!
    io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports; // FIXME-DPI
#endif


    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    //ImGui::StyleColorsDark();
    ImGui::StyleColorsClassic();

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_DX111908, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DX111908));

    MSG msg = {};
    
    Device::Create(hWnd);

    ImGui_ImplWin32_Init(hWnd);
    ImGui_ImplDX11_Init(DEVICE, DC);

    Keyboard::Create();
    Timer::Create();
    SceneManager::Create();
    SoundManager::Create();

    Environment::Create();

    program = new Program();


    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            Step();
        }
    }

    delete program;
    
    Shader::Delete();
    Texture::Delete();

    SceneManager::Delete();
    SoundManager::Delete();

    Environment::Delete();

    Keyboard::Delete();
    Timer::Delete();

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    Device::Delete();

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DX111908));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_DX111908);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   RECT rc = { 0, 0, WIN_WIDTH, WIN_HEIGHT };
   AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);

   hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      WIN_START_X, WIN_START_Y, 
       rc.right - rc.left, rc.bottom - rc.top,
       nullptr, nullptr, hInstance, nullptr);

   SetMenu(hWnd, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//

IMGUI_IMPL_API LRESULT  ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
        return true;

    switch (message)
    {
    case WM_CREATE:
        {
            g_mouse->SetWindow(hWnd);
        }   
        break;
    case WM_ACTIVATEAPP:
        {

             Mouse::ProcessMessage(message, wParam, lParam);
        }
        break;
    case WM_INPUT:
        {
            Mouse::ProcessMessage(message, wParam, lParam);
        }
        break;
    case WM_MOUSEMOVE:
    {
        Mouse::ProcessMessage(message, wParam, lParam);
        Keyboard::Get()->SetMouse(LOWORD(lParam), HIWORD(lParam));
        M_position.x = LOWORD(lParam);
        M_position.y = HIWORD(lParam);
    }  
        break;
    case WM_LBUTTONDOWN:
        break;
    case WM_LBUTTONUP:
        break;
    case WM_RBUTTONDOWN:
        break;
    case WM_RBUTTONUP:
        break;
    case WM_MBUTTONDOWN:
        break;
    case WM_MBUTTONUP:
        break;
    case WM_MOUSEWHEEL:
    {
        short value = (short)HIWORD(wParam);
        Keyboard::Get()->SetWheel((float)value);
    }
        break;
    case WM_XBUTTONDOWN:
        break;
    case WM_XBUTTONUP:
        break;
    case WM_MOUSEHOVER:
        Mouse::ProcessMessage(message, wParam, lParam);
        break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    //case WM_MOUSEMOVE:
    //    Keyboard::Get()->SetMouse(LOWORD(lParam), HIWORD(lParam));
    //    break;
    //case WM_MOUSEWHEEL:
    //{
    //    short value = (short)HIWORD(wParam);
    //    Keyboard::Get()->SetWheel((float)value);
    //}
    //    break;
    case WM_PAINT:
        {
            if (program != nullptr)
                Step();
        }
        break;
    case WM_SIZE:
        {
            if (Device::Get() == nullptr)
                break;
            
            if (wParam != SIZE_MINIMIZED)
            {
                RECT rc;
                GetClientRect(hWnd, &rc);
                UINT width = rc.right;
                UINT height = rc.bottom;

                Device::Get()->Resize(width, height);
            }
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}