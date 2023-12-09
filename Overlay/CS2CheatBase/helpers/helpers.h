#pragma once
namespace fs = std::filesystem;

bool GetWindowInFocus(char* name) {
	HWND mainWindowHandle = FindWindow(nullptr, name);
	HWND currentForegroundWindow = GetForegroundWindow();
	return currentForegroundWindow == mainWindowHandle;
}

const char* appDataPath = std::getenv("APPDATA");
std::string directoryPath = std::string(appDataPath) + "\\CatWare Configs";
auto nafing = CreateDirectoryA(directoryPath.c_str(), NULL);

bool Keybinds(ImGuiKey key) {
	return ImGui::IsKeyPressed(key, false);
}

void HideWindow(HWND window) {
	if (!globals::Is_Visible) {
		SetWindowLongPtr(window, GWL_EXSTYLE, WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED);
	}
	else {
		SetWindowLongPtr(window, GWL_EXSTYLE, WS_EX_TOPMOST);
	}
	SetWindowPos(window, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
}

bool fileExists(const std::string& filePath) {
	std::ifstream file(filePath);
	return file.good();
}

char** listFiles(const std::string& path, int& fileCount) {
	WIN32_FIND_DATA findFileData;
	HANDLE hFind = FindFirstFile((path + "/*").c_str(), &findFileData);

	if (hFind == INVALID_HANDLE_VALUE) {
		std::cerr << "Error opening directory" << std::endl;
		return nullptr;
	}

	std::vector<std::string> filenames;
	do {
		std::string filename = findFileData.cFileName;
		if (filename != "." && filename != "..") {
			filenames.push_back(filename);
		}
	} while (FindNextFile(hFind, &findFileData) != 0);

	FindClose(hFind);

	fileCount = static_cast<int>(filenames.size());

	char** files = new char* [fileCount];
	for (int i = 0; i < fileCount; ++i) {
		files[i] = new char[filenames[i].size() + 1];
		strcpy(files[i], filenames[i].c_str());
	}

	return files;
}

float Distance(ImVec2 pos, ImVec2 screen_center) {
	return std::sqrt(std::pow(pos.x - screen_center.x, 2) + std::pow(pos.y - screen_center.y, 2));
}

bool IsInFov(ImVec2 pos, ImVec2 screen_center, float fov) {

	float dist = std::sqrt(std::pow(pos.x-screen_center.x,2)+ std::pow(pos.y-screen_center.y,2));
	return dist <= fov;
}

void RemoveFiles(std::string path) {
	DeleteFileA(path.c_str());
}


void FinishProggram(IDXGISwapChain* swap_chain, 
	ID3D11DeviceContext* device_context, 
	ID3D11RenderTargetView* render_target_view, 
	ID3D11Device* device,
	HWND window,
	WNDCLASSEXW wc) {
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();

	ImGui::DestroyContext();
	if (swap_chain) {
		swap_chain->Release();
	}

	if (device_context) {
		device_context->Release();
	}

	if (device) {
		device->Release();
	}

	if (render_target_view) {
		render_target_view->Release();
	}

	DestroyWindow(window);
	UnregisterClassW(wc.lpszClassName, wc.hInstance);

}