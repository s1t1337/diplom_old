#include <format>

#include "../menu.hpp"
#include "../../render/render.hpp"
#include "../../../context/cfg.hpp"
#include "../../database/database.hpp"

void menu::basket_tab()
{
    ImGui::PushStyleColor( ImGuiCol_ChildBg, ImVec4( 0.0f, 0.0f, 0.0f, 0.0f ) );
    ImGui::BeginChild( "##list_basket", ImVec2( 580.0f, 390.0f ), false, render::flag_child );
    {
        ImGui::SetCursorPosX( 10 );
        ImGui::BeginGroup();
        {
            int count_items = 0;

            ImGui::PushStyleColor( ImGuiCol_Button, ImVec4( 0.110f, 0.118f, 0.129f, 1.000f ) );
            ImGui::PushStyleVar( ImGuiStyleVar_FrameRounding, 4.0f );
            ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2( 9.0f, 29.0f ) );

            for ( auto it = ctx::cfg->basket_info.begin(); it != ctx::cfg->basket_info.end(); )
            {

                if ( render::ImageButton( it->second.name, std::to_string( it->second.price ).c_str(), it->first, it->second.picture, ImVec2( 173.0f, 143.0f ), std::to_string( it->first ).c_str(), 4, true, true ) )
                {

                    it = ctx::cfg->basket_info.erase( it );
                }
                else
                    ++it;

                count_items++;

                if ( ( count_items % 3 ) != 0 )
                    ImGui::SameLine();
            }

            ImGui::Spacing();
            ImGui::Spacing();

            ImGui::PopStyleColor();
            ImGui::PopStyleVar( 2 );
        }
        ImGui::EndGroup();
    }
    ImGui::EndChild();
    ImGui::PopStyleColor();


    if ( ImGui::Button( reinterpret_cast< const char* >( u8"Сохранить корзину" ), ImVec2( 580.0f, 30.0f ) ) )
    {

        time_t rawtime;
        struct tm* timeinfo;
        char buffer[ 80 ];

        time( &rawtime );
        timeinfo = localtime( &rawtime );

        strftime( buffer, 80, "%d.%m.%Y", timeinfo );

        int all_price = 0, all_cout = 0;

        for ( auto const& check : ctx::cfg->basket_info )
        {
            all_price += check.second.price;
            all_cout++;
        }


       auto id =  database::add_order( std::format( R"(INSERT INTO "orders" ("AllCount","AllPrice","IsReady","UserId","Time") VALUES ({},{},false,{},'{}'); SELECT max("Id") FROM "orders";)", all_cout, all_price, ctx::cfg->user_settings.id, buffer ) );


       for ( auto const& send_info : ctx::cfg->basket_info )
       {

           database::add_basket_items( std::format( R"(INSERT INTO "basket_items" ("Price","OrdersId","ItemsId") VALUES ({},{},{});)", send_info.second.price,id,send_info.second.item_id ) );
       }

       ctx::cfg->basket_info.clear();
    }
}