
#include "../menu.hpp"
#include "../../render/render.hpp"
#include "../../../context/cfg.hpp"
#include "../../database/database.hpp"

void menu::reports_tab()
{
	static int select_id = -1;

    if ( ctx::cfg->global.subtab_reports == ctx::subtab_reports_id::all_reports )
    {
        int all_price = 0;

        ImGui::PushStyleVar( ImGuiStyleVar_FramePadding, ImVec2( 24.0f, 8.0f ) );
        ImGui::PushStyleVar( ImGuiStyleVar_FrameBorderSize, 1.5f );
        ImGui::PushStyleVar( ImGuiStyleVar_FrameRounding, 3.0f );

        ImGui::PushItemWidth( 250.0f );
        ImGui::SetCursorPos( { 510.0f,20.0f } );
        ImGui::InputTextWithHint( "##search", "Search", &ctx::cfg->global.search_info );
        ImGui::PopItemWidth();
        ImGui::PopStyleVar( 3 );


        ImGui::SetCursorPosY( 70.0f );
        ImGui::BeginTable( "##reports_list", 2, ImGuiTableFlags_ScrollY | ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_BordersInnerV, ImVec2( 580.0f, 350.0f ), 10.0f );
        {
            ImGui::TableSetupColumn( reinterpret_cast< const char* >( u8"Дата" ), ImGuiTableColumnFlags_None );
            ImGui::TableSetupColumn( reinterpret_cast< const char* >( u8"Выручка в рублях" ), ImGuiTableColumnFlags_None );
            ImGui::TableHeadersRow();


            for ( const auto& reports_get : ctx::cfg->reports_info )
            {

                if ( !( reports_get.second.date.find( ctx::cfg->global.search_info ) != std::string::npos ) ) // search
                    continue;

                ImGui::TableNextRow( ImGuiTableRowFlags_None );

                ImGui::TableSetColumnIndex( 0 );
                ImGui::Text( reports_get.second.date.c_str() );

                ImGui::TableSetColumnIndex( 1 );
                ImGui::Text( std::to_string( reports_get.second.all_sum ).c_str() );

                ImGui::SameLine();

                ImGui::PushID( reports_get.first );
                if ( ImGui::Selectable( "##check_reports", &render::select_table, ImGuiSelectableFlags_SpanAllColumns ) )
                {
                    render::select_table = false;

                    select_id = reports_get.first;

                    database::get_info_report( select_id );
                    ctx::cfg->global.subtab_reports = ctx::subtab_reports_id::select_report;

                }
                ImGui::PopID();

            }
        }
        ImGui::EndTable();


        for ( auto const& summ : ctx::cfg->reports_info )
        {
            all_price += summ.second.all_sum;
        }

        if ( !ctx::cfg->reports_info.empty() )
            ImGui::Text( reinterpret_cast< const char* >( u8"Общая выручка: %d рублей" ), all_price );
    }
    else if ( ctx::cfg->global.subtab_reports == ctx::subtab_reports_id::select_report )
    {
        int all_price = 0;
        ImGui::BeginTable( "##reports_list_select", 2, ImGuiTableFlags_ScrollY | ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_BordersInnerV, ImVec2( 580.0f, 390.0f ), 10.0f );
        {
            ImGui::TableSetupColumn( reinterpret_cast< const char* >( u8"Наименование раздела" ), ImGuiTableColumnFlags_None );
            ImGui::TableSetupColumn( reinterpret_cast< const char* >( u8"Сумма" ), ImGuiTableColumnFlags_None );
            ImGui::TableHeadersRow();


            for ( const auto& select_report_get : ctx::cfg->reports_select_info )
            {

                ImGui::TableNextRow( ImGuiTableRowFlags_None );

                ImGui::TableSetColumnIndex( 0 );
                ImGui::Text( ctx::type_item[ select_report_get.second.type_id_item - 1] );

                ImGui::TableSetColumnIndex( 1 );
                ImGui::Text( std::to_string( select_report_get.second.price ).c_str() );



            }
        }
        ImGui::EndTable();

        for ( auto const&  summ : ctx::cfg->reports_select_info )
        {
            all_price += summ.second.price;
        }

        if( !ctx::cfg->reports_select_info.empty() )
        ImGui::Text( reinterpret_cast< const char* >( u8"Общая выручка: %d рублей" ), all_price );
    }
}