#include <Windows.h>


#include "core/render/render.hpp"
#include "core/menu/menu.hpp"
#include "context/cfg.hpp"
#include "core/helpers/helpers.hpp"
#include <iostream>

namespace ctx
{
    std::unique_ptr< cfg_t > cfg = nullptr;
}


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);


    WNDCLASSEX wc = { sizeof( WNDCLASSEX ), CS_CLASSDC, render::WndProc, 0L, 0L, GetModuleHandle( NULL ), NULL, NULL, NULL, NULL, L"test_demo", NULL };
    ::RegisterClassEx( &wc );

    const HWND hwnd = ::CreateWindow( wc.lpszClassName, L"test_demo", WS_POPUP, ( GetSystemMetrics( SM_CXSCREEN ) - render::menu_size.x ) / 2, ( GetSystemMetrics( SM_CYSCREEN ) - render::menu_size.y ) / 2, render::menu_size.x, render::menu_size.y, NULL, NULL, wc.hInstance, NULL );


    if ( !render::CreateDeviceD3D( hwnd ) )
    {
        MessageBoxA( 0, "Failed create_device_d3d", "[drx_device]", MB_ICONERROR );
        render::CleanupDeviceD3D();
        ::UnregisterClass( wc.lpszClassName, wc.hInstance );
        return 1;
    }

    ::ShowWindow( hwnd, SW_SHOWDEFAULT );
    ::UpdateWindow( hwnd );


    IMGUI_CHECKVERSION();
    ImGui::CreateContext();


    ctx::cfg = std::make_unique< ctx::cfg_t >();
    menu::init();

    ImGui::StyleColorsDark();

    //AllocConsole();
    //freopen( "CONOUT$", "wt", stdout );
    //freopen( "CONIN$", "rt", stdin );
    //freopen( "CONOUT$", "wt", stderr );





    ImGui_ImplWin32_Init( hwnd );
    ImGui_ImplDX9_Init( render::g_pd3dDevice );

    const ImVec4 clear_color = ImVec4( 0.45f, 0.55f, 0.60f, 1.00f );


    // Main loop
    MSG msg;
    ZeroMemory( &msg, sizeof( msg ) );
    while ( msg.message != WM_QUIT )
    {

        if ( ::PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
        {
            ::TranslateMessage( &msg );
            ::DispatchMessage( &msg );
            continue;
        }


        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        menu::render_menu();


        ImGui::EndFrame();
        render::g_pd3dDevice->SetRenderState( D3DRS_ZENABLE, FALSE );
        render::g_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
        render::g_pd3dDevice->SetRenderState( D3DRS_SCISSORTESTENABLE, FALSE );
        const D3DCOLOR clear_col_dx = D3DCOLOR_RGBA( ( int )( clear_color.x * 255.0f ), ( int )( clear_color.y * 255.0f ), ( int )( clear_color.z * 255.0f ), ( int )( clear_color.w * 255.0f ) );
        render::g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0 );
        if ( render::g_pd3dDevice->BeginScene() >= 0 )
        {
            ImGui::Render();
            ImGui_ImplDX9_RenderDrawData( ImGui::GetDrawData() );
            render::g_pd3dDevice->EndScene();
        }
        const HRESULT result = render::g_pd3dDevice->Present( NULL, NULL, NULL, NULL );

        // Handle loss of D3D9 device
        if ( result == D3DERR_DEVICELOST && render::g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET )
            render::ResetDevice();
    }

    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    render::CleanupDeviceD3D();
    ::DestroyWindow( hwnd );
    ::UnregisterClass( wc.lpszClassName, wc.hInstance );

    return 0;
}
