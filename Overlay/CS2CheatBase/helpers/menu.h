#pragma once

#define or || 
#define and &&
#define non_nul !=nullptr
#define non_0f != 0.00f
#define non_0 !=0

char config_name[256] = "Nigger";
bool item_clicked;
int no_files;
int selected_cfg = 0;

char** files = listFiles(directoryPath, no_files);


void ImGuiWindowSetup() {
	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
	style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);  // Set the title bar background color to black
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);  // Set the active title bar background color to black
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f); //Sets the window bg to black
	style.Colors[ImGuiCol_Button] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.3f, 0.0f, 0.3f, 1.0f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.5f, 0.0f, 0.5f, 1.0f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.3f, 0.0f, 0.3f, 1.0f);
	style.ItemSpacing = ImVec2(0, 10);
	style.ChildRounding = 13.f;
	style.FrameRounding = 3.f;
	style.GrabRounding = 3.f;
	style.WindowRounding = 15.f;
	style.TabRounding = 15.f;
	style.PopupRounding = 6.f;
	ImGui::SetNextWindowPos(ImVec2(1920/2,1080/2));
}


void BeginImGui() {
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGuiWindowSetup();
	ImGui::NewFrame();
	ImGui::SetNextWindowSize(ImVec2(390, 400));
}

void CreateImGuiMenu(HWND window, ImFont* font) {
	ImGui::PushFont(font);
	if (globals::Is_Visible) {
		ImGui::Begin("Nero.cc", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);
		if (ImGui::Button("Home", ImVec2(80, 40))) {
			globals::Home = true;
			globals::Aimbot = false;
			globals::Visuals = false;
			globals::Config = false;
		}
		ImGui::SameLine(80.f, 20.f);
		if (ImGui::Button("Aimbot", ImVec2(80, 40))) {
			globals::Home = false;
			globals::Aimbot = true;
			globals::Visuals = false;
			globals::Config = false;
		}
		ImGui::SameLine(160.f, 40.f);
		if (ImGui::Button("Visuals", ImVec2(80, 40))) {
			globals::Home = false;
			globals::Aimbot = false;
			globals::Visuals = true;
			globals::Config = false;
		}
		ImGui::SameLine(240.f, 60.f);
		if (ImGui::Button("Config", ImVec2(80, 40))) {
			globals::Home = false;
			globals::Aimbot = false;
			globals::Visuals = false;
			globals::Config = true;
		}
		ImGui::Separator();

		if (globals::Home) {
			ImGui::Text("Welcome to Nero.cc");
			ImGui::Text("Coded by Gh1ti");
			ImGui::Text("Enjoy :3");
			//ImGui::Text("Helped by Adrian Radulescu aka Elon Musk");
		}

		if (globals::Aimbot) {
			ImGui::Checkbox("Enable Aimbot", &Aimbot::enabled);
			if (Aimbot::enabled) {
				ImGui::Text("Hold Left-Alt to use");
				ImGui::Checkbox("Fov Circle", &Aimbot::fov_circle);
				ImGui::SliderFloat("FOV", &Aimbot::fov, 0, 180, "%.1f", ImGuiSliderFlags_NoInput);
				ImGui::SliderFloat("Smoothnes (%)", &Aimbot::smoothness, 1, 100, "%.1f", ImGuiSliderFlags_NoInput);
			}
		}
		if (globals::Visuals) {
			ImGui::Checkbox("Enable Esp", &Visuals::esp);
			if (Visuals::esp) {
				ImGui::ColorEdit3("Box Color", Visuals::esp_col, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
				ImGui::SameLine(0, 5);
				ImGui::Checkbox("Enable Box", &Visuals::box);
				ImGui::ColorEdit3("Name Color", Visuals::name_col, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
				ImGui::SameLine(0, 5);
				ImGui::Checkbox("Name", &Visuals::name);
				ImGui::ColorEdit3("Health Bar Color", Visuals::bar_col, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
				ImGui::SameLine(0, 5);
				ImGui::Checkbox("Enable Health Bar", &Visuals::hp_bar);
				ImGui::ColorEdit3("Health Number Color", Visuals::num_col, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
				ImGui::SameLine(0, 5);
				ImGui::Checkbox("Enable Health Number", &Visuals::hp_num);
				ImGui::ColorEdit3("Head Dot Color", Visuals::head_col, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
				ImGui::SameLine(0, 5);
				ImGui::Checkbox("Enable Head Dot", &Visuals::head_dot);
				ImGui::ColorEdit3("PP Dot Color", Visuals::pp_col, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
				ImGui::SameLine(0, 5);
				ImGui::Checkbox("Enable PP Dot", &Visuals::ppdot);
			}
		}
		if (globals::Config) {
			char cfg[5] = ".cfg";
			char path_separator[2] = "\\";
			ImGui::Text("Config Name");
			ImGui::InputText(" ", config_name, 256);
			ImGui::SameLine(0, 10);
			if (ImGui::Button("Create Config", ImVec2(100, 20))) {
				std::string path = directoryPath + path_separator + config_name + cfg;
				if (!fileExists(path)) {
					Write_Config(path);
					files = listFiles(directoryPath, no_files);
				}
				
			}
			if (ImGui::BeginCombo("Select a config", files[selected_cfg])) {
				for (int i = 0; i < no_files; ++i) {
					const bool is_selected = (selected_cfg == i);
					if (ImGui::Selectable(files[i], is_selected)) {
						selected_cfg = i;
					}
					if (is_selected) {
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}
			if (ImGui::Button("Save Config", ImVec2(120, 35))) {
				std::string path = directoryPath + path_separator + files[selected_cfg];
				if (fileExists(path)) {
					Write_Config(path);
					files = listFiles(directoryPath, no_files);
				}
				
			}
			ImGui::SameLine(0, 6);
			if (ImGui::Button("Load Config", ImVec2(120, 35))) {
				//*config[0].boolVar = true;
				//*config[2].floatVar = 15.3;
				//config[11].floatArray[0] = 0.5;
				//config[11].floatArray[1] = 0.5;
				//config[11].floatArray[2] = 0.5;
				std::string path = directoryPath + path_separator + files[selected_cfg];
				if (fileExists(path)) {
					std::vector<std::string> cfg = ValsAfterColon(path);
					int cfg_size = sizeof(config) / sizeof(config[0]);
					for (int i = 0; i < cfg_size; i++) {
						if (config[i].boolVar non_nul) {
							*config[i].boolVar = (cfg[i] == "true");
						}
						if (config[i].floatVar non_nul) {
							*config[i].floatVar = StringToFloat(cfg[i]);
						}
						if (config[i].floatArray non_nul) {
							float* float_arr = StringToFloatArray(cfg[i]);
							for (int j = 0; j < 3; j++) {
								config[i].floatArray[j] = float_arr[j];
							}
						}
					}
				}
			}
			ImGui::SameLine(0, 6);
			if (ImGui::Button("Reload Configs", ImVec2(120, 35))) {
				files = listFiles(directoryPath, no_files);
			}
			if (ImGui::Button("Delete Config", ImVec2(120, 35))) {
				std::string path = directoryPath + "\\" + files[selected_cfg];
				RemoveFiles(path);
				std::this_thread::sleep_for(std::chrono::milliseconds(5));
				files = listFiles(directoryPath, no_files);
			}
			ImGui::SameLine(0, 6);
			if (ImGui::Button("Open Config Folder", ImVec2(140, 35))) {
				ShellExecuteA(NULL, "open", "explorer.exe", directoryPath.c_str(), NULL, SW_SHOWNORMAL);
			}
		}
		SetWindowLongPtr(window, GWL_EXSTYLE, WS_EX_TOPMOST);
		SetWindowPos(window, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
		ImGui::End();
	}
	ImGui::PopFont();
}