#pragma once
#include "includes.h"
#include "fonts/Fonts.h"
#include "http.h"
#include "hwid.h"


#define or || 
#define and &&
#define non_nul !=nullptr
#define non_0f != 0.00f
#define non_0 !=0

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

ImFont* verdana;
ImFont* pixel;
ImFont* impact;

LRESULT CALLBACK window_procedure(HWND window, UINT message, WPARAM w_param, LPARAM l_param) {
	if (ImGui_ImplWin32_WndProcHandler(window, message, w_param, l_param)) {
		return 0L;
	}

	if (message == WM_DESTROY) {
		PostQuitMessage(0);
		return 0L;
	}

	return DefWindowProc(window, message, w_param, l_param);
}
int hackloop(HINSTANCE instance, int cmd_show) {

	const int targetFPS = 120;
	const DWORD frameTime = 1000 / targetFPS;
	if (!fileExists(directoryPath + "\\default.cfg")) {
		std::fstream Filestream;
		Filestream.open(directoryPath + "\\default.cfg", std::ios::out | std::ios::trunc);
		Filestream.close();
	}

	WNDCLASSEXW wc{};
	wc.cbSize = sizeof(WNDCLASSEXW);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = window_procedure;
	wc.hInstance = instance;
	wc.lpszClassName = L"External Overlay Class";

	RegisterClassExW(&wc);

	const HWND window = CreateWindowExW(
		WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED,
		wc.lpszClassName,
		L"External Overlay",
		WS_POPUP,
		0,
		0,
		1920,
		1080,
		nullptr,
		nullptr,
		wc.hInstance,
		nullptr
	);
	SetLayeredWindowAttributes(window, RGB(0, 0, 0), BYTE(255), LWA_ALPHA);

	{
		RECT client_area{};
		GetClientRect(window, &client_area);

		RECT window_area{};
		GetWindowRect(window, &window_area);

		POINT diff{};
		ClientToScreen(window, &diff);
		const MARGINS margins{
			window_area.left + (diff.x - window_area.left),
			window_area.top + (diff.y - window_area.top),
			client_area.right,
			client_area.bottom
		};

		DwmExtendFrameIntoClientArea(window, &margins);
	}

	DXGI_SWAP_CHAIN_DESC sd{};
	sd.BufferDesc.RefreshRate.Numerator = 60U;
	sd.BufferDesc.RefreshRate.Denominator = 1U;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.SampleDesc.Count = 1U;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 2U;
	sd.OutputWindow = window;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	constexpr D3D_FEATURE_LEVEL levels[2]{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_0
	};

	ID3D11Device* device{ nullptr };
	ID3D11DeviceContext* device_context{ nullptr };
	IDXGISwapChain* swap_chain{ nullptr };
	ID3D11RenderTargetView* render_target_view{ nullptr };
	D3D_FEATURE_LEVEL level{};

	//creating device
	D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0U,
		levels,
		2U,
		D3D11_SDK_VERSION,
		&sd,
		&swap_chain,
		&device,
		&level,
		&device_context
	);

	ID3D11Texture2D* back_buffer{ nullptr };
	swap_chain->GetBuffer(0U, IID_PPV_ARGS(&back_buffer));

	if (back_buffer) {
		device->CreateRenderTargetView(back_buffer, nullptr, &render_target_view);
		back_buffer->Release();
	}
	else
	{
		return 1;
	}

	ShowWindow(window, cmd_show);
	UpdateWindow(window);

	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(device, device_context);

	bool runing = true;
	//LoadFonts(verdana, pixel, impact);
	ImGuiIO io = ImGui::GetIO();

	ImFontConfig verdana_cfg;
	verdana_cfg.RasterizerMultiply = 1.5f;
	verdana_cfg.OversampleH = 3;
	verdana_cfg.FontBuilderFlags = 2;

	verdana = io.Fonts->AddFontFromMemoryTTF(Verdana, sizeof(Verdana), 16,&verdana_cfg);
	pixel = io.Fonts->AddFontFromMemoryTTF(Pixel, sizeof(Pixel), 7);
    impact = io.Fonts->AddFontFromMemoryTTF(Impact, sizeof(Impact), 16);

	while (runing && g_memory::mem.GetModuleAddress("client.dll") && (!GetAsyncKeyState(VK_DELETE))) {
		MSG msg;
		while (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
			{
				runing = false;
			}
		}
		if (!runing) {
			break;
		}

		BeginImGui();

		CreateImGuiMenu(window,verdana);

		
		if (Keybinds(ImGuiKey_Insert)) {
			globals::Is_Visible = !globals::Is_Visible;
		}
		ImGui::SetNextWindowSize(ImVec2(390, 400));
		char window1[] = "E";
		char name[] = "Counter-Strike 2";
		float screenx = 1920;
		float screeny = 1080;
		char cs2[] = "Counter-Strike 2";
		ImVec2 screen_center = ImVec2(screenx / 2, screeny / 2);

		ImDrawList* g_render = ImGui::GetBackgroundDrawList();
		interfaces::vm = g_memory::mem.Read<view_matrix_t>(dlls::client + offsets::dwViewMatrix);
		interfaces::entity_list = g_memory::mem.Read<uintptr_t>(dlls::client + offsets::dwEntityList);

		if (!(GetWindowInFocus(cs2) or GetWindowInFocus(window1))) goto end;

		 HideWindow(window);
		 if (GetWindowInFocus(cs2)) {
			 hacks::Aimbot(Aimbot::fov, screen_center);
			 if (Aimbot::enabled) {
				 if (Aimbot::fov_circle and Local_Player().Health() >= 0) {
					 ImGui::GetBackgroundDrawList()->AddCircle(screen_center, Aimbot::fov, ImColor(255, 255, 255), 360);
				 }
			 }
		 }
		 if (Visuals::esp) {
			 hacks::DoEsp(verdana, g_render);
		 }
		 if (Aimbot::enabled) {

		 }
		 

		end:

		ImGui::Render();

		constexpr float color[4]{ 0.f,0.f,0.f,0.f };
		device_context->OMSetRenderTargets(1U, &render_target_view, nullptr);
		device_context->ClearRenderTargetView(render_target_view, color);

		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		swap_chain->Present(1U, 0U);

		DWORD frameStart = GetTickCount();

		// Limit the FPS by sleeping for the remaining time
		DWORD frameDuration = GetTickCount() - frameStart;
		if (frameDuration < frameTime) {
			Sleep(frameTime - frameDuration);
		}
	}

	FinishProggram(swap_chain, device_context, render_target_view, device,window,wc);

	return 0;
}