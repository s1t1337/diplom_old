#include "render.hpp"
#include "../../context/cfg.hpp"
#include "../database/database.hpp"


extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

namespace render
{
    LPDIRECT3D9  g_pD3D = NULL;
    LPDIRECT3DDEVICE9  g_pd3dDevice = NULL;
    D3DPRESENT_PARAMETERS g_d3dpp = {};



    LRESULT WINAPI WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
    {
        if ( ImGui_ImplWin32_WndProcHandler( hWnd, msg, wParam, lParam ) )
            return true;

        static POINTS m;
        ImGuiIO& io = ImGui::GetIO(); ( void )io;
        switch ( msg )
        {
        case WM_SIZE:
            if ( g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED )
            {
                g_d3dpp.BackBufferWidth = LOWORD( lParam );
                g_d3dpp.BackBufferHeight = HIWORD( lParam );
                ResetDevice();
            }
            return 0;

        case WM_LBUTTONDOWN:
            io.MouseDown[ 0 ] = true;
            SetCapture( hWnd );
            m = MAKEPOINTS( lParam );
            return 0;
            break;
        case WM_LBUTTONUP:
            ReleaseCapture();
            io.MouseDown[ 0 ] = false;
            return 0;
            break;

        case WM_MOUSEMOVE:
            io.MousePos.x = ( signed short )( lParam );
            io.MousePos.y = ( signed short )( lParam >> 16 );

            if ( wParam == MK_LBUTTON )
            {
                POINTS p = MAKEPOINTS( lParam );
                RECT rect;
                GetWindowRect( hWnd, &rect );

                rect.left += p.x - m.x;
                rect.top += p.y - m.y;

                SetWindowPos( hWnd, HWND_TOPMOST, rect.left, rect.top, 0, 0,
                    SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOZORDER );
            }
            return 0;
            break;
        case WM_SYSCOMMAND:
            if ( ( wParam & 0xfff0 ) == SC_KEYMENU )
                return 0;
            break;
        case WM_DESTROY:
            ::PostQuitMessage( 0 );
            return 0;

        }
        return ::DefWindowProc( hWnd, msg, wParam, lParam );

    }

    bool CreateDeviceD3D( HWND hWnd )
    {
        if ( ( g_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) == NULL )
            return false;

        ZeroMemory( &g_d3dpp, sizeof( g_d3dpp ) );
        g_d3dpp.Windowed = TRUE;
        g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
        g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
        g_d3dpp.EnableAutoDepthStencil = TRUE;
        g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
        g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
        if ( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice ) < 0 )
            return false;

