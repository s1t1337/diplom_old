

#include "menu.hpp"
#include "../render/render.hpp"
#include "../../context/cfg.hpp"


#include "../../dependencies/images/user_icon.h"

namespace menu
{

	void init()
	{
        D3DXCreateTextureFromFileInMemoryEx( render::g_pd3dDevice, &usericon, sizeof( usericon ), 100, 100, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &render::image_texture::user_logo );


		ImGui::GetIO().Fonts->AddFontFromFileTTF( "C:\\Windows\\Fonts\\Verdana.ttf", 15.0f, nullptr, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic() );
	}

	void render_menu()
	{
		ImGui::SetNextWindowPos( { 0.0f,0.0f } );
		ImGui::SetNextWindowSize( ImVec2( render::menu_size.x, render::menu_size.y ), ImGuiCond_Always );
		if ( ImGui::Begin( "##main", nullptr, render::flag_begin ) )
		{

			if ( ctx::cfg->global.active_tab == ctx::tabs_id::auth )
			{
				auth_tab();
			}
			else if ( ctx::cfg->global.active_tab == ctx::tabs_id::reg )
			{
				reg_tab();
			}
			else
			{
                ImGui::BeginChild( "##header", ImVec2( 180.0f, render::menu_size.y ), false, render::flag_child );
                {

                    ImGui::GetWindowDrawList()->AddText( ImGui::GetFont(), ImGui::GetFontSize(), ImVec2( ( 180.0f - ImGui::GetFont()->CalcTextSizeA( ImGui::GetFontSize(), FLT_MAX, 0.0f, reinterpret_cast< const char* >( u8"Учет продаж" ) ).x ) / 2, 15.0f ), ImColor( 255, 255, 255, 255 ), reinterpret_cast< const char* >( u8"Учет продаж" ) );

                    ImGui::GetWindowDrawList()->AddText( ImGui::GetFont(), ImGui::GetFontSize(), ImVec2( ( 180.0f - ImGui::GetFont()->CalcTextSizeA( ImGui::GetFontSize(), FLT_MAX, 0.0f, reinterpret_cast< const char* >( u8"компьютерной техники" ) ).x ) / 2, 30.0f ), ImColor( 255, 255, 255, 255 ), reinterpret_cast< const char* >( u8"компьютерной техники" ) );

                    ImGui::SetCursorPos( { 12.0f,62.0f } );
                    ImGui::BeginGroup();
                    {
                        ImGui::PushStyleVar( ImGuiStyleVar_FrameRounding, 4.0f );

                        if ( ctx::cfg->user_settings.admin_panel )
                        {
                            for ( size_t i = 0; i < _countof( ctx::name_tabs ); i++ )
                            {
                                render::render_tab( ctx::name_tabs[ i ], i );
                            }
                        }
                        else
                        {
                            for ( size_t i = 0; i < _countof( ctx::user_tabs ); i++ )
                            {
                                render::render_tab( ctx::user_tabs[ i ], i, true );
                            }
                        }

                        ImGui::PopStyleVar();

                    }
                    ImGui::EndGroup();


                    ImGui::PushStyleVar( ImGuiStyleVar_FrameRounding, 4.0f );
                    ImGui::PushStyleColor( ImGuiCol_Button, ImVec4( 0.110f, 0.118f, 0.129f, 1.000f ) );


                    if ( ctx::cfg->global.subtab_users > 0 || ctx::cfg->global.subtab_orders > 0 || ctx::cfg->global.subtab_reports > 0 || ctx::cfg->global.subtab_sections > 0 )
                    {
                        ImGui::SetCursorPos( { 12.0f , 370.0f } );
                        if ( ImGui::Button( reinterpret_cast< const char* >( u8"Назад" ), ImVec2( 160.0f, 32.0f ) ) )
                        {
                            if ( ctx::cfg->global.subtab_users > 0 )
                            {
                                ctx::cfg->global.subtab_users -= 1;
                            }

                            if ( ctx::cfg->global.subtab_orders > 0 )
                            {
                                ctx::cfg->global.subtab_orders -= 1;
                            }

                            if ( ctx::cfg->global.subtab_reports > 0 )
                            {
                                ctx::cfg->global.subtab_reports -= 1;
                            }

                            if ( ctx::cfg->global.subtab_sections > 0 )
                            {

                                switch ( ctx::cfg->global.subtab_sections )
                                {
                                case ctx::subtab_sections_id::add_new_section:
                                    ctx::cfg->global.subtab_sections = 0;
                                    break;
                                case ctx::subtab_sections_id::open_currect_section:
                                    ctx::cfg->global.subtab_sections = 0;
                                    break;
                                case ctx::subtab_sections_id::add_new_item_in_currect_section:
                                    ctx::cfg->global.subtab_sections = 2;
                                    break;
                                case ctx::subtab_sections_id::open_currect_item_in_section:
                                    ctx::cfg->global.subtab_sections = 2;
                                    break;
                                default:
                                    break;
                                }
                            }

                            render::select_table = false;
                        }
                    }

                    ImGui::SetCursorPos( { 12.0f , 410.0f } );
                    if ( ImGui::Button( reinterpret_cast< const char* >( u8"Выход" ), ImVec2( 160.0f, 32.0f ) ) )
                        exit( 0 );

                    ImGui::PopStyleVar();
                    ImGui::PopStyleColor();


                }
                ImGui::EndChild();

                ImGui::SetCursorPos( { 190.0f,15.0f } );
                ImGui::BeginGroup();
                {
                    switch ( ctx::cfg->global.active_tab )
                    {
                        case ctx::tabs_id::orders:
                        {
                            orders_tab();
                            break;
                        }

                        case ctx::tabs_id::reports:
                        {
                            if ( ctx::cfg->user_settings.admin_panel )
                            {
                                reports_tab();
                            }
                            else
                            {
                                guarantee_tab();
                            }
                            break;
                        }

                        case ctx::tabs_id::sections:
                        {
                            sections_tab();
                            break;
                        }

                        case ctx::tabs_id::users:
                        {
                            if ( ctx::cfg->user_settings.admin_panel )
                            {
                                users_tab();
                            }
                            else
                            {
                                basket_tab();
                            }
                            break;
                        }

                        case ctx::tabs_id::settings:
                        {
                            settings_tab();
                            break;
                        }

                        default:
                        break;
                    }

                }
                ImGui::EndGroup();
			}
		}
		ImGui::End();
	}
}