#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <TlHelp32.h>
#include <string_view>
class Memory
{
private:
	std::uintptr_t processId = 0;
	void* processHandle = nullptr;

public:
	// Constructor that finds the process id
	// and opens a handle
	Memory(std::string_view processName) noexcept
	{
		::PROCESSENTRY32 entry = { };
		entry.dwSize = sizeof(::PROCESSENTRY32);

		const HANDLE snapShot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

		while (::Process32Next(snapShot, &entry))
		{
			if (!processName.compare(entry.szExeFile))
			{
				processId = entry.th32ProcessID;
				processHandle = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
				break;
			}
		}

		// Free handle
		if (snapShot)
			::CloseHandle(snapShot);
	}

	// Destructor that frees the opened handle
	~Memory()
	{
		if (processHandle)
			::CloseHandle(processHandle);
	}

	// Returns the base address of a module by name
	std::uintptr_t GetModuleAddress(std::string_view moduleName) const noexcept
	{
		::MODULEENTRY32 entry = { };
		entry.dwSize = sizeof(::MODULEENTRY32);

		const HANDLE snapShot = ::CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, processId);

		std::uintptr_t result = 0;

		while (::Module32Next(snapShot, &entry))
		{
			if (!moduleName.compare(entry.szModule))
			{
				result = reinterpret_cast<std::uintptr_t>(entry.modBaseAddr);
				break;
			}
		}

		if (snapShot)
			::CloseHandle(snapShot);

		return result;
	}

	// Read process memory
	template <typename T>
	const T Read(const std::uintptr_t address) const noexcept
	{
		T value = { };
		::ReadProcessMemory(processHandle, reinterpret_cast<const void*>(address), &value, sizeof(T), NULL);
		return value;
	}

	// Write process memory
	template <typename T>
	void Write(const std::uintptr_t address, const T& value) const noexcept
	{
		::WriteProcessMemory(processHandle, reinterpret_cast<void*>(address), &value, sizeof(T), NULL);
	}

	const bool ReadRaw(uintptr_t address, void* buffer, size_t size)
	{
		SIZE_T bytesRead;
		if (ReadProcessMemory(processHandle, reinterpret_cast<LPCVOID>(address), buffer, size, &bytesRead))
		{
			return bytesRead == size;
		}
		return false;
	}


	std::string ReadString(uintptr_t address, size_t size) {
		char* buffer = new char[size];
		if (ReadRaw(address, buffer, size)) {
			// Ensure null-termination for string
			buffer[size - 1] = '\0';
			std::string result(buffer);
			delete[] buffer;
			return result;
		}
		delete[] buffer; // Free memory in case of failure
		return ""; // Return an empty string when reading fails
	}


	std::uintptr_t GetProcessId() {
		return this->processId;
	}
};
