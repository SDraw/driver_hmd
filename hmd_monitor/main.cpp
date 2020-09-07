#include "stdafx.h"

#include "CMonitorCore.h"

#ifdef _DEBUG
int main(void)
{
#else
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    UNREFERENCED_PARAMETER(hInstance);
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nCmdShow);
#endif
    CMonitorCore *l_core = new CMonitorCore();
    if(l_core->Initialize())
    {
        while(l_core->DoPulse());
        l_core->Terminate();
    }
    delete l_core;

    return EXIT_SUCCESS;
}