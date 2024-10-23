#include <format>

#include "../menu.hpp"
#include "../../render/render.hpp"
#include "../../../context/cfg.hpp"
#include "../../database/database.hpp"

void menu::users_tab()
{

    static int select_id = -1;

    if ( ctx::cfg->global.subtab_users == ctx::subtab_users_id::list_all_users )
    {
        ImGui::PushStyleVar( ImGuiStyleVar_FramePadding, ImVec2( 24.0f, 8.0f ) );
        ImGui::PushStyleVar( ImGuiStyleVar_FrameBorderSize, 1.5f );
        ImGui::PushStyleVar( ImGuiStyleVar_FrameRounding, 3.0f );

        ImGui::PushItemWidth( 250.0f );
        ImGui::SetCursorPos( { 510.0f,20.0f } );
        ImGui::InputTextWithHint( "##search", "Search", &ctx::cfg->global.search_info );
        ImGui::PopItemWidth();
        ImGui::PopStyleVar( 3 );

        ImGui::SetCursorPosY( 70.0f );
        ImGui::BeginTable( "##user_list", 2, ImGuiTableFlags_ScrollY | ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_BordersInnerV, ImVec2( 580.0f, 370.0f ), 10.0f );
        {
            ImGui::TableSetupColumn( reinterpret_cast< const char* >( u8"Логин" ), ImGuiTableColumnFlags_None );
            ImGui::TableSetupColumn( reinterpret_cast< const char* >( u8"Почта" ), ImGuiTableColumnFlags_None );
            ImGui::TableHeadersRow();


            for ( const auto& user_get : ctx::cfg->users_info )
            {

                if ( !( user_get.second.login.find( ctx::cfg->global.search_info ) != std::string::npos ) ) // search
                    continue;

                ImGui::TableNextRow( ImGuiTableRowFlags_None );

                ImGui::TableSetColumnIndex( 0 );
                ImGui::Text( user_get.second.login.c_str() );

                ImGui::TableSetColumnIndex( 1 );
                ImGui::Text( user_get.second.email.c_str() );

                ImGui::SameLine();

                ImGui::PushID( user_get.first );
                if ( ImGui::Selectable( "##check_user", &render::select_table, ImGuiSelectableFlags_SpanAllColumns ) )
                {
                    render::select_table = false;

                    select_id = user_get.first;

                    ctx::cfg->global.subtab_users = ctx::subtab_users_id::edit_user;

                }
                ImGui::PopID();

            }
        }
        ImGui::EndTable();
    }
    else if ( ctx::cfg->global.subtab_users == ctx::subtab_users_id::edit_user )
    {
        ImGui::GetWindowDrawList()->AddText( ImGui::GetFont(), 15.0f, ImVec2( ( ImGui::GetWindowWidth() + 200.0f - ImGui::GetFont()->CalcTextSizeA( 15.0f, FLT_MAX, 0.0f, reinterpret_cast< const char* >( u8"Редактирование пользователя" ) ).x ) / 2, 10.0f ), ImColor( 255, 255, 255, 255 ), reinterpret_cast< const char* >( u8"Редактирование пользователя" ) );

        ImGui::SetCursorPos( { 200.0f,40.0f } );
        ImGui::BeginGroup();
        {

            ImGui::PushStyleVar( ImGuiStyleVar_FrameRounding, 3.0f );
            ImGui::PushStyleVar( ImGuiStyleVar_FramePadding, ImVec2( 24.0f, 8.0f ) );
            ImGui::PushStyleVar( ImGuiStyleVar_FrameBorderSize, 1.5f );


            ImGui::Text( reinterpret_cast< const char* >( u8"Логин" ) );
            ImGui::InputText( "##login", &ctx::cfg->users_info[ select_id ].login );

            ImGui::Text( reinterpret_cast< const char* >( u8"ФИО" ) );
            ImGui::InputText( "##fio", &ctx::cfg->users_info[ select_id ].FIO );

            ImGui::Text( reinterpret_cast< const char* >( u8"Номер телефона" ) );
            ImGui::InputText( "##number", &ctx::cfg->users_info[ select_id ].number );

            ImGui::Text( reinterpret_cast< const char* >( u8"Почта" ) );
            ImGui::InputText( "##email", &ctx::cfg->users_info[ select_id ].email );

            ImGui::Text( reinterpret_cast< const char* >( u8"Дата рождения" ) );
            ImGui::InputText( "##date1", &ctx::cfg->users_info[ select_id ].date );

            ImGui::PopStyleVar( 2 );


            ImGui::Checkbox( "Admin panel", &ctx::cfg->users_info[ select_id ].is_admin );


            ImGui::PushStyleColor( ImGuiCol_Button, ImVec4( 0.906f, 0.133f, 0.259f, 1.000f ) );
            ImGui::PushStyleColor( ImGuiCol_ButtonHovered, ImVec4( 0.110f, 0.118f, 0.129f, 1.000f ) );
            ImGui::PushStyleColor( ImGuiCol_ButtonActive, ImVec4( 0.110f, 0.118f, 0.129f, 1.000f ) );

            if ( ImGui::Button( reinterpret_cast< const char* >( u8"Сохранить изменения" ), ImVec2( 195.0f, 30.0f ) ) )
            {

                database::update_currect_user( std::format( R"(UPDATE "users" SET "Login" = '{}',"PhoneNumber" = '{}', "FullName" = '{}', "Email" = '{}', "BirthDate" = '{}', "IsAdmin" = {} WHERE "Id" = {};)", ctx::cfg->users_info[ select_id ].login, ctx::cfg->users_info[ select_id ].number, ctx::cfg->users_info[ select_id ].FIO, ctx::cfg->users_info[ select_id ].email, ctx::cfg->users_info[ select_id ].date, ctx::cfg->users_info[ select_id ].is_admin, ctx::cfg->users_info[ select_id ].id ) );

                ctx::cfg->global.subtab_users = ctx::subtab_users_id::list_all_users;

            }
            ImGui::SameLine();
            if ( ImGui::Button( reinterpret_cast< const char* >( u8"Удалить пользователя" ), ImVec2( 195.0f, 30.0f ) ) )
            {

                ctx::cfg->global.subtab_users = ctx::subtab_users_id::list_all_users;
                database::delete_currect_user( ctx::cfg->users_info[ select_id ].id );

            }

            ImGui::PopStyleColor( 3 );
            ImGui::PopStyleVar();
        }
        ImGui::EndGroup();
    }
}
