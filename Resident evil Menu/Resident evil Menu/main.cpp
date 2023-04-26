// Dear ImGui: standalone example application for DirectX 9
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include <d3d9.h>
#include <tchar.h>
#include <windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <vector>
#include "stdafx.h"
#include "proc.h"
#include <dwmapi.h>

// Data
static LPDIRECT3D9              g_pD3D = NULL;
static LPDIRECT3DDEVICE9        g_pd3dDevice = NULL;
static D3DPRESENT_PARAMETERS    g_d3dpp = {};

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void ResetDevice();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Main code
int main(int, char**)
{
    HWND hWnd = GetConsoleWindow();
    ShowWindow(hWnd, SW_HIDE);

    // Create application window
    //ImGui_ImplWin32_EnableDpiAwareness();
    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, L"ImGui Example", NULL, };
    ::RegisterClassExW(&wc);
    HWND hwnd = ::CreateWindow(wc.lpszClassName, L"Dear ImGui DirectX9 Example", WS_EX_TOPMOST | WS_POPUP | WS_EX_LAYERED | WS_EX_COMPOSITED, 0, 0, 800, 400, NULL, NULL, wc.hInstance, NULL, );

    // Initialize Direct3D
    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    // Show the window
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX9_Init(g_pd3dDevice);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    ImGuiStyle& style = ImGui::GetStyle();
    style.Colors[ImGuiCol_WindowBg] = ImVec4(255, 0, 0, 255);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0, 0, 0, 130);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0, 0, 0, 130);
    style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0, 0, 0, 130);

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    bool godmode = false;
    bool godmode1 = false;
    float x = 0x00E340B0 + 0xEC + 0x34;
    float y = 0x00E340B0 + 0xEC + 0x30;
    float z = 0x00E340B0 + 0xEC + 0x38;
    int spot1 = 0x00E340B0 + 0xEC + 0x21AC;
    int spot2 = 0x00E340B0 + 0xEC + 0x21DC;
    int spot3 = 0x00E340B0 + 0xEC + 0x220C;
    int spot4 = 0x00E340B0 + 0xEC + 0x223C;
    int spot5 = 0x00E340B0 + 0xEC + 0x226C;
    int spot6 = 0x00E340B0 + 0xEC + 0x229C;
    int spot7 = 0x00E340B0 + 0xEC + 0x22CC;
    int spot8 = 0x00E340B0 + 0xEC + 0x22FC;
    int spot9 = 0x00E340B0 + 0xEC + 0x232C;
    float sx = 0x00E340B0 + 0xF0 + 0x34;
    float sy = 0x00E340B0 + 0xF0 + 0x30;
    float sz = 0x00E340B0 + 0xF0 + 0x38;

    // Main loop
    bool done = false;
    while (!done)
    {
        // Poll and handle messages (inputs, window resize, etc.)
        // See the WndProc() function below for our to dispatch events to the Win32 backend.
        MSG msg;
        while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        // Start the Dear ImGui frame
        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(400, 400));
        if (ImGui::Begin("Resident Evil 5", NULL, ImGuiWindowFlags_NoResize))
        {
            if (ImGui::BeginTabBar("tabs"))
            {


                ImGui::EndTabBar();
            }

            
            ImGui::Checkbox("God Mode (Chris)", &godmode);
            ImGui::Checkbox("God Mode (Sheva)", &godmode1);
            ImGui::SeparatorText("Chris Current Location");
            ImGui::InputFloat("x", &x);
            ImGui::InputFloat("y", &y);
            ImGui::InputFloat("z", &z);
            ImGui::SeparatorText("Chris Inventory Slots");
            ImGui::InputInt("1st Slot", &spot1);
            ImGui::InputInt("2nd Slot", &spot2);
            ImGui::InputInt("3rd Slot", &spot3);
            ImGui::InputInt("4th Slot", &spot4);
            ImGui::InputInt("5th Slot", &spot5);
            ImGui::InputInt("6th Slot", &spot6);
            ImGui::InputInt("7th Slot", &spot7);
            ImGui::InputInt("8th Slot", &spot8);
            ImGui::InputInt("9th Slot", &spot9);
            ImGui::SeparatorText("Sheva Current Location");
            ImGui::InputFloat("x", &sx);
            ImGui::InputFloat("y", &sy);
            ImGui::InputFloat("z", &sz);

        }ImGui::End();

        if (godmode == true);
        {
            if (godmode == true)
            {
                //Get ProcId of the target process
                DWORD procId = GetProcId(L"Re5dx9.exe");

                //Getmodulebaseaddress
                uintptr_t moduleBase = GetModuleBaseAddress(procId, L"Re5dx9.exe");

                //Get Handle to Process
                HANDLE hProcess = 0;
                hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

                //Resolve base address of the pointer chain
                uintptr_t dynamicPtrBaseAddr = moduleBase + 0x00E340B0;

                std::cout << "DynamicPtrBaseAddr = " << "0x" << std::hex << dynamicPtrBaseAddr << std::endl;

                //Resolve our ammo pointer chain
                std::vector<unsigned int> HealthOffsets = { 0xEC, 0x1364, };
                uintptr_t HealthAddr = FindDMAAddy(hProcess, dynamicPtrBaseAddr, HealthOffsets);

                std::vector<unsigned int> MaxHealthOffsets = { 0xEC, 0x1366, };
                uintptr_t MaxHealthAddr = FindDMAAddy(hProcess, dynamicPtrBaseAddr, MaxHealthOffsets);

                std::cout << "HealthAddr = " << "0x" << std::hex << HealthAddr << std::endl;

                std::cout << "MaxHealthAddr = " << "0x" << std::hex << MaxHealthAddr << std::endl;

                //Read Ammo value
                short HealthValue = 30000;
                short HealthMaxValue = 30000;

                ReadProcessMemory(hProcess, (BYTE*)HealthAddr, &HealthValue, sizeof(HealthValue), nullptr);
                std::cout << "Current Health = " << std::dec << HealthValue << std::endl;

                ReadProcessMemory(hProcess, (BYTE*)MaxHealthAddr, &HealthMaxValue, sizeof(HealthMaxValue), nullptr);
                std::cout << "Current Max Health = " << std::dec << HealthMaxValue << std::endl;

                //Write to it
                short newHealth = 30000;
                short newMaxHealth = 30000;


                WriteProcessMemory(hProcess, (BYTE*)HealthAddr, &newHealth, sizeof(newHealth), nullptr);

                WriteProcessMemory(hProcess, (BYTE*)MaxHealthAddr, &newMaxHealth, sizeof(newMaxHealth), nullptr);

                //Read out again
                ReadProcessMemory(hProcess, (BYTE*)HealthAddr, &HealthValue, sizeof(HealthValue), nullptr);

                ReadProcessMemory(hProcess, (BYTE*)MaxHealthAddr, &HealthMaxValue, sizeof(HealthMaxValue), nullptr);

            }

            else (godmode == false);
            {

            }

        }



        if (godmode1 == true);
        {
            if (godmode1 == true)
            {
                //Get ProcId of the target process
                DWORD procId = GetProcId(L"Re5dx9.exe");

                //Getmodulebaseaddress
                uintptr_t moduleBase = GetModuleBaseAddress(procId, L"Re5dx9.exe");

                //Get Handle to Process
                HANDLE hProcess = 0;
                hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

                //Resolve base address of the pointer chain
                uintptr_t dynamicPtrBaseAddr = moduleBase + 0x00E340B0;

                std::cout << "DynamicPtrBaseAddr = " << "0x" << std::hex << dynamicPtrBaseAddr << std::endl;

                //Resolve our ammo pointer chain
                std::vector<unsigned int> HealthOffsets = { 0xF0, 0x1364, };
                uintptr_t HealthAddr = FindDMAAddy(hProcess, dynamicPtrBaseAddr, HealthOffsets);

                std::vector<unsigned int> MaxHealthOffsets = { 0xF0, 0x1366, };
                uintptr_t MaxHealthAddr = FindDMAAddy(hProcess, dynamicPtrBaseAddr, MaxHealthOffsets);

                std::cout << "HealthAddr = " << "0x" << std::hex << HealthAddr << std::endl;

                std::cout << "MaxHealthAddr = " << "0x" << std::hex << MaxHealthAddr << std::endl;

                //Read Ammo value
                short HealthValue = 30000;
                short HealthMaxValue = 30000;

                ReadProcessMemory(hProcess, (BYTE*)HealthAddr, &HealthValue, sizeof(HealthValue), nullptr);
                std::cout << "Current Health = " << std::dec << HealthValue << std::endl;

                ReadProcessMemory(hProcess, (BYTE*)MaxHealthAddr, &HealthMaxValue, sizeof(HealthMaxValue), nullptr);
                std::cout << "Current Max Health = " << std::dec << HealthMaxValue << std::endl;

                //Write to it
                short newHealth = 30000;
                short newMaxHealth = 30000;


                WriteProcessMemory(hProcess, (BYTE*)HealthAddr, &newHealth, sizeof(newHealth), nullptr);

                WriteProcessMemory(hProcess, (BYTE*)MaxHealthAddr, &newMaxHealth, sizeof(newMaxHealth), nullptr);

                //Read out again
                ReadProcessMemory(hProcess, (BYTE*)HealthAddr, &HealthValue, sizeof(HealthValue), nullptr);

                ReadProcessMemory(hProcess, (BYTE*)MaxHealthAddr, &HealthMaxValue, sizeof(HealthMaxValue), nullptr);

            }

            else (godmode1 == false);
            {

            }

        }

        if (ImGui::InputFloat)
        {
            //Get ProcId of the target process
            DWORD procId = GetProcId(L"Re5dx9.exe");

            //Getmodulebaseaddress
            uintptr_t moduleBase = GetModuleBaseAddress(procId, L"Re5dx9.exe");

            //Get Handle to Process
            HANDLE hProcess = 0;
            hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

            //Resolve base address of the pointer chain
            uintptr_t dynamicPtrBaseAddr = moduleBase + 0x00E340B0;

            std::cout << "DynamicPtrBaseAddr = " << "0x" << std::hex << dynamicPtrBaseAddr << std::endl;

            //Resolve our ammo pointer chain
            std::vector<unsigned int> xOffsets = { 0xEC, 0x34, };
            uintptr_t xAddr = FindDMAAddy(hProcess, dynamicPtrBaseAddr, xOffsets);

            std::cout << "xAddr = " << "0x" << std::hex << xAddr << std::endl;

            //Read Ammo value
            float xValue = xValue; //lets it builds
            x = xValue;            // need to read to output box in imgui

            ReadProcessMemory(hProcess, (BYTE*)xAddr, &xValue, sizeof(xValue), nullptr);
            std::cout << "Current x = " << std::dec << xValue << std::endl;

            //Write to it
            float newx = 0; // define new value
            if (ImGui::InputFloat("x", &newx))

            WriteProcessMemory(hProcess, (BYTE*)xAddr, &newx, sizeof(newx), nullptr);

            //Read out again
            ReadProcessMemory(hProcess, (BYTE*)xAddr, &xValue, sizeof(xValue), nullptr);

            }

        if (ImGui::InputFloat)
        {
            //Get ProcId of the target process
            DWORD procId = GetProcId(L"Re5dx9.exe");

            //Getmodulebaseaddress
            uintptr_t moduleBase = GetModuleBaseAddress(procId, L"Re5dx9.exe");

            //Get Handle to Process
            HANDLE hProcess = 0;
            hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

            //Resolve base address of the pointer chain
            uintptr_t dynamicPtrBaseAddr = moduleBase + 0x00E340B0;

            std::cout << "DynamicPtrBaseAddr = " << "0x" << std::hex << dynamicPtrBaseAddr << std::endl;

            //Resolve our ammo pointer chain
            std::vector<unsigned int> yOffsets = { 0xEC, 0x30, };
            uintptr_t yAddr = FindDMAAddy(hProcess, dynamicPtrBaseAddr, yOffsets);

            std::cout << "yAddr = " << "0x" << std::hex << yAddr << std::endl;

            //Read Ammo value
            float yValue = yValue;
            y = yValue;

            ReadProcessMemory(hProcess, (BYTE*)yAddr, &yValue, sizeof(yValue), nullptr);
            std::cout << "Current y = " << std::dec << yValue << std::endl;

            //Write to it
            float newy = 0;
            if (ImGui::InputFloat("y", &newy))

                WriteProcessMemory(hProcess, (BYTE*)yAddr, &newy, sizeof(newy), nullptr);

            //Read out again
            ReadProcessMemory(hProcess, (BYTE*)yAddr, &yValue, sizeof(yValue), nullptr);

        }

        if (ImGui::InputFloat)
        {
            //Get ProcId of the target process
            DWORD procId = GetProcId(L"Re5dx9.exe");

            //Getmodulebaseaddress
            uintptr_t moduleBase = GetModuleBaseAddress(procId, L"Re5dx9.exe");

            //Get Handle to Process
            HANDLE hProcess = 0;
            hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

            //Resolve base address of the pointer chain
            uintptr_t dynamicPtrBaseAddr = moduleBase + 0x00E340B0;

            std::cout << "DynamicPtrBaseAddr = " << "0x" << std::hex << dynamicPtrBaseAddr << std::endl;

            //Resolve our ammo pointer chain
            std::vector<unsigned int> zOffsets = { 0xEC, 0x38, };
            uintptr_t zAddr = FindDMAAddy(hProcess, dynamicPtrBaseAddr, zOffsets);

            std::cout << "zAddr = " << "0x" << std::hex << zAddr << std::endl;

            //Read Ammo value
            float zValue = zValue;
            z = zValue;

            ReadProcessMemory(hProcess, (BYTE*)zAddr, &zValue, sizeof(zValue), nullptr);
            std::cout << "Current z = " << std::dec << zValue << std::endl;

            //Write to it
            float newz = 0;
            if (ImGui::InputFloat("z", &newz))

                WriteProcessMemory(hProcess, (BYTE*)zAddr, &newz, sizeof(newz), nullptr);

            //Read out again
            ReadProcessMemory(hProcess, (BYTE*)zAddr, &zValue, sizeof(zValue), nullptr);

        }
      
        if (ImGui::InputInt)
        {
            //Get ProcId of the target process
            DWORD procId = GetProcId(L"Re5dx9.exe");

            //Getmodulebaseaddress
            uintptr_t moduleBase = GetModuleBaseAddress(procId, L"Re5dx9.exe");

            //Get Handle to Process
            HANDLE hProcess = 0;
            hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

            //Resolve base address of the pointer chain
            uintptr_t dynamicPtrBaseAddr = moduleBase + 0x00E340B0;

            std::cout << "DynamicPtrBaseAddr = " << "0x" << std::hex << dynamicPtrBaseAddr << std::endl;

            //Resolve our ammo pointer chain
            std::vector<unsigned int> S1Offsets = { 0xEC, 0x21AC, };
            uintptr_t S1Addr = FindDMAAddy(hProcess, dynamicPtrBaseAddr, S1Offsets);

            std::cout << "S1Addr = " << "0x" << std::hex << S1Addr << std::endl;

            //Read Ammo value
            int S1Value = S1Value; //lets it builds
            spot1 = S1Value;            // need to read to output box in imgui

            ReadProcessMemory(hProcess, (BYTE*)S1Addr, &S1Value, sizeof(S1Value), nullptr);
            std::cout << "Current 1st slot = " << std::dec << S1Value << std::endl;

            //Write to it
            int newS1 = 0; // define new value
            ImGui::SeparatorText("Chris Inventory modification");
            if (ImGui::InputInt("1st Slot", &newS1))

                WriteProcessMemory(hProcess, (BYTE*)S1Addr, &newS1, sizeof(newS1), nullptr);

            //Read out again
            ReadProcessMemory(hProcess, (BYTE*)S1Addr, &S1Value, sizeof(S1Value), nullptr);

        }

        if (ImGui::InputInt)
        {
            //Get ProcId of the target process
            DWORD procId = GetProcId(L"Re5dx9.exe");

            //Getmodulebaseaddress
            uintptr_t moduleBase = GetModuleBaseAddress(procId, L"Re5dx9.exe");

            //Get Handle to Process
            HANDLE hProcess = 0;
            hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

            //Resolve base address of the pointer chain
            uintptr_t dynamicPtrBaseAddr = moduleBase + 0x00E340B0;

            std::cout << "DynamicPtrBaseAddr = " << "0x" << std::hex << dynamicPtrBaseAddr << std::endl;

            //Resolve our ammo pointer chain
            std::vector<unsigned int> S2Offsets = { 0xEC, 0x21DC, };
            uintptr_t S2Addr = FindDMAAddy(hProcess, dynamicPtrBaseAddr, S2Offsets);

            std::cout << "S2Addr = " << "0x" << std::hex << S2Addr << std::endl;

            //Read Ammo value
            int S2Value = S2Value; //lets it builds
            spot2 = S2Value;            // need to read to output box in imgui

            ReadProcessMemory(hProcess, (BYTE*)S2Addr, &S2Value, sizeof(S2Value), nullptr);
            std::cout << "Current 2nd slot = " << std::dec << S2Value << std::endl;

            //Write to it
            int newS2 = 0; // define new value
            if (ImGui::InputInt("2nd Slot", &newS2))

                WriteProcessMemory(hProcess, (BYTE*)S2Addr, &newS2, sizeof(newS2), nullptr);

            //Read out again
            ReadProcessMemory(hProcess, (BYTE*)S2Addr, &S2Value, sizeof(S2Value), nullptr);

        }

        if (ImGui::InputInt)
        {
            //Get ProcId of the target process
            DWORD procId = GetProcId(L"Re5dx9.exe");

            //Getmodulebaseaddress
            uintptr_t moduleBase = GetModuleBaseAddress(procId, L"Re5dx9.exe");

            //Get Handle to Process
            HANDLE hProcess = 0;
            hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

            //Resolve base address of the pointer chain
            uintptr_t dynamicPtrBaseAddr = moduleBase + 0x00E340B0;

            std::cout << "DynamicPtrBaseAddr = " << "0x" << std::hex << dynamicPtrBaseAddr << std::endl;

            //Resolve our ammo pointer chain
            std::vector<unsigned int> S3Offsets = { 0xEC, 0x220C, };
            uintptr_t S3Addr = FindDMAAddy(hProcess, dynamicPtrBaseAddr, S3Offsets);

            std::cout << "S3Addr = " << "0x" << std::hex << S3Addr << std::endl;

            //Read Ammo value
            int S3Value = S3Value; //lets it builds
            spot3 = S3Value;            // need to read to output box in imgui

            ReadProcessMemory(hProcess, (BYTE*)S3Addr, &S3Value, sizeof(S3Value), nullptr);
            std::cout << "Current 3rd slot = " << std::dec << S3Value << std::endl;

            //Write to it
            int newS3 = 0; // define new value
            if (ImGui::InputInt("3rd Slot", &newS3))

                WriteProcessMemory(hProcess, (BYTE*)S3Addr, &newS3, sizeof(newS3), nullptr);

            //Read out again
            ReadProcessMemory(hProcess, (BYTE*)S3Addr, &S3Value, sizeof(S3Value), nullptr);

        }

        if (ImGui::InputInt)
        {
            //Get ProcId of the target process
            DWORD procId = GetProcId(L"Re5dx9.exe");

            //Getmodulebaseaddress
            uintptr_t moduleBase = GetModuleBaseAddress(procId, L"Re5dx9.exe");

            //Get Handle to Process
            HANDLE hProcess = 0;
            hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

            //Resolve base address of the pointer chain
            uintptr_t dynamicPtrBaseAddr = moduleBase + 0x00E340B0;

            std::cout << "DynamicPtrBaseAddr = " << "0x" << std::hex << dynamicPtrBaseAddr << std::endl;

            //Resolve our ammo pointer chain
            std::vector<unsigned int> S4Offsets = { 0xEC, 0x223C, };
            uintptr_t S4Addr = FindDMAAddy(hProcess, dynamicPtrBaseAddr, S4Offsets);

            std::cout << "S4Addr = " << "0x" << std::hex << S4Addr << std::endl;

            //Read Ammo value
            int S4Value = S4Value; //lets it builds
            spot4 = S4Value;            // need to read to output box in imgui

            ReadProcessMemory(hProcess, (BYTE*)S4Addr, &S4Value, sizeof(S4Value), nullptr);
            std::cout << "Current 4th slot = " << std::dec << S4Value << std::endl;

            //Write to it
            int newS4 = 0; // define new value
            if (ImGui::InputInt("4th Slot", &newS4))

                WriteProcessMemory(hProcess, (BYTE*)S4Addr, &newS4, sizeof(newS4), nullptr);

            //Read out again
            ReadProcessMemory(hProcess, (BYTE*)S4Addr, &S4Value, sizeof(S4Value), nullptr);

        }

        if (ImGui::InputInt)
        {
            //Get ProcId of the target process
            DWORD procId = GetProcId(L"Re5dx9.exe");

            //Getmodulebaseaddress
            uintptr_t moduleBase = GetModuleBaseAddress(procId, L"Re5dx9.exe");

            //Get Handle to Process
            HANDLE hProcess = 0;
            hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

            //Resolve base address of the pointer chain
            uintptr_t dynamicPtrBaseAddr = moduleBase + 0x00E340B0;

            std::cout << "DynamicPtrBaseAddr = " << "0x" << std::hex << dynamicPtrBaseAddr << std::endl;

            //Resolve our ammo pointer chain
            std::vector<unsigned int> S5Offsets = { 0xEC, 0x226C, };
            uintptr_t S5Addr = FindDMAAddy(hProcess, dynamicPtrBaseAddr, S5Offsets);

            std::cout << "S5Addr = " << "0x" << std::hex << S5Addr << std::endl;

            //Read Ammo value
            int S5Value = S5Value; //lets it builds
            spot5 = S5Value;            // need to read to output box in imgui

            ReadProcessMemory(hProcess, (BYTE*)S5Addr, &S5Value, sizeof(S5Value), nullptr);
            std::cout << "Current 5th slot = " << std::dec << S5Value << std::endl;

            //Write to it
            int newS5 = 0; // define new value
            if (ImGui::InputInt("5th Slot", &newS5))

                WriteProcessMemory(hProcess, (BYTE*)S5Addr, &newS5, sizeof(newS5), nullptr);

            //Read out again
            ReadProcessMemory(hProcess, (BYTE*)S5Addr, &S5Value, sizeof(S5Value), nullptr);

        }

        if (ImGui::InputInt)
        {
            //Get ProcId of the target process
            DWORD procId = GetProcId(L"Re5dx9.exe");

            //Getmodulebaseaddress
            uintptr_t moduleBase = GetModuleBaseAddress(procId, L"Re5dx9.exe");

            //Get Handle to Process
            HANDLE hProcess = 0;
            hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

            //Resolve base address of the pointer chain
            uintptr_t dynamicPtrBaseAddr = moduleBase + 0x00E340B0;

            std::cout << "DynamicPtrBaseAddr = " << "0x" << std::hex << dynamicPtrBaseAddr << std::endl;

            //Resolve our ammo pointer chain
            std::vector<unsigned int> S6Offsets = { 0xEC, 0x229C, };
            uintptr_t S6Addr = FindDMAAddy(hProcess, dynamicPtrBaseAddr, S6Offsets);

            std::cout << "S6Addr = " << "0x" << std::hex << S6Addr << std::endl;

            //Read Ammo value
            int S6Value = S6Value; //lets it builds
            spot6 = S6Value;            // need to read to output box in imgui

            ReadProcessMemory(hProcess, (BYTE*)S6Addr, &S6Value, sizeof(S6Value), nullptr);
            std::cout << "Current 6th slot = " << std::dec << S6Value << std::endl;

            //Write to it
            int newS6 = 0; // define new value
            if (ImGui::InputInt("6th Slot", &newS6))

                WriteProcessMemory(hProcess, (BYTE*)S6Addr, &newS6, sizeof(newS6), nullptr);

            //Read out again
            ReadProcessMemory(hProcess, (BYTE*)S6Addr, &S6Value, sizeof(S6Value), nullptr);

        }

        if (ImGui::InputInt)
        {
            //Get ProcId of the target process
            DWORD procId = GetProcId(L"Re5dx9.exe");

            //Getmodulebaseaddress
            uintptr_t moduleBase = GetModuleBaseAddress(procId, L"Re5dx9.exe");

            //Get Handle to Process
            HANDLE hProcess = 0;
            hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

            //Resolve base address of the pointer chain
            uintptr_t dynamicPtrBaseAddr = moduleBase + 0x00E340B0;

            std::cout << "DynamicPtrBaseAddr = " << "0x" << std::hex << dynamicPtrBaseAddr << std::endl;

            //Resolve our ammo pointer chain
            std::vector<unsigned int> S7Offsets = { 0xEC, 0x22CC, };
            uintptr_t S7Addr = FindDMAAddy(hProcess, dynamicPtrBaseAddr, S7Offsets);

            std::cout << "S7Addr = " << "0x" << std::hex << S7Addr << std::endl;

            //Read Ammo value
            int S7Value = S7Value; //lets it builds
            spot7 = S7Value;            // need to read to output box in imgui

            ReadProcessMemory(hProcess, (BYTE*)S7Addr, &S7Value, sizeof(S7Value), nullptr);
            std::cout << "Current 7th slot = " << std::dec << S7Value << std::endl;

            //Write to it
            int newS7 = 0; // define new value
            if (ImGui::InputInt("7th Slot", &newS7))

                WriteProcessMemory(hProcess, (BYTE*)S7Addr, &newS7, sizeof(newS7), nullptr);

            //Read out again
            ReadProcessMemory(hProcess, (BYTE*)S7Addr, &S7Value, sizeof(S7Value), nullptr);

        }

        if (ImGui::InputInt)
        {
            //Get ProcId of the target process
            DWORD procId = GetProcId(L"Re5dx9.exe");

            //Getmodulebaseaddress
            uintptr_t moduleBase = GetModuleBaseAddress(procId, L"Re5dx9.exe");

            //Get Handle to Process
            HANDLE hProcess = 0;
            hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

            //Resolve base address of the pointer chain
            uintptr_t dynamicPtrBaseAddr = moduleBase + 0x00E340B0;

            std::cout << "DynamicPtrBaseAddr = " << "0x" << std::hex << dynamicPtrBaseAddr << std::endl;

            //Resolve our ammo pointer chain
            std::vector<unsigned int> S8Offsets = { 0xEC, 0x22FC, };
            uintptr_t S8Addr = FindDMAAddy(hProcess, dynamicPtrBaseAddr, S8Offsets);

            std::cout << "S8Addr = " << "0x" << std::hex << S8Addr << std::endl;

            //Read Ammo value
            int S8Value = S8Value; //lets it builds
            spot8 = S8Value;            // need to read to output box in imgui

            ReadProcessMemory(hProcess, (BYTE*)S8Addr, &S8Value, sizeof(S8Value), nullptr);
            std::cout << "Current 8th slot = " << std::dec << S8Value << std::endl;

            //Write to it
            int newS8 = 0; // define new value
            if (ImGui::InputInt("8th Slot", &newS8))

                WriteProcessMemory(hProcess, (BYTE*)S8Addr, &newS8, sizeof(newS8), nullptr);

            //Read out again
            ReadProcessMemory(hProcess, (BYTE*)S8Addr, &S8Value, sizeof(S8Value), nullptr);

        }

        if (ImGui::InputInt)
        {
            //Get ProcId of the target process
            DWORD procId = GetProcId(L"Re5dx9.exe");

            //Getmodulebaseaddress
            uintptr_t moduleBase = GetModuleBaseAddress(procId, L"Re5dx9.exe");

            //Get Handle to Process
            HANDLE hProcess = 0;
            hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

            //Resolve base address of the pointer chain
            uintptr_t dynamicPtrBaseAddr = moduleBase + 0x00E340B0;

            std::cout << "DynamicPtrBaseAddr = " << "0x" << std::hex << dynamicPtrBaseAddr << std::endl;

            //Resolve our ammo pointer chain
            std::vector<unsigned int> S9Offsets = { 0xEC, 0x232C, };
            uintptr_t S9Addr = FindDMAAddy(hProcess, dynamicPtrBaseAddr, S9Offsets);

            std::cout << "S9Addr = " << "0x" << std::hex << S9Addr << std::endl;

            //Read Ammo value
            int S9Value = S9Value; //lets it builds
            spot9 = S9Value;            // need to read to output box in imgui

            ReadProcessMemory(hProcess, (BYTE*)S9Addr, &S9Value, sizeof(S9Value), nullptr);
            std::cout << "Current 9th slot = " << std::dec << S9Value << std::endl;

            //Write to it
            int newS9 = 0; // define new value
            if (ImGui::InputInt("9th Slot", &newS9))

                WriteProcessMemory(hProcess, (BYTE*)S9Addr, &newS9, sizeof(newS9), nullptr);

            //Read out again
            ReadProcessMemory(hProcess, (BYTE*)S9Addr, &S9Value, sizeof(S9Value), nullptr);

        }

        if (ImGui::InputFloat)
        {
            //Get ProcId of the target process
            DWORD procId = GetProcId(L"Re5dx9.exe");

            //Getmodulebaseaddress
            uintptr_t moduleBase = GetModuleBaseAddress(procId, L"Re5dx9.exe");

            //Get Handle to Process
            HANDLE hProcess = 0;
            hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

            //Resolve base address of the pointer chain
            uintptr_t dynamicPtrBaseAddr = moduleBase + 0x00E340B0;

            std::cout << "DynamicPtrBaseAddr = " << "0x" << std::hex << dynamicPtrBaseAddr << std::endl;

            //Resolve our ammo pointer chain
            std::vector<unsigned int> sxOffsets = { 0xF0, 0x34, };
            uintptr_t sxAddr = FindDMAAddy(hProcess, dynamicPtrBaseAddr, sxOffsets);

            std::cout << "sxAddr = " << "0x" << std::hex << sxAddr << std::endl;

            //Read Ammo value
            float sxValue = sxValue; //lets it builds
            sx = sxValue;            // need to read to output box in imgui

            ReadProcessMemory(hProcess, (BYTE*)sxAddr, &sxValue, sizeof(sxValue), nullptr);
            std::cout << "Current sx = " << std::dec << sxValue << std::endl;

            //Write to it
            float newsx = 0; // define new value
            ImGui::SeparatorText("Sheva Location modification");
            if (ImGui::InputFloat("x Cord", &newsx))


                WriteProcessMemory(hProcess, (BYTE*)sxAddr, &newsx, sizeof(newsx), nullptr);

            //Read out again
            ReadProcessMemory(hProcess, (BYTE*)sxAddr, &sxValue, sizeof(sxValue), nullptr);

        }

        if (ImGui::InputFloat)
        {
            //Get ProcId of the target process
            DWORD procId = GetProcId(L"Re5dx9.exe");

            //Getmodulebaseaddress
            uintptr_t moduleBase = GetModuleBaseAddress(procId, L"Re5dx9.exe");

            //Get Handle to Process
            HANDLE hProcess = 0;
            hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

            //Resolve base address of the pointer chain
            uintptr_t dynamicPtrBaseAddr = moduleBase + 0x00E340B0;

            std::cout << "DynamicPtrBaseAddr = " << "0x" << std::hex << dynamicPtrBaseAddr << std::endl;

            //Resolve our ammo pointer chain
            std::vector<unsigned int> syOffsets = { 0xF0, 0x30, };
            uintptr_t syAddr = FindDMAAddy(hProcess, dynamicPtrBaseAddr, syOffsets);

            std::cout << "syAddr = " << "0x" << std::hex << syAddr << std::endl;

            //Read Ammo value
            float syValue = syValue;
            sy = syValue;

            ReadProcessMemory(hProcess, (BYTE*)syAddr, &syValue, sizeof(syValue), nullptr);
            std::cout << "Current sy = " << std::dec << syValue << std::endl;

            //Write to it
            float newsy = 0;
            if (ImGui::InputFloat("y Cord", &newsy))

                WriteProcessMemory(hProcess, (BYTE*)syAddr, &newsy, sizeof(newsy), nullptr);

            //Read out again
            ReadProcessMemory(hProcess, (BYTE*)syAddr, &syValue, sizeof(syValue), nullptr);

        }

        if (ImGui::InputFloat)
        {
            //Get ProcId of the target process
            DWORD procId = GetProcId(L"Re5dx9.exe");

            //Getmodulebaseaddress
            uintptr_t moduleBase = GetModuleBaseAddress(procId, L"Re5dx9.exe");

            //Get Handle to Process
            HANDLE hProcess = 0;
            hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

            //Resolve base address of the pointer chain
            uintptr_t dynamicPtrBaseAddr = moduleBase + 0x00E340B0;

            std::cout << "DynamicPtrBaseAddr = " << "0x" << std::hex << dynamicPtrBaseAddr << std::endl;

            //Resolve our ammo pointer chain
            std::vector<unsigned int> szOffsets = { 0xF0, 0x38, };
            uintptr_t szAddr = FindDMAAddy(hProcess, dynamicPtrBaseAddr, szOffsets);

            std::cout << "szAddr = " << "0x" << std::hex << szAddr << std::endl;

            //Read Ammo value
            float szValue = szValue;
            sz = szValue;

            ReadProcessMemory(hProcess, (BYTE*)szAddr, &szValue, sizeof(szValue), nullptr);
            std::cout << "Current sz = " << std::dec << szValue << std::endl;

            //Write to it
            float newsz = 0;
            if (ImGui::InputFloat("z Cord", &newsz))

                WriteProcessMemory(hProcess, (BYTE*)szAddr, &newsz, sizeof(newsz), nullptr);

            //Read out again
            ReadProcessMemory(hProcess, (BYTE*)szAddr, &szValue, sizeof(szValue), nullptr);

        }

        // Rendering
        ImGui::EndFrame();
        g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
        D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(clear_color.x*clear_color.w*255.0f), (int)(clear_color.y*clear_color.w*255.0f), (int)(clear_color.z*clear_color.w*255.0f), (int)(clear_color.w*255.0f));
        g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);
        if (g_pd3dDevice->BeginScene() >= 0)
        {
            ImGui::Render();
            ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
            g_pd3dDevice->EndScene();
        }
        HRESULT result = g_pd3dDevice->Present(NULL, NULL, NULL, NULL);

        // Handle loss of D3D9 device
        if (result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
            ResetDevice();
    }

    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}



// Helper functions

bool CreateDeviceD3D(HWND hWnd)
{
    if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
        return false;

    // Create the D3DDevice
    ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
    g_d3dpp.Windowed = TRUE;
    g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN; // Need to use an explicit format with alpha if needing per-pixel alpha composition.
    g_d3dpp.EnableAutoDepthStencil = TRUE;
    g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;           // Present with vsync
    //g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;   // Present without vsync, maximum unthrottled framerate
    if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice) < 0)
        return false;

    return true;
}

void CleanupDeviceD3D()
{
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
    if (g_pD3D) { g_pD3D->Release(); g_pD3D = NULL; }
}

void ResetDevice()
{
    ImGui_ImplDX9_InvalidateDeviceObjects();
    HRESULT hr = g_pd3dDevice->Reset(&g_d3dpp);
    if (hr == D3DERR_INVALIDCALL)
        IM_ASSERT(0);
    ImGui_ImplDX9_CreateDeviceObjects();
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
        {
            g_d3dpp.BackBufferWidth = LOWORD(lParam);
            g_d3dpp.BackBufferHeight = HIWORD(lParam);
            ResetDevice();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}
