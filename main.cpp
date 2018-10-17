#undef __STRICT_ANSI__
#include "main.h"
#include <math.h>

UINT_PTR base;
double height;
double YNoise = 0.01;


_declspec(naked) void DLL_EXPORT ASMMultiplier(){
    asm("call [_HeightMultiplierPtr]");
    asm("movsd [ebp + 0x10], xmm3");

    asm("mov eax, [_base]"); //jump  back
    asm("add eax, 0xC10ED");
    asm("jmp eax");
}

_declspec(naked) void DLL_EXPORT ASMYNoise1(){
    asm(" mulsd xmm1, [_YNoise]");
    asm("cvtdq2pd xmm2, xmm2");

    asm("mov eax, [_base]"); //jump  back
    asm("add eax, 0x1EE5B4");
    asm("jmp eax");
}

_declspec(naked) void DLL_EXPORT ASMYNoise2(){
    asm("mulsd xmm2, [_YNoise]");
    asm("movaps xmm0, xmm1");

    asm("mov eax, [_base]"); //jump  back
    asm("add eax, 0x1EE5C4");
    asm("jmp eax");
}

void DLL_EXPORT HeightMultiplier(){
    asm("movsd [_height], xmm3");

    height *= 1.4;

    asm("movsd xmm3, [_height]");
}
DWORD HeightMultiplierPtr = (DWORD)&HeightMultiplier;

void WriteJMP(BYTE* location, BYTE* newFunction){
	DWORD dwOldProtection;
	VirtualProtect(location, 5, PAGE_EXECUTE_READWRITE, &dwOldProtection);
    location[0] = 0xE9; //jmp
    *((DWORD*)(location + 1)) = (DWORD)(( (unsigned INT32)newFunction - (unsigned INT32)location ) - 5);
	VirtualProtect(location, 5, dwOldProtection, &dwOldProtection);
}


extern "C" DLL_EXPORT BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    base = (UINT_PTR)GetModuleHandle(NULL);
    switch (fdwReason)
    {

        case DLL_PROCESS_ATTACH:
            WriteJMP((BYTE*)(base + 0xC10E8), (BYTE*)&ASMMultiplier);

            //WriteJMP((BYTE*)(base + 0x1EE5A8), (BYTE*)&ASMYNoise1);

            //WriteJMP((BYTE*)(base + 0x1EE5B9), (BYTE*)&ASMYNoise2);

            break;
;
    }
    return TRUE;
}
