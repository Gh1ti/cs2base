#pragma once
#include "memory.h"
#include "Offsets.h"
#include "Entity.h"
#include "Vector.h"

namespace g_memory {
	Memory mem = Memory("cs2.exe");
}

namespace dlls {
	uintptr_t client = g_memory::mem.GetModuleAddress("client.dll");
	uintptr_t engine = g_memory::mem.GetModuleAddress("engine2.dll");
}

namespace interfaces {
	uintptr_t entity_list = g_memory::mem.Read<uintptr_t>(dlls::client + offsets::dwEntityList);
	view_matrix_t vm = g_memory::mem.Read<view_matrix_t>(dlls::client + offsets::dwViewMatrix);
}

float* StringToFloatArray(const std::string& str) {
	std::istringstream iss(str);
	std::vector<float> floats;

	float value;
	char comma;

	while (iss >> value) {
		floats.push_back(value);
		if (!(iss >> comma)) {
			break;  // No more characters after the last float
		}
	}

	size_t size = floats.size();

	float* floatArray = new float[size];
	for (size_t i = 0; i < size; i++) {
		floatArray[i] = floats[i];
	}

	return floatArray;

	//delete[] floatArray;
}

float StringToFloat(const std::string& str) {
	return std::stof(str);
}