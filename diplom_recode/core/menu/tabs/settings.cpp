#include "../menu.hpp"
#include "../../render/render.hpp"
#include "../../../context/cfg.hpp"

void menu::settings_tab()
{
    //user image profile
    ImGui::SetCursorPos( { 220.0f,25.0f } );
    ImGui::Image( render::image_texture::user_logo, ImVec2( 100.0f, 100.0f ) );

    //user login
    ImGui::SetCursorPos( { ( 550.0f - ImGui::GetFont()->CalcTextSizeA( ImGui::GetFontSize() , FLT_MAX, 0.0f, ctx::cfg->user_settings.login.c_str() ).x ) / 2,145.0f } );
    ImGui::TextWrapped( ctx::cfg->user_settings.login.c_str() );

    //user edit info
    ImGui::SetCursorPos( { 380,25.0f } );
    ImGui::BeginChild( "##settings_user", ImVec2( 300.0f, 370.0f ), true, render::flag_child );
    {

        ImGui::SetCursorPos( { 55.0f,15.0f } );
        ImGui::BeginGroup();
        {

            ImGui::PushStyleVar( ImGuiStyleVar_FrameRounding, 3.0f );
            ImGui::PushStyleVar( ImGuiStyleVar_FramePadding, ImVec2( 24.0f, 8.0f ) );
            ImGui::PushStyleVar( ImGuiStyleVar_FrameBorderSize, 1.5f );

            ImGui::Text( reinterpret_cast< const char* >( u8"ФИО" ) );
            ImGui::InputText( "##fio", &ctx::cfg->user_settings.FIO );

            ImGui::Text( reinterpret_cast< const char* >( u8"Номер телефона" ) );
            ImGui::InputText( "##number", &ctx::cfg->user_settings.number );

            ImGui::Text( reinterpret_cast< const char* >( u8"Почта" ) );
            ImGui::InputText( "##email", &ctx::cfg->user_settings.email );

            ImGui::Text( reinterpret_cast< const char* >( u8"Дата рождения" ) );
            ImGui::InputText( "##date1", &ctx::cfg->user_settings.date );

            ImGui::PopStyleVar( 2 );


            if ( ctx::cfg->user_settings.is_admin )
                ImGui::Checkbox( "Admin panel", &ctx::cfg->user_settings.admin_panel );


            ImGui::PushStyleColor( ImGuiCol_Button, ImVec4( 0.906f, 0.133f, 0.259f, 1.000f ) );
            ImGui::PushStyleColor( ImGuiCol_ButtonHovered, ImVec4( 0.110f, 0.118f, 0.129f, 1.000f ) );
            ImGui::PushStyleColor( ImGuiCol_ButtonActive, ImVec4( 0.110f, 0.118f, 0.129f, 1.000f ) );

            if ( ImGui::Button( reinterpret_cast< const char* >( u8"Сохранить изменения" ), ImVec2( 195.0f, 30.0f ) ) )
            {

            }

            ImGui::PopStyleColor( 3 );
            ImGui::PopStyleVar();
        }
        ImGui::EndGroup();

    }
    ImGui::EndChild();
}