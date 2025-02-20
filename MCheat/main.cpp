#define BEA_ENGINE_STATIC
#define BEA_USE_STDCALL

#include "MemR.h"
#include <stdio.h>
#pragma comment (lib,"BeaEngine_s_d_l_stdcall.lib")
#include <BeaEngine.h>
#if _DEBUG
#pragma comment (lib,"AsmJit_debug.lib")
#else
#pragma comment (lib,"AsmJit.lib")
#endif*/
#include <asmjit.h>


using namespace asmjit;

// Signature of the generated function.
typedef int(*Func)(void);

int main(int argc, char* argv[]) {
	MemR* mem = new MemR((char*)"Project3(test).exe");
	//JitRuntime rt;                          // Runtime specialized for JIT code execution.

	CodeHolder code;                        // Holds code and relocation information.
	code.init(CodeInfo(ArchInfo::kTypeHost));
	//code.init(rt.getCodeInfo());            // Initialize to the same arch as JIT runtime.

	X86Assembler a(&code);                  // Create and attach X86Assembler to `code`.
	Label Loop = a.newLabel();              // To construct the loop, we need some labels.
	Label Exit = a.newLabel();
	
	a.cmp(x86::byte_ptr(x86::eax, 0x20), 0xAB);
	a.jne(Loop);
	a.mov(x86::dword_ptr(x86::eax, 0x40), 1000);
	a.jnp(Exit);
	
	a.bind(Loop);
	a.mov(x86::dword_ptr(x86::eax, 0x40), 0);
	a.bind(Exit);
	a.ret();
	//a.mov(x86::eax, 1);                     // Move one to 'eax' register.
                                // Return from function.
	// ----> X86Assembler is no longer needed from here and can be destroyed <----

	//Func fn;
	//Error err = rt.add(&fn, &code);         // Add the generated code to the runtime.
	//if (err) return 1;                      // Handle a possible error returned by AsmJit.
	// ----> CodeHolder is no longer needed from here and can be destroyed <----

	//int result = fn();                      // Execute the generated code.
//	printf("%d\n", result);                 // Print the resulting "1".
	printf("%x\n", a.getBufferData());
	printf("%x\n", a.getBufferEnd());
	printf("%x\n", a.getCode());
	printf("%x\n", a.getBufferPtr());
	printf("%x\n", a.getBufferCapacity());



	
	mem->Open();
	printf("PID: %x\n", mem->getPID());
	//DWORD addres = mem->Alloc();
	//printf("PID: %x\n", addres);
	DWORD ii = DWORD(a.getBufferData());
	printf("%x\n", ii);
	mem->Read(DWORD(a.getBufferData()), 15);
	printf("%x\n", mem->getbuf());


	/* ============================= Init datas */
	//DISASM MyDisasm;
	//(void)memset(&MyDisasm, 0, sizeof(DISASM));
	
	//int len;
	//len = Disasm(&MyDisasm);
	//printf("%d\n", len);


	/* ============================= Init datas */
	DISASM MyDisasm;
	int len, i = 0;
	bool Error = false;

	/* ============================= Init the Disasm structure (important !)*/
	(void)memset(&MyDisasm, 0, sizeof(DISASM));

	/* ============================= Init EIP */
	MyDisasm.EIP = UIntPtr(mem->getbuf());

	/* ============================= Loop for Disasm */
	while ((!Error) && (i < 14)) {
		
		len = Disasm(&MyDisasm);
		printf("%d\n", len);
		if (len != UNKNOWN_OPCODE) {
			(void)puts(MyDisasm.CompleteInstr);
			MyDisasm.EIP = MyDisasm.EIP + len;
			i++;
		}
		else {
			Error = true;
		}
	}
	//printf("%d\n", len);


	//int false = 0, true = 1;

	/*int len;
	bool Error = false;
	int EndCodeSection = 0x00498CE4;

	 ============================= Init the Disasm structure (important !)
	(void)memset(&MyDisasm, 0, sizeof(DISASM));

	 ============================= Init EIP 
	MyDisasm.EIP = 0x00498CAA;

	============================= Loop for Disasm 
	while (!Error) {
		 ============================= Fix SecurityBlock 
		MyDisasm.SecurityBlock = EndCodeSection - MyDisasm.EIP;

		len = Disasm(&MyDisasm);
		if (len == OUT_OF_BLOCK) {
			(void)printf("disasm engine is not allowed to read more memory \n");
			Error = true;
		}
		else if (len == UNKNOWN_OPCODE) {
			(void)printf("unknown opcode");
			Error = true;
		}
		else {
			(void)puts(MyDisasm.CompleteInstr);
			MyDisasm.EIP = MyDisasm.EIP + len;
			if (MyDisasm.EIP >= EndCodeSection) {
				(void)printf("End of buffer reached ! \n");
				Error = true;
			}
		}
	}*/

	// All classes use RAII, all resources will be released before `main()` returns,
	// the generated function can be, however, released explicitly if you intend to
	// reuse or keep the runtime alive, which you should in a production-ready code.
	//rt.release(fn);
	system("PAUSE");
	return 0;
	
}
/*#include "MemR.h"
#include "asmjit/asmjit.h"
#include <stdio.h>
using namespace asmjit;

// Signature of the generated function.
typedef int(*Func)(void);

int main(int argc, char* argv[]) {


	MemR* mem = new MemR((char*)"Project3(test).exe");
	mem->Open();
	printf("PID: %x\n", mem->getPID());
	DWORD addres = mem->Alloc();
	printf("PID: %x\n", addres);

	JitRuntime rt;                          // Runtime specialized for JIT code execution.
	CodeInfo ci(ArchInfo::kTypeHost);
	ci.setBaseAddress(addres);
	CodeHolder code;                        // Holds code and relocation information.
	code.init(ci);            // Initialize to the same arch as JIT runtime.

	//code.init(rt.getCodeInfo());

	X86Assembler a(&code);                  // Create and attach X86Assembler to `code`.
	X86Gp dst;
	X86Gp src_a;
	X86Gp src_b;
	dst = x86::eax;
	src_a = x86::ecx;
	src_b = x86::edx;
	a.mov(dst, x86::dword_ptr(x86::esp, 4));  // Load the destination pointer.
	a.mov(src_a, x86::dword_ptr(x86::esp, 8));  // Load the first source pointer.
	a.mov(src_b, x86::dword_ptr(x86::esp, 12)); // Load the second source pointer.


	  a.movdqu(x86::xmm0, x86::ptr(src_a));   // Load 4 ints from [src_a] to XMM0.
	  a.movdqu(x86::xmm1, x86::ptr(src_b));   // Load 4 ints from [src_b] to XMM1.
	  a.paddd(x86::xmm0, x86::xmm1);          // Add 4 ints in XMM1 to XMM0.
	  a.movdqu(x86::ptr(dst), x86::xmm0);     // Store the result to [dst].
	  a.ret();


	//a.mov(x86::eax, 1);                     // Move one to 'eax' register.
	//a.ret();                                // Return from function.
	// ----> X86Assembler is no longer needed from here and can be destroyed <----
	//Func fn;
	//Error err = rt.add(&fn, &code);         // Add the generated code to the runtime.
	//if (err) return 1;                      // Handle a possible error returned by AsmJit.
	// ----> CodeHolder is no longer needed from here and can be destroyed <----

	//  Func fn;
	//  Error err = rt.add(&fn, &code);

	//int result = fn();                      // Execute the generated code.
	//printf("%d\n", result);					// Print the resulting "1".
	printf("%d\n", &code);
	//size_t size = code.getCodeSize();
	//VMemMgr vm;
	//void* p = vm.alloc(size);               // Allocate a virtual memory (executable).
	//if (!p) return 0;                       // Handle a possible out-of-memory case.



	size_t size = code.getCodeSize();
	VMemMgr vm;
	printf("%d\n", code.getCodeSize());
	void* p = vm.alloc(size);


	//size_t realSize = code.relocate(p);

	code.relocate((LPVOID)ci.getBaseAddress());

	printf("%x\n", code.getBaseAddress());
	printf("%x\n", p);

	//code.relocate((LPVOID)ci.getBaseAddress());

	//size_t realSize = code.relocate(p);     // Relocate & store the output in 'p'.

	//printf("%d\n", p);

	// All classes use RAII, all resources will be released before `main()` returns,
	// the generated function can be, however, released explicitly if you intend to
	// reuse or keep the runtime alive, which you should in a production-ready code.
	//rt.release(fn);
	//vm.release(p);

	system("PAUSE");
	return 0;
}

/*#include <stdio.h>
#include <iostream>
#include "MemR.h"

void main()
{
	char A[] = "Steam.exe";
	MemR* mem = new MemR((char*)"winamp.exe");
	mem->Open();
	printf("PID: %X\n", mem->getPID());
	//printf("Value: %lf\n", mem->Read(0x75C2B19C,8).toDouble());
	int a = 999999999;
	//mem->Write(&a,0xBC9776C, 4);

	//DWORD adress = mem->Alloc();
	//mem->Free(0x25A0000);
	//printf("memory ox%X\n", adress);

	MODULEENTRY32 g = {NULL};
	mem->GetModuleInfo(/*(char*)"GDI32.dll"NULL, &g, true);
	printf("module info addr of base 0x%X\n size 0x%X\n", (DWORD)g.modBaseAddr, g.modBaseSize);


	DWORD someadress = mem->FindSignature((DWORD)g.modBaseAddr, (DWORD)g.modBaseAddr, (PBYTE) "\xA1\x22\x42\x4B\x00\x31\x45\xFC", (char*)"x1111xxx");

	printf("signature tut 0x%X\n", someadress);
	std::cout<<"sdadad\n \x8B\xE5\x5D\x51\xC3";

	system("PAUSE");
}*/
