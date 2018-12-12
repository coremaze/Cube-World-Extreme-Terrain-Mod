#include <windows.h>
#include <math.h>

unsigned int base;

double VanillaNoise(double x, double y)
{
  double v3;
  double v4;
  double v5;
  int v6;
  signed int v7;
  int v8;
  int v9;
  int v10;
  int v11;
  double v12;
  double v13;
  int v15;

  v3 = x;
  v4 = (double)(signed int)y;
  v5 = (double)(signed int)x;
  v6 = 57 * (signed int)v4;
  v7 = (signed int)(v5 + 1.0);
  v8 = (((signed int)v5 + v6) << 13) ^ ((signed int)v5 + v6);
  v15 = ((v7 + v6) << 13) ^ (v7 + v6);
  v9 = 57 * (signed int)(v4 + 1.0);
  v10 = ((v9 + (signed int)v5) << 13) ^ (v9 + (signed int)v5);
  v11 = ((v7 + v9) << 13) ^ (v7 + v9);
  v12 = (1.0 - cos((v3 - v5) * 3.1415927)) * 0.5;
  v13 = cos((y - v4) * 3.1415927);
  return (float)(((1.0 - (double)((v8 * (60493 * v8 * v8 + 19990303) - 771171059) & 0x7FFFFFFF) * 9.313225746154785e-10)
                * (1.0 - v12)
                + (1.0
                 - (double)((v15 * (60493 * v15 * v15 + 19990303) - 771171059) & 0x7FFFFFFF) * 9.313225746154785e-10)
                * v12)
               * (1.0 - (1.0 - v13) * 0.5)
               + ((1.0
                 - (double)((v10 * (60493 * v10 * v10 + 19990303) - 771171059) & 0x7FFFFFFF) * 9.313225746154785e-10)
                * (1.0 - v12)
                + (1.0
                 - (double)((v11 * (60493 * v11 * v11 + 19990303) - 771171059) & 0x7FFFFFFF) * 9.313225746154785e-10)
                * v12)
               * ((1.0 - v13)
                * 0.5));
}


double __cdecl GenerateNoise(double x, double y){
    double vn = VanillaNoise(x, y);
    //apply a sigmoid function
    return 2.0 * (1.0/(1.0 + pow(2, -6.0 * vn))) - 1.0;
}
void WriteJMP(BYTE* location, BYTE* newFunction){
	DWORD dwOldProtection;
	VirtualProtect(location, 5, PAGE_EXECUTE_READWRITE, &dwOldProtection);
    location[0] = 0xE9; //jmp
    *((DWORD*)(location + 1)) = (DWORD)(( (unsigned INT32)newFunction - (unsigned INT32)location ) - 5);
	VirtualProtect(location, 5, dwOldProtection, &dwOldProtection);
}

extern "C" __declspec(dllexport) bool APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            base = (unsigned int)GetModuleHandle(NULL);

            //overwrite beginning of vanilla noise function
            WriteJMP((BYTE*)(base + 0x0C0EF0), (BYTE*)&GenerateNoise);

            break;

    }
    return true;
}
