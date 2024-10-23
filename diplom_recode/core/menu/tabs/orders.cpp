#include "../menu.hpp"
#include "../../render/render.hpp"
#include "../../../context/cfg.hpp"
#include "../../database/database.hpp"

void menu::orders_tab()
{
    static int select_id = -1;

    if ( ctx::cfg->global.subtab_orders == ctx::subtab_orders_id::orders_info )
    {
        ImGui::BeginTable( "##orders_list", 4, ImGuiTableFlags_ScrollY | ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_BordersInnerV, ImVec2( 580.0f, 425.0f ), 10.0f );
        {

            ImGui::TableSetupColumn( reinterpret_cast< const char* >( u8"Номер заказа" ), ImGuiTableColumnFlags_None );
            ImGui::TableSetupColumn( reinterpret_cast< const char* >( u8"Дата" ), ImGuiTableColumnFlags_None );
            ImGui::TableSetupColumn( reinterpret_cast< const char* >( u8"Сумма" ), ImGuiTableColumnFlags_None );
            ImGui::TableSetupColumn( reinterpret_cast< const char* >( u8"Статус" ), ImGuiTableColumnFlags_None );
            ImGui::TableHeadersRow();


            for ( const auto& test_render_info : ctx::cfg->orders_info )
            {
                if ( !ctx::cfg->user_settings.admin_panel && ctx::cfg->user_settings.id != test_render_info.second.user_id )
                    continue;

                ImGui::TableNextRow( ImGuiTableRowFlags_None );


                ImGui::TableSetColumnIndex( 0 );
                ImGui::Text( std::to_string( test_render_info.second.id ).c_str() );

                ImGui::TableSetColumnIndex( 1 );
                ImGui::Text( test_render_info.second.time.c_str() );

                ImGui::TableSetColumnIndex( 2 );
                ImGui::Text( std::to_string( test_render_info.second.all_price ).c_str() );

                ImGui::TableSetColumnIndex( 3 );
                ImGui::TextColored( test_render_info.second.is_ready == true ? ImVec4( 0.000f, 0.753f, 0.266f, 1.000f ) : ImVec4( 1.0f, 1.0f, 1.0f, 1.0f ), test_render_info.second.is_ready == true ? reinterpret_cast< const char* >( u8"Подтвержден" ) : reinterpret_cast< const char* >( u8"Не подтвержден" ) );

                ImGui::SameLine();
                ImGui::PushID( test_render_info.first );
                if ( ImGui::Selectable( "##check", &render::select_table, ImGuiSelectableFlags_SpanAllColumns ) )
                {

                    render::select_table = false;

                    select_id = test_render_info.first;
                    database::get_order_by_id( select_id );
                     ctx::cfg->global.subtab_orders =  ctx::subtab_orders_id::edit_orders;

                }
                ImGui::PopID();


            }
        }
        ImGui::EndTable();
    }
    else if ( ctx::cfg->global.subtab_orders == ctx::subtab_orders_id::edit_orders )
    {
        ImGui::BeginTable( "##order_edit", 4, ImGuiTableFlags_ScrollY | ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_BordersInnerV, ImVec2( 580.0f, 350.0f ), 10.0f );
        {
            ImGui::TableSetupColumn( reinterpret_cast< const char* >( u8"Имя товара" ), ImGuiTableColumnFlags_None );
            ImGui::TableSetupColumn( reinterpret_cast< const char* >( u8"Тип" ), ImGuiTableColumnFlags_None );
            ImGui::TableSetupColumn( reinterpret_cast< const char* >( u8"Кол-во" ), ImGuiTableColumnFlags_None );
            ImGui::TableSetupColumn( reinterpret_cast< const char* >( u8"Сумма" ), ImGuiTableColumnFlags_None );
            ImGui::TableHeadersRow();


            for ( const auto& test_render_info : ctx::cfg->select_orders_info )
            {

                ImGui::TableNextRow( ImGuiTableRowFlags_None );



                ImGui::TableSetColumnIndex( 0 );
                ImGui::Text( test_render_info.second.name.c_str() );

                ImGui::TableSetColumnIndex( 1 );
                ImGui::Text( ctx::type_item[ test_render_info.second.type_item_id - 1 ] );

                ImGui::TableSetColumnIndex( 2 );
                ImGui::Text( std::to_string( test_render_info.second.count ).c_str() );

                ImGui::TableSetColumnIndex( 3 );
                ImGui::Text( std::to_string( test_render_info.second.price ).c_str() );


            }
        }
        ImGui::EndTable();

        //select buttons
        ImGui::PushStyleVar( ImGuiStyleVar_FrameRounding, 4.0f );

        if ( ctx::cfg->user_settings.admin_panel )
            ImGui::PushStyleColor( ImGuiCol_Button, ImVec4( 0.906f, 0.133f, 0.259f, 1.000f ) );

        ImGui::PushStyleColor( ImGuiCol_ButtonHovered, ImVec4( 0.110f, 0.118f, 0.129f, 1.000f ) );
        ImGui::PushStyleColor( ImGuiCol_ButtonActive, ImVec4( 0.110f, 0.118f, 0.129f, 1.000f ) );


        if ( !ctx::cfg->orders_info[ select_id ].is_ready && ctx::cfg->user_settings.admin_panel )
        {
            if ( ImGui::Button( reinterpret_cast< const char* >( u8"Подвердить заказ" ), ImVec2( 580.0f, 30.0f ) ) )
            {
                //to do

                database::accept_currect_order( select_id );
                ctx::cfg->global.subtab_orders = ctx::subtab_orders_id::orders_info;
            }

            ImGui::PopStyleColor(); // ImGuiCol_Button
        }
          
         

        ImGui::PushStyleColor( ImGuiCol_Button, ImVec4( 0.110f, 0.118f, 0.129f, 1.000f ) );
        if ( ImGui::Button( reinterpret_cast< const char* >( u8"Отменить заказ" ), ImVec2( 580.0f, 30.0f ) ) )
        {
            database::delete_currect_order( select_id );
            ctx::cfg->global.subtab_orders = ctx::subtab_orders_id::orders_info;
        }

        ImGui::PopStyleColor( 3 );
        ImGui::PopStyleVar();
    }
}