        return true;
    }

    void CleanupDeviceD3D()
    {
        if ( g_pd3dDevice )
        {
            g_pd3dDevice->Release();
            g_pd3dDevice = NULL;
        }

        if ( g_pD3D )
        {
            g_pD3D->Release();
            g_pD3D = NULL;
        }
    }

    void ResetDevice()
    {
        ImGui_ImplDX9_InvalidateDeviceObjects();
        HRESULT hr = g_pd3dDevice->Reset( &g_d3dpp );
        if ( hr == D3DERR_INVALIDCALL )
            IM_ASSERT( 0 );
        ImGui_ImplDX9_CreateDeviceObjects();
    }

    void render_tab( const std::string_view name_tab, const int index, bool user)
    {

        ImGui::PushStyleColor( ImGuiCol_Button, ctx::cfg->global.active_tab == index ? ImVec4( 0.110f, 0.118f, 0.129f, 1.000f ) : ImVec4( 0.110f, 0.118f, 0.129f, 0.000f ) );
        ImGui::PushStyleColor( ImGuiCol_ButtonHovered, ImVec4( 0.110f, 0.118f, 0.129f, 1.000f ) );
        ImGui::PushStyleColor( ImGuiCol_ButtonActive, ImVec4( 0.110f, 0.118f, 0.129f, 1.000f ) );
        ImGui::PushStyleColor( ImGuiCol_Text, ctx::cfg->global.active_tab == index ? ImVec4( 1.000f, 1.000f, 1.000f, 1.000f ) : ImVec4( 0.639f, 0.690f, 0.737f, 1.000f ) );

        if ( ImGui::Button( name_tab.data(), ImVec2( 160.0f, 32.0f ) ) )
        {

            if ( name_tab == ctx::name_tabs[ ctx::tabs_id::orders ] )
                database::get_orders();

            if ( !user && name_tab == ctx::name_tabs[ ctx::tabs_id::users ] )
                database::get_all_users();

            if (name_tab == ctx::name_tabs[ ctx::tabs_id::sections ] && ctx::cfg->global.subtab_sections == ctx::subtab_sections_id::all_sections )
                database::get_sections();

            if ( user && name_tab == ctx::user_tabs[ ctx::tabs_id::reports ])
                database::get_all_guarantee();

            if ( !user && name_tab == ctx::name_tabs[ ctx::tabs_id::reports ] )
                database::get_reports();


            ctx::cfg->global.search_info.clear();

            ctx::cfg->global.active_tab = index;
        }


        if ( ctx::cfg->global.active_tab == index )
        {

            ImGui::GetCurrentWindow()->DrawList->AddRectFilled( ImGui::GetWindowPos() + ImVec2( ImGui::GetCursorPos().x + 3.0f, ImGui::GetCursorPos().y - 17.0f ),
                ImGui::GetWindowPos() + ImVec2( ImGui::GetCursorPos().x + 5.0f, ImGui::GetCursorPos().y - 17.0f - 18.0f ), IM_COL32( 231, 34, 66, 255 ), 2 );

        }

        ImGui::PopStyleColor( 4 );



    }

    bool render::Button( const char* label, const char* header, const char* description, const ImVec2& size_arg, bool delete_item, ImGuiButtonFlags flags )
    {
        ImGuiWindow* window = ImGui::GetCurrentWindow();

        if ( window->SkipItems )
            return false;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;

        const ImGuiID id = window->GetID( label );
        const ImVec2 label_size = ImGui::CalcTextSize( label, nullptr, true );

        ImVec2 pos = window->DC.CursorPos;


        ImVec2 size = ImGui::CalcItemSize( size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f );

        const ImRect bb( pos, pos + size + ImVec2( 0.0f, 45.0f ) );
        ImGui::ItemSize( size, style.FramePadding.y );

        if ( !ImGui::ItemAdd( bb, id ) )
            return false;

        if ( g.LastItemData.InFlags & ImGuiItemFlags_ButtonRepeat )
            flags |= ImGuiButtonFlags_Repeat;

        bool hovered, held;
        bool pressed = ImGui::ButtonBehavior( bb, id, &hovered, &held, flags );

        const ImU32 col = ImGui::GetColorU32( ( held && hovered ) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button );

        ImGui::RenderNavHighlight( bb, id );
        ImGui::RenderFrame( bb.Min, bb.Max - ImVec2( 0.0f, 40.0f ), col, true, style.FrameRounding );

        const ImRect default_bb( pos, pos + size );
        const ImVec2 bb_center = default_bb.GetCenter();

        window->DrawList->AddCircleFilled( bb_center, 28.44f / 2, ImColor( 51, 55, 60, 255 ), 360 );

        window->DrawList->AddLine( bb_center - ImVec2( 7.0f, 0.0f ), bb_center + ImVec2( 7.0f, 0.0f ), ImColor( 105, 112, 122, 255 ), 2.0f );

        if ( !delete_item )
            window->DrawList->AddLine( bb_center - ImVec2( 0.5f, 7.0f ), bb_center + ImVec2( -0.5f, 7.0f ), ImColor( 105, 112, 122, 255 ), 2.0f );

        window->DrawList->AddText( ImGui::GetFont(), 16.5f, bb.Max - ImVec2( size.x, 31.0f ), ImGui::GetColorU32( ImGuiCol_Text ), header );

        if ( description )
            window->DrawList->AddText( ImGui::GetFont(), 16.5f, bb.Max - ImVec2( size.x, 8.0f ), ImColor( 119, 133, 146 ), description );

        return pressed;
    }

    bool ImageButton( std::string_view item, std::string_view paintkit, int rarity, ImTextureID user_texture_id, const ImVec2& size,
        const char* item_id, int frame_padding, bool draw_image, bool draw_quality )
    {
        ImGuiWindow* window = ImGui::GetCurrentWindow();

        if ( window->SkipItems )
            return false;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;

        ImGui::PushID( ( void* )( intptr_t )user_texture_id );

        const ImGuiID id = window->GetID( item_id != nullptr ? item_id : item.data() );

        ImGui::PopID();

        const ImVec2 padding = ( frame_padding >= 0 ) ? ImVec2( ( float )frame_padding, ( float )frame_padding ) : style.FramePadding;
        const ImRect bb( window->DC.CursorPos, window->DC.CursorPos + size + padding * 2 + ImVec2( 0.0f, 40.0f ) );
        const ImRect image_bb( window->DC.CursorPos + padding, window->DC.CursorPos + padding + size - ImVec2( 2.0f, 21.0f ) );

        ImGui::ItemSize( bb );

        if ( !ImGui::ItemAdd( bb, id ) )
            return false;

        bool hovered, held;
        bool pressed = ImGui::ButtonBehavior( bb, id, &hovered, &held );

        const ImColor col = ( held && hovered ) ? ImColor( 10, 32, 55 ) : hovered ? ImColor( 7, 29, 52 ) : ImColor( 28, 30, 33, 255 );
        ImGui::RenderNavHighlight( bb, id );
        ImGui::RenderFrame( bb.Min, bb.Max - ImVec2( 0.0f, 40.0f ), col, true, ImClamp( ( float )ImMin( padding.x, padding.y ), 0.0f, style.FrameRounding ) );


        if ( draw_image && user_texture_id != nullptr )
            window->DrawList->AddImage( user_texture_id, image_bb.Min + ImVec2( 3.0f, 9.0f ), image_bb.Max + ImVec2( 2.0f, 10.0f ), ImVec2( 0.0f, 0.0f ), ImVec2( 1.0f, 1.0f ), ImGui::GetColorU32( ImVec4( 1.0f, 1.0f, 1.0f, 1.0f ) ) );


        ImColor quality_color = ImColor( 226, 76, 84 );

        switch ( rarity )
        {
        case 1:
            quality_color = IM_COL32_WHITE;
            break;
        case 2:
            quality_color = IM_COL32( 166, 201, 255, 255 ); // light blue
            break;
        case 3:
            quality_color = IM_COL32( 73, 138, 235, 255 ); // blue
            break;
        case 4:
            quality_color = IM_COL32( 160, 32, 240, 255 ); // purple
            break;
        case 5:
            quality_color = IM_COL32( 232, 73, 235, 255 ); // pink
            break;
        case 6:
            quality_color = IM_COL32( 167, 94, 93, 255 ); // red
            break;
        case 7:
            quality_color = IM_COL32( 255, 153, 0, 255 ); // yellow
            break;
        default:
            quality_color = IM_COL32( 169, 169, 169, 255 );
            break;
        }

        if ( draw_quality )
            window->DrawList->AddRectFilled( bb.GetBL() - ImVec2( 0, 46.0f ), bb.GetBR() - ImVec2( 0, 42.0f ), quality_color, 25.0f, ImDrawCornerFlags_Bot );


        const ImVec2 label_size = ImGui::CalcTextSize( item.data(), NULL, true );
        const float offset = size.x >= label_size.x + style.FramePadding.x * 2.0f ? size.x + style.FramePadding.x : static_cast< int >( g.Time * 60.f ) % static_cast< int >( label_size.x + size.x + style.FramePadding.x * 2.f + 4 );
        const ImRect text = ImRect( ImVec2( bb.Min.x + size.x - offset + style.FramePadding.x * 2.f, bb.Max.y - 31.0f ), bb.Max );

        ImGui::RenderTextClipped( text.Min - ImVec2( 5.0f, 0.0f ), text.Max, item.data(), NULL, &label_size, ImVec2( 0, 0 ), &bb );


        if ( !paintkit.empty() )
            window->DrawList->AddText( ImGui::GetFont(), 16.5f, bb.Max - ImVec2( size.x + 8.5f, 8.0f ), ImColor( 119, 133, 146 ), paintkit.data() );

        return pressed;
    }

    namespace image_texture
    {
        IDirect3DTexture9* user_logo = nullptr;
    }
}