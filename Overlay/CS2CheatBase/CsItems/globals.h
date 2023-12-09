#pragma once
#include <fstream>
#include <vector>
#include <sstream>

namespace globals {
	bool Home = true;
	bool Aimbot = false;
	bool Visuals = false;
	bool Config = false;
	bool Is_Visible = true;
	bool Createing = false;
}

namespace Aimbot {
	bool enabled = false;
	float fov = 0.f;
	float smoothness = 1.f;
	bool fov_circle = false;
}

namespace Visuals {
	bool esp = false;
	bool box = false;
	bool hp_bar = false;
	bool hp_num = false;
	bool name = false;
	bool head_dot = false;
	bool ppdot = false;
	float esp_col[] = { 1.f,1.f,1.f };
	float bar_col[] = { 1.f,1.f,1.f };
	float num_col[] = { 1.f,1.f,1.f };
	float head_col[] = { 1.f,1.f,1.f };
	float pp_col[] = { 1.f,1.f,1.f };
	float name_col[] = { 1.f,1.f,1.f };
}

namespace Config {
	char** name;
}

struct ConfigData {
	bool* boolVar;
	float* floatVar;
	float* floatArray;  // Pointer to a float array
	const char* varName;  // Optional: You can use this to print the variable name
};

ConfigData config[] = {
	{&Aimbot::enabled,nullptr,nullptr,"Aimbot"},
	{&Aimbot::fov_circle,nullptr,nullptr,"Fov Circle"},
	{nullptr,&Aimbot::fov,nullptr,"Fov"},
	{nullptr,&Aimbot::smoothness,nullptr,"Smoothness"},
	{&Visuals::esp,nullptr,nullptr,"Esp"},
	{&Visuals::box,nullptr,nullptr,"Box"},
	{&Visuals::hp_bar,nullptr,nullptr,"Hp Bar"},
	{&Visuals::hp_num,nullptr,nullptr,"Hp Num"},
	{&Visuals::name,nullptr,nullptr,"Name"},
	{&Visuals::head_dot,nullptr,nullptr,"Head Dot"},
	{&Visuals::ppdot,nullptr,nullptr,"PP Dot"},
	{nullptr,nullptr,Visuals::esp_col,"Esp Color"},
	{nullptr,nullptr,Visuals::bar_col,"Bar Color"},
	{nullptr,nullptr,Visuals::num_col,"Number Color"},
	{nullptr,nullptr,Visuals::head_col,"Head Color"},
	{nullptr,nullptr,Visuals::pp_col,"PP Color"},
	{nullptr,nullptr,Visuals::name_col,"Name Color"},
};

void Write_Config(std::string path) {
	std::fstream FileStream(path, std::ios::out | std::ios::trunc);
	int configSize = sizeof(config) / sizeof(config[0]);
	for (int index = 0; index < configSize; ++index) {
		FileStream << "\"" << config[index].varName << "\":";
		if (config[index].boolVar != nullptr) {
			FileStream << (*config[index].boolVar ? "true" : "false");
		}
		else if (config[index].floatVar != nullptr) {
			FileStream << *config[index].floatVar;
		}
		else if (config[index].floatArray != nullptr) {
			for (int i = 0; i < 3; i++) {
				FileStream << config[index].floatArray[i];
				if (i < 2) {
					FileStream << ",";
				}
			}
		}

		if (index != configSize - 1) {
			FileStream << "\n";
		}
	}
}

std::vector<std::string> ValsAfterColon(const std::string& path) {
	std::ifstream fileStream(path);
	std::vector<std::string> Store;
	std::string line;
	while (std::getline(fileStream, line)) {
		std::size_t found = line.find(':');
		if (found != std::string::npos && found + 1 < line.length()) {
			std::string contentAfterColon = line.substr(found + 1);
			Store.push_back(contentAfterColon);
		}
	}

	fileStream.close();
	return Store;
}

ImU32 FloatColorToU32(float r, float g, float b, float a) {
	int intR = static_cast<int>(r * 255.0f);
	int intG = static_cast<int>(g * 255.0f);
	int intB = static_cast<int>(b * 255.0f);
	int intA = static_cast<int>(a * 255.0f);

	return IM_COL32(intR, intG, intB, intA);
}