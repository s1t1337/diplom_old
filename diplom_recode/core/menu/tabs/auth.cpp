
#include "../menu.hpp"
#include "../../render/render.hpp"
#include "../../../context/cfg.hpp"
#include "../../database/database.hpp"

void menu::auth_tab()
{
    ImGui::GetWindowDrawList()->AddText( ImGui::GetFont(), 25.0f, ImVec2( ( ImGui::GetWindowWidth() - ImGui::GetFont()->CalcTextSizeA( 25.0f, FLT_MAX, 0.0f, reinterpret_cast< const char* >( u8"Авторизация" ) ).x ) / 2, 40.0f ), ImColor( 255, 255, 255, 255 ), reinterpret_cast< const char* >( u8"Авторизация" ) );

    ImGui::SetCursorPos( { 237.0f,100.0f } );
    ImGui::BeginGroup();
    {

        ImGui::PushItemWidth( 310.0f );

        ImGui::PushStyleVar( ImGuiStyleVar_FramePadding, ImVec2( 24.0f, 8.0f ) );
        ImGui::PushStyleVar( ImGuiStyleVar_FrameBorderSize, 1.5f );
        ImGui::PushStyleVar( ImGuiStyleVar_FrameRounding, 3.0f );

        ImGui::Text( reinterpret_cast< const char* >( u8"Логин" ) );
        ImGui::InputTextWithHint( "##login", reinterpret_cast< const char* >( u8"Логин" ), &ctx::cfg->user_settings.login, render::flag_login );

        ImGui::Text( reinterpret_cast< const char* >( u8"Пароль" ) );
        ImGui::InputTextWithHint( "##password", reinterpret_cast< const char* >( u8"Пароль" ), &ctx::cfg->user_settings.password, render::flag_pass );

        ImGui::PopStyleVar( 3 );
        ImGui::PopItemWidth();

        //reg button
        ImGui::PushStyleColor( ImGuiCol_Button, ImVec4( 0.0f, 0.0f, 0.0f, 0.0f ) );
        ImGui::PushStyleColor( ImGuiCol_ButtonHovered, ImVec4( 0.0f, 0.0f, 0.0f, 0.0f ) );
        ImGui::PushStyleColor( ImGuiCol_ButtonActive, ImVec4( 0.0f, 0.0f, 0.0f, 0.0f ) );

        if ( ImGui::Button( reinterpret_cast< const char* >( u8"Нет аккаунта?" ), ImVec2( 310.0f, 25.0f ) ) )
        {
            ctx::cfg->global.active_tab = ctx::tabs_id::reg;
        }

        ImGui::PopStyleColor( 3 );

        ImGui::PushStyleVar( ImGuiStyleVar_FrameRounding, 3.0f );

        if ( ImGui::Button( reinterpret_cast< const char* >( u8"Войти" ), ImVec2( 310.0f, 40.0f ) ) )
        {

            if ( database::auth_program() )
            {
               database::get_all_users();

                ctx::cfg->global.active_tab = ctx::tabs_id::none;
            }

        }

        ImGui::PushStyleColor( ImGuiCol_Button, ImVec4( 0.906f, 0.133f, 0.259f, 1.000f ) );
        if ( ImGui::Button( reinterpret_cast< const char* >( u8"Выйти" ), ImVec2( 310.0f, 40.0f ) ) )
        {
            exit( 0 );
        }
        ImGui::PopStyleColor();

        ImGui::PopStyleVar();
    }
    ImGui::EndGroup();
}