
#include "../menu.hpp"
#include "../../render/render.hpp"
#include "../../../context/cfg.hpp"
#include "../../database/database.hpp"

void menu::guarantee_tab()
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
    ImGui::BeginTable( "##guarantee_list", 2, ImGuiTableFlags_ScrollY | ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_BordersInnerV, ImVec2( 580.0f, 370.0f ), 10.0f );
    {
        ImGui::TableSetupColumn( reinterpret_cast< const char* >( u8"Товар" ), ImGuiTableColumnFlags_None );
        ImGui::TableSetupColumn( reinterpret_cast< const char* >( u8"Гарантия до" ), ImGuiTableColumnFlags_None );
        ImGui::TableHeadersRow();


        for ( const auto& guarantee_get : ctx::cfg->guarantee_info )
        {

            if ( !( guarantee_get.second.name.find( ctx::cfg->global.search_info ) != std::string::npos ) ) // search
                continue;

            ImGui::TableNextRow( ImGuiTableRowFlags_None );

            ImGui::TableSetColumnIndex( 0 );
            ImGui::Text( guarantee_get.second.name.c_str() );

            ImGui::TableSetColumnIndex( 1 );
            ImGui::Text( guarantee_get.second.date.c_str() );

        }
    }
    ImGui::EndTable();
}