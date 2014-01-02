#define WM_Hook WM_USER+100

extern "C" __declspec(dllexport) BOOL WINAPI Start();
extern "C" __declspec(dllexport) void WINAPI Stop();