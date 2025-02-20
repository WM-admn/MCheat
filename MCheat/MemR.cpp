#include "MemR.h"

MemR::MemR(char* procName, size_t bufferSize) : m_pID(0), m_bufferSize(bufferSize)
{
	int len = strlen(procName) + 1;
	m_pName = new char[len];
	strcpy_s(m_pName, len, procName);
	m_buffer = new byte[bufferSize];
}

MemR::~MemR()
{
	delete[] m_pName;
	delete[] m_buffer;
}

void MemR::GetPID()
{
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 pInfo;

	pInfo.dwSize = sizeof(PROCESSENTRY32);
	if(Process32First(snapshot,&pInfo))
	{
		while (Process32Next(snapshot, &pInfo))
		{
			if (strcmp(m_pName, pInfo.szExeFile) == 0)
			{
				m_pID = pInfo.th32ProcessID;
				CloseHandle(snapshot);
				return;
			}
		}
	}
	CloseHandle(snapshot);
	m_pID = 0;
}

void MemR::Open(DWORD accessRights)
{
	GetPID();
	m_hProc = OpenProcess(accessRights, false, m_pID);
}

void MemR::Close()
{
	CloseHandle(m_hProc);
}

DWORD MemR::getPID()
{
	return m_pID;
}
byte* MemR::getbuf() { return m_buffer; }

void MemR::Write(void* ptr, DWORD to, size_t size)
{
	WriteProcessMemory(m_hProc, (LPVOID)to, ptr, size, NULL);
}
void MemR::Write(void* ptr, DWORD to, size_t size, DWORD memProtect)
{
	DWORD oldMemProtect = 0;
	VirtualProtectEx(m_hProc, (LPVOID)to, size, memProtect, &oldMemProtect);
	WriteProcessMemory(m_hProc, (LPVOID)to, ptr, size, NULL);
	VirtualProtectEx(m_hProc, (LPVOID)to, size, oldMemProtect, &oldMemProtect);
}

MemR& MemR::Read(DWORD from, size_t size)
{
	//if (size > m_bufferSize) return;
	memset(m_buffer, 0, m_bufferSize);
	ReadProcessMemory(m_hProc, (LPVOID)from, m_buffer, size, NULL);
	return *this;
}
MemR& MemR::Read(DWORD from, size_t size, DWORD memProtect)
{
	//if (size > m_bufferSize) return;
	DWORD oldMemProtect = 0;
	memset(m_buffer, 0, m_bufferSize);
	VirtualProtectEx(m_hProc, (LPVOID)from, size, memProtect, &oldMemProtect);
	ReadProcessMemory(m_hProc, (LPVOID)from, m_buffer, size, NULL);
	VirtualProtectEx(m_hProc, (LPVOID)from, size, oldMemProtect, &oldMemProtect);
	return *this;
}
DWORD MemR::ReadPointer(DWORD base, DWORD* offsets, int count)
{
		for (int i = 0; i < count - 1; i++)
		{
			base = Read(base + offsets[i], 4).toUINT32();
		}
	return base + offsets[count - 1];

}

DWORD MemR::Alloc(DWORD memProtect)
{
	return (DWORD)VirtualAllocEx(m_hProc, NULL, 0x1000, MEM_COMMIT | MEM_RESERVE, memProtect);
}
void MemR::Free(DWORD ptr)
{
	VirtualFreeEx(m_hProc,(LPVOID)ptr,0,MEM_RELEASE);
}

DWORD MemR::FindSignature(DWORD base, DWORD size, byte* sign, char* mask)
{
	MEMORY_BASIC_INFORMATION mbi = { NULL };
	DWORD offset = NULL;
	while (offset < size)
	{
		VirtualQueryEx(m_hProc,(LPVOID)(base+offset),&mbi,sizeof(MEMORY_BASIC_INFORMATION));
		if(mbi.State!=MEM_FREE)
		{
			byte* buffer = new byte[mbi.RegionSize];
			ReadProcessMemory(m_hProc,mbi.BaseAddress,buffer,mbi.RegionSize,NULL);
			for (int i = 0; i < mbi.RegionSize; i++)
			{
				if (DataCompare(buffer + i, sign, mask))
				{
					delete[] buffer;
					return (DWORD)mbi.BaseAddress + i;
				}
			}
			delete[] buffer;
		}
		offset += mbi.RegionSize;
	}
	return 0;
}
bool MemR::DataCompare(byte* data, byte* sign, char* mask)
{
	for (; *mask; mask++, sign++, data++)
	{
		if (*mask == 'x' && *data != *sign)
			return false;
	}
	return true;
}




void MemR::GetModuleInfo(char* moduleName, MODULEENTRY32* mInfo, bool aboutProcess )
{
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, m_pID);
	mInfo->dwSize = sizeof(MODULEENTRY32);
	if (Module32First(snapshot, mInfo))
	{
		if (aboutProcess) { CloseHandle(snapshot); return; }
		while (Module32Next(snapshot, mInfo))
		{
			if (strcmp(moduleName, mInfo->szModule) == 0)
			{
				CloseHandle(snapshot);
				return;
			}
		}
	}
	CloseHandle(snapshot);
	memset(mInfo, 0, sizeof(MODULEENTRY32));
}

double MemR::toDouble()
{
	double v = 0;
	memcpy(&v, m_buffer, sizeof(double));
	return v;
}
float MemR::toFloat()
{
	float v = 0;
	memcpy(&v, m_buffer, sizeof(float));
	return v;
}
UINT64 MemR::toUINT64()
{
	UINT64 v = 0;
	memcpy(&v, m_buffer, sizeof(UINT64));
	return v;
}
INT64 MemR::toINT64()
{
	INT64 v = 0;
	memcpy(&v, m_buffer, sizeof(INT64));
	return v;
}
UINT32 MemR::toUINT32()
{
	UINT32 v = 0;
	memcpy(&v, m_buffer, sizeof(UINT32));
	return v;
}
int MemR::toINT32()
{
	int v = 0;
	memcpy(&v, m_buffer, sizeof(int));
	return v;
}
UINT16 MemR::toUINT16()
{
	UINT16 v = 0;
	memcpy(&v, m_buffer, sizeof(UINT16));
	return v;
}
short int MemR::toINT16()
{
	short int v = 0;
	memcpy(&v, m_buffer, sizeof(short int));
	return v;
}
UINT8 MemR::toUINT8()
{
	UINT8 v = 0;
	memcpy(&v, m_buffer, sizeof(UINT8));
	return v;
}
char MemR::toINT8()
{
	char v = 0;
	memcpy(&v, m_buffer, sizeof(char));
	return v;
}
char* MemR::toStringA()
{
	return (char*)m_buffer;
}
wchar_t* MemR::toStringW()
{
	return (wchar_t*)m_buffer;
}