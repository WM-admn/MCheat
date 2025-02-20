#pragma once
#include <Windows.h>
#include <TlHelp32.h>

class MemR
{
public:
	MemR(char* procName, size_t bufferSize = 32);
	~MemR(void);
	void Open(DWORD accessRights = PROCESS_ALL_ACCESS);
	void Close();

	void Write(void* ptr, DWORD to, size_t size);
	void Write(void* ptr, DWORD to, size_t size,DWORD memProtect = PAGE_EXECUTE_READWRITE);

	MemR& Read(DWORD from, size_t size);
	MemR& Read(DWORD from, size_t size,DWORD memProtect /*= PAGE_READWRITE*/);

	DWORD ReadPointer(DWORD base, DWORD* offsets, int count);

	DWORD Alloc(DWORD memProtect = PAGE_EXECUTE_READWRITE);
	void Free(DWORD ptr);

	DWORD FindSignature(DWORD base, DWORD size, byte* sign, char* mask);







	void GetModuleInfo(char* moduleName, MODULEENTRY32* mInfo, bool aboutProcess = false);








	DWORD getPID();
	byte* getbuf();

	double    toDouble();
	float      toFloat();

	UINT64    toUINT64();
	INT64      toINT64();

	UINT32    toUINT32();
	int        toINT32();

	UINT16    toUINT16();
	short int  toINT16();

	UINT8      toUINT8();
	char        toINT8();

	char*    toStringA();
	wchar_t* toStringW();

private:
	void GetPID();
	bool DataCompare(byte* data, byte* sign, char* mask);
	char* m_pName;
	HANDLE m_hProc;
	DWORD m_pID;
	byte* m_buffer;
	size_t m_bufferSize;
};

