#include <iostream>
#include <fstream>
#include <format>

#include "../menu.hpp"
#include "../../render/render.hpp"
#include "../../../context/cfg.hpp"
#include "../../database/database.hpp"
#include "../../helpers/helpers.hpp"


void menu::sections_tab()
{
    static int select_id_category = -1, select_item_in_category = -1;
    static char filename[ MAX_PATH ];
    

    if ( ctx::cfg->global.subtab_sections == ctx::subtab_sections_id::add_new_item_in_currect_section )
    {
        ImGui::GetWindowDrawList()->AddText( ImGui::GetFont(), 15.0f, ImVec2( ( ImGui::GetWindowWidth() + 200.0f - ImGui::GetFont()->CalcTextSizeA( 15.0f, FLT_MAX, 0.0f, reinterpret_cast< const char* >( u8"Добавление товара" ) ).x ) / 2, 10.0f ), ImColor( 255, 255, 255, 255 ), reinterpret_cast< const char* >( u8"Добавление товара" ) );
    }
    else if ( ctx::cfg->global.subtab_sections == ctx::subtab_sections_id::add_new_section )
    {
        ImGui::GetWindowDrawList()->AddText( ImGui::GetFont(), 18.0f, ImVec2( ( ImGui::GetWindowWidth() + 200.0f - ImGui::GetFont()->CalcTextSizeA( 18.0f, FLT_MAX, 0.0f, reinterpret_cast< const char* >( u8"Добавление нового раздела" ) ).x ) / 2, 10.0f ), ImColor( 255, 255, 255, 255 ), reinterpret_cast< const char* >( u8"Добавление нового раздела" ) );
    }
    else if ( ctx::cfg->global.subtab_sections == ctx::subtab_sections_id::open_currect_item_in_section )
    {
        ImGui::GetWindowDrawList()->AddText( ImGui::GetFont(), 15.0f, ImVec2( ( ImGui::GetWindowWidth() + 200.0f - ImGui::GetFont()->CalcTextSizeA( 15.0f, FLT_MAX, 0.0f, ctx::cfg->user_settings.admin_panel ? reinterpret_cast< const char* >( u8"Редактирование товара" ) : reinterpret_cast< const char* >( u8"Посмотр товара" ) ).x ) / 2, 10.0f ), ImColor( 255, 255, 255, 255 ), ctx::cfg->user_settings.admin_panel ? reinterpret_cast< const char* >( u8"Редактирование товара" ) : reinterpret_cast< const char* >( u8"Посмотр товара" ) );
    }

    if ( ctx::cfg->global.subtab_sections != ctx::subtab_sections_id::add_new_section && ctx::cfg->global.subtab_sections != ctx::subtab_sections_id::add_new_item_in_currect_section && ctx::cfg->global.subtab_sections != ctx::subtab_sections_id::open_currect_item_in_section )
    {

        //search
        ImGui::PushStyleVar( ImGuiStyleVar_FramePadding, ImVec2( 24.0f, 8.0f ) );
        ImGui::PushStyleVar( ImGuiStyleVar_FrameBorderSize, 1.5f );
        ImGui::PushStyleVar( ImGuiStyleVar_FrameRounding, 3.0f );

        ImGui::PushItemWidth( 250.0f );
        ImGui::SetCursorPos( { 510.0f,10.0f } );
        ImGui::InputTextWithHint( "##search", "Search", &ctx::cfg->global.search_info, render::flag_login );

        ImGui::PopItemWidth();
        ImGui::PopStyleVar( 3 );
    }


    ImGui::PushStyleColor( ImGuiCol_ChildBg, ImVec4( 0.0f, 0.0f, 0.0f, 0.0f ));

    ImGui::SetCursorPos( { 182.0f,50.0f } );
    ImGui::BeginChild( "##inventory", ImVec2( 595.0f, 390.0f ), false, render::flag_child );
    {
        if ( ctx::cfg->global.subtab_sections == ctx::subtab_sections_id::all_sections )
        {
            ImGui::SetCursorPosX( 10 );
            ImGui::BeginGroup();
            {
                int count_items = ctx::cfg->user_settings.admin_panel ? 1 : 0;

                ImGui::PushStyleColor( ImGuiCol_Button, ImVec4( 0.110f, 0.118f, 0.129f, 1.000f ) );
                ImGui::PushStyleVar( ImGuiStyleVar_FrameRounding, 4.0f );
                ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2( 9.0f, 29.0f ) );

                if ( ctx::cfg->user_settings.admin_panel )
                {
                    if ( render::Button( "+", reinterpret_cast< const char* >( u8"Добавить новый раздел" ), NULL, ImVec2( 180.0f, 145.0f ), 0 ) )
                    {
                        ctx::cfg->global.subtab_sections = ctx::subtab_sections_id::add_new_section;
                    }
                    ImGui::SameLine();
                }

                for ( const auto & sections_get : ctx::cfg->section_info )
                {

                    if ( !( sections_get.second.name.find( ctx::cfg->global.search_info ) != std::string::npos ) ) // search
                        continue;

                    if ( render::ImageButton( sections_get.second.name, "", sections_get.first, sections_get.second.picture, ImVec2( 173.0f, 143.0f ), std::to_string( sections_get.first ).c_str(), 4, true, true ) )
                    {
                        ctx::cfg->global.subtab_sections = ctx::subtab_sections_id::open_currect_section;
                        select_id_category = sections_get.first;
                        database::get_item_in_currect_section( sections_get.second.id, true );
                    }

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
        else if ( ctx::cfg->global.subtab_sections == ctx::subtab_sections_id::add_new_section )
        {

            static std::string name_section;
            static char filename[ MAX_PATH ];

            ImGui::SetCursorPos( { 140.0f,30.0f } );
            ImGui::BeginGroup();
            {
                ImGui::PushItemWidth( 310.0f );

                ImGui::PushStyleVar( ImGuiStyleVar_FramePadding, ImVec2( 24.0f, 8.0f ) );
                ImGui::PushStyleVar( ImGuiStyleVar_FrameBorderSize, 1.5f );
                ImGui::PushStyleVar( ImGuiStyleVar_FrameRounding, 3.0f );

                ImGui::Text( reinterpret_cast< const char* >( u8"Название раздела" ) );
                ImGui::InputTextWithHint( "##name_section", reinterpret_cast< const char* >( u8"Название раздела" ),&name_section, render::flag_login );

                
                if ( ImGui::Button( reinterpret_cast< const char* >( u8"Выбрать картинку" ), ImVec2( 310.0f, 30.0f ) ) )
                {

                    OPENFILENAMEA ofn;
                    ZeroMemory( &filename, sizeof( filename ) );
                    ZeroMemory( &ofn, sizeof( ofn ) );
                    ofn.lStructSize = sizeof( ofn );
                    ofn.hwndOwner = NULL;  
                    ofn.lpstrFilter = "Png Files\0*.png\0Any File\0*.*\0";
                    ofn.lpstrFile = filename;
                    ofn.nMaxFile = MAX_PATH;
                    ofn.lpstrTitle =  "Select Image file" ;
                    ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

                    if ( GetOpenFileNameA( &ofn ) )
                    {
                        std::cout << "You chose the file \"" << filename << "\"\n";
                    }
                    else
                    {
                        // All this stuff below is to tell you exactly how you messed up above. 
                        // Once you've got that fixed, you can often (not always!) reduce it to a 'user cancelled' assumption.
                        switch ( CommDlgExtendedError() )
                        {
                        case CDERR_DIALOGFAILURE: std::cout << "CDERR_DIALOGFAILURE\n";   break;
                        case CDERR_FINDRESFAILURE: std::cout << "CDERR_FINDRESFAILURE\n";  break;
                        case CDERR_INITIALIZATION: std::cout << "CDERR_INITIALIZATION\n";  break;
                        case CDERR_LOADRESFAILURE: std::cout << "CDERR_LOADRESFAILURE\n";  break;
                        case CDERR_LOADSTRFAILURE: std::cout << "CDERR_LOADSTRFAILURE\n";  break;
                        case CDERR_LOCKRESFAILURE: std::cout << "CDERR_LOCKRESFAILURE\n";  break;
                        case CDERR_MEMALLOCFAILURE: std::cout << "CDERR_MEMALLOCFAILURE\n"; break;
                        case CDERR_MEMLOCKFAILURE: std::cout << "CDERR_MEMLOCKFAILURE\n";  break;
                        case CDERR_NOHINSTANCE: std::cout << "CDERR_NOHINSTANCE\n";     break;
                        case CDERR_NOHOOK: std::cout << "CDERR_NOHOOK\n";          break;
                        case CDERR_NOTEMPLATE: std::cout << "CDERR_NOTEMPLATE\n";      break;
                        case CDERR_STRUCTSIZE: std::cout << "CDERR_STRUCTSIZE\n";      break;
                        case FNERR_BUFFERTOOSMALL: std::cout << "FNERR_BUFFERTOOSMALL\n";  break;
                        case FNERR_INVALIDFILENAME: std::cout << "FNERR_INVALIDFILENAME\n"; break;
                        case FNERR_SUBCLASSFAILURE: std::cout << "FNERR_SUBCLASSFAILURE\n"; break;
                        default: std::cout << "You cancelled.\n";
                        }
                    }
                }

                if ( ImGui::Button( reinterpret_cast< const char* >( u8"Добавить" ), ImVec2( 310.0f, 30.0f ) ) )
                {

                    std::vector<uint8_t> raw_image = { 0 };
                    helpers::read_file_to_memory( filename, &raw_image );

                    std::string str( raw_image.begin(), raw_image.end() );
                   auto out =  helpers::b64encode( str );

                   str.clear();
                   auto id_image =  database::add_image( std::format( R"(INSERT INTO "images" ("ImageFile") VALUES ('{}'); SELECT max("Id") FROM "images";)", out ) );

                    out.clear();
                    raw_image.clear();


                    database::add_sections( std::format( R"(INSERT INTO "sections" ("Name","ImageId") VALUES ('{}',{});)",name_section, id_image ) );


                    ctx::cfg->global.subtab_sections = ctx::subtab_sections_id::all_sections;


                }


                ImGui::PopStyleVar( 3 );
                ImGui::PopItemWidth();

            }
            ImGui::EndGroup();
        }
        else if ( ctx::cfg->global.subtab_sections == ctx::subtab_sections_id::open_currect_section )
        {
            ImGui::SetCursorPosX( 10 );
            ImGui::BeginGroup();
            {
                int count_items = ctx::cfg->user_settings.admin_panel ? 2 : 0;

                ImGui::PushStyleColor( ImGuiCol_Button, ImVec4( 0.110f, 0.118f, 0.129f, 1.000f ) );
                ImGui::PushStyleVar( ImGuiStyleVar_FrameRounding, 4.0f );
                ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2( 9.0f, 29.0f ) );

                if ( ctx::cfg->user_settings.admin_panel )
                {
                    if ( render::Button( "+", reinterpret_cast< const char* >( u8"Добавить новый товар" ), NULL, ImVec2( 180.0f, 145.0f ), 0 ) )
                    {
                        memset( filename, 0, MAX_PATH );
                        ctx::cfg->global.subtab_sections = ctx::subtab_sections_id::add_new_item_in_currect_section;
                    }

                    ImGui::SameLine();

                    if ( render::Button( "-", reinterpret_cast< const char* >( u8"Удалить категорию" ), NULL, ImVec2( 180.0f, 145.0f ), true ) )
                    {

                        database::delete_currect_sections( select_id_category );
                        database::get_sections();
                        ctx::cfg->global.subtab_sections = ctx::subtab_sections_id::all_sections;
                    }

                    ImGui::SameLine();
                }

                for ( const auto& render_items : ctx::cfg->item_section_info )
                {
                    if ( !( render_items.second.name.find( ctx::cfg->global.search_info ) != std::string::npos ) ) // search
                        continue;

                    if ( render::ImageButton( render_items.second.name, render_items.second.price, render_items.first, render_items.second.picture, ImVec2( 173.0f, 143.0f ), std::to_string( render_items.first ).c_str(), 4, true, true ) )
                    {

                        select_item_in_category = render_items.first;

                        ctx::cfg->global.subtab_sections = ctx::subtab_sections_id::open_currect_item_in_section;


                        ctx::cfg->processor_render_edit = 
                        {{

                            { reinterpret_cast< const char* >( u8"Поколение процессоров" ), &ctx::cfg->item_section_info[ render_items.first].processor_type.Processor_ProcessorGeneration },
                            { reinterpret_cast< const char* >( u8"Сокет" ), &ctx::cfg->item_section_info[ render_items.first ].processor_type.Processor_Socket },
                            { reinterpret_cast< const char* >( u8"Количество ядер" ), &ctx::cfg->item_section_info[ render_items.first ].processor_type.Processor_NumberOfCores },
                            { reinterpret_cast< const char* >( u8"Макс. число потоков" ), &ctx::cfg->item_section_info[ render_items.first ].processor_type.Processor_MaxNumberOfThreads },
                            { reinterpret_cast< const char* >( u8"Ядро" ), &ctx::cfg->item_section_info[ render_items.first ].processor_type.Processor_Core },
                            { reinterpret_cast< const char* >( u8"Кэш L1 инструкции" ), &ctx::cfg->item_section_info[ render_items.first ].processor_type.Processor_CacheL1Instructions },
                            { reinterpret_cast< const char* >( u8"Кэш L1 данные" ), &ctx::cfg->item_section_info[ render_items.first ].processor_type.Processor_CacheL1Data },
                            { reinterpret_cast< const char* >( u8"Объем кэша L2" ), &ctx::cfg->item_section_info[ render_items.first ].processor_type.Processor_L2CacheSize },
                            { reinterpret_cast< const char* >( u8"Объем кэша L3" ), &ctx::cfg->item_section_info[ render_items.first ].processor_type.Processor_L3CacheSize },
                            { reinterpret_cast< const char* >( u8"Базовая частота" ), &ctx::cfg->item_section_info[ render_items.first ].processor_type.Processor_BaseFrequency },
                            { reinterpret_cast< const char* >( u8"Макс. частота(турбо)" ), &ctx::cfg->item_section_info[ render_items.first ].processor_type.Processor_MaxFrequencyTurbo },
                            { reinterpret_cast< const char* >( u8"Тип памяти" ), &ctx::cfg->item_section_info[ render_items.first ].processor_type.Processor_MemoryType }


                       }};

                        ctx::cfg->motherboard_render_edit =
                        {{

                            { reinterpret_cast< const char* >( u8"Форм-фактор" ), &ctx::cfg->item_section_info[ render_items.first ].motherboard_type.MotherBoard_FormFactor },
                            { reinterpret_cast< const char* >( u8"Высота" ), &ctx::cfg->item_section_info[ render_items.first ].motherboard_type.MotherBoard_Height },
                            { reinterpret_cast< const char* >( u8"Ширина" ), &ctx::cfg->item_section_info[ render_items.first ].motherboard_type.MotherBoard_Thickness },
                            { reinterpret_cast< const char* >( u8"Сокет" ),&ctx::cfg->item_section_info[ render_items.first ].motherboard_type.MotherBoard_Socket },
                            { reinterpret_cast< const char* >( u8"Чипсет" ),&ctx::cfg->item_section_info[ render_items.first ].motherboard_type.MotherBoard_Chipset },
                            { reinterpret_cast< const char* >( u8"Кол-во слотов ОЗУ" ),&ctx::cfg->item_section_info[ render_items.first ].motherboard_type.MotherBoard_CountRAMSlots },
                            { reinterpret_cast< const char* >( u8"Форм-фактор памяти" ),&ctx::cfg->item_section_info[ render_items.first ].motherboard_type.MotherBoard_MemoryFormFactor },
                            { reinterpret_cast< const char* >( u8"Тип памяти" ),&ctx::cfg->item_section_info[ render_items.first ].motherboard_type.MotherBoard_MemoryType },
                            { reinterpret_cast< const char* >( u8"Кол-во каналов памяти" ),&ctx::cfg->item_section_info[ render_items.first ].motherboard_type.MotherBoard_CountMemoryChannels },
                            { reinterpret_cast< const char* >( u8"Макс. объем памяти" ),&ctx::cfg->item_section_info[ render_items.first ].motherboard_type.MotherBoard_MaxMemoryCapacity },
                            { reinterpret_cast< const char* >( u8"Макс частота памяти" ),&ctx::cfg->item_section_info[ render_items.first ].motherboard_type.MotherBoard_MaxMemoryFrequency },

                            { reinterpret_cast< const char* >( u8" Частота ОЗУ" ),&ctx::cfg->item_section_info[ render_items.first ].motherboard_type.MotherBoard_RAMFrequency },
                            { reinterpret_cast< const char* >( u8"Кол-во разъемонов М.2" ),&ctx::cfg->item_section_info[ render_items.first ].motherboard_type.MotherBoard_CountConnectorsM2 },
                            { reinterpret_cast< const char* >( u8"Разъемы М.2" ),&ctx::cfg->item_section_info[ render_items.first ].motherboard_type.MotherBoard_ConnectorsM2 },
                            { reinterpret_cast< const char* >( u8"Кол-во портов SATA" ),&ctx::cfg->item_section_info[ render_items.first ].motherboard_type.MotherBoard_CountSATAPorts },
                            { reinterpret_cast< const char* >( u8"Другие разъемы накопителей" ),&ctx::cfg->item_section_info[ render_items.first ].motherboard_type.MotherBoard_OtherDriveConnectors }

                        }};

                        ctx::cfg->videocard_render_edit =
                        {{

                            { reinterpret_cast< const char* >( u8"Форм-фактор" ), &ctx::cfg->item_section_info[ render_items.first ].videocard_type.VideoCard_FormFactor },
                            { reinterpret_cast< const char* >( u8"Длина" ), &ctx::cfg->item_section_info[ render_items.first ].videocard_type.VideoCard_Length },
                            { reinterpret_cast< const char* >( u8"Толщина" ), &ctx::cfg->item_section_info[ render_items.first ].videocard_type.VideoCard_Thickness },
                            { reinterpret_cast< const char* >( u8"Объем памяти" ),&ctx::cfg->item_section_info[ render_items.first ].videocard_type.VideoCard_MemoryCapacity },
                            { reinterpret_cast< const char* >( u8"Тип памяти" ),&ctx::cfg->item_section_info[ render_items.first ].videocard_type.VideoCard_MemoryType },
                            { reinterpret_cast< const char* >( u8"Видеоразъемы" ),&ctx::cfg->item_section_info[ render_items.first ].videocard_type.VideoCard_VideoConnectors },
                            { reinterpret_cast< const char* >( u8"Версия HDMI" ),&ctx::cfg->item_section_info[ render_items.first ].videocard_type.VideoCard_HDMIVersion },
                            { reinterpret_cast< const char* >( u8"Макс. разрешение" ),&ctx::cfg->item_section_info[ render_items.first ].videocard_type.VideoCard_MaxResolution },
                            { reinterpret_cast< const char* >( u8"Кол-во подкл. мониторов" ),&ctx::cfg->item_section_info[ render_items.first ].videocard_type.VideoCard_CountMonitors },
                            { reinterpret_cast< const char* >( u8"Интерфейс подключения" ),&ctx::cfg->item_section_info[ render_items.first ].videocard_type.VideoCard_ConnectionInterface },
                            { reinterpret_cast< const char* >( u8"Версия PCI Express" ),&ctx::cfg->item_section_info[ render_items.first ].videocard_type.VideoCard_PCIExpressVersion }

                        }};

                        ctx::cfg->ram_render_edit =
                        {{

                            { reinterpret_cast< const char* >( u8"Тип памяти" ), &ctx::cfg->item_section_info[ render_items.first ].ram_type.RAM_MemoryType },
                            { reinterpret_cast< const char* >( u8"Форм-фактор" ), &ctx::cfg->item_section_info[ render_items.first ].ram_type.RAM_FormFactor },
                            { reinterpret_cast< const char* >( u8"Объем памяти" ), &ctx::cfg->item_section_info[ render_items.first ].ram_type.RAM_MemoryCapacity },
                            { reinterpret_cast< const char* >( u8"Тактовая частота" ),&ctx::cfg->item_section_info[ render_items.first ].ram_type.RAM_ClockFrequency }

                        }};

                        ctx::cfg->power_supply_render_edit =
                        {{

                            { reinterpret_cast< const char* >( u8"Форм-фактор" ), &ctx::cfg->item_section_info[ render_items.first ].power_supply_type.PowerSupplyUnit_FormFactor },
                            { reinterpret_cast< const char* >( u8"Мощность" ), &ctx::cfg->item_section_info[ render_items.first ].power_supply_type.PowerSupplyUnit_Power },
                            { reinterpret_cast< const char* >( u8"Отстегивающие кабели" ), &ctx::cfg->item_section_info[ render_items.first ].power_supply_type.PowerSupplyUnit_DetachableCables },
                            { reinterpret_cast< const char* >( u8"Питание PCI-E" ), &ctx::cfg->item_section_info[ render_items.first ].power_supply_type.PowerSupplyUnit_PCIEPowerSupply },
                            { reinterpret_cast< const char* >( u8"Ширина" ), &ctx::cfg->item_section_info[ render_items.first ].power_supply_type.PowerSupplyUnit_Thickness },
                            { reinterpret_cast< const char* >( u8"Высота" ), &ctx::cfg->item_section_info[ render_items.first ].power_supply_type.PowerSupplyUnit_Height }

                        }};

                        ctx::cfg->body_render_edit =
                        { {

                            { reinterpret_cast< const char* >( u8"Типоразмер корпуса" ), &ctx::cfg->item_section_info[ render_items.first ].body_type.Body_FormFactor },
                            { reinterpret_cast< const char* >( u8"Ориентация мат.платы" ), &ctx::cfg->item_section_info[ render_items.first ].body_type.Body_OrientationMatBoards },
                            { reinterpret_cast< const char* >( u8"Длина" ), &ctx::cfg->item_section_info[ render_items.first ].body_type.Body_Length },
                            { reinterpret_cast< const char* >( u8"Ширина" ),&ctx::cfg->item_section_info[ render_items.first ].body_type.Body_Thickness },
                            { reinterpret_cast< const char* >( u8"Высота" ),&ctx::cfg->item_section_info[ render_items.first ].body_type.Body_Height },
                            { reinterpret_cast< const char* >( u8"Форм-фактор мат.платы" ),&ctx::cfg->item_section_info[ render_items.first ].body_type.Body_FormFactorMatBoards },
                            { reinterpret_cast< const char* >( u8"Форм-фактор БП" ),&ctx::cfg->item_section_info[ render_items.first ].body_type.Body_BPFormFactor },
                            { reinterpret_cast< const char* >( u8"Размещение БП" ),&ctx::cfg->item_section_info[ render_items.first ].body_type.Body_BP_Placement },
                            { reinterpret_cast< const char* >( u8"Расположение I/O панели" ),&ctx::cfg->item_section_info[ render_items.first ].body_type.Body_LocationIOPanel },
                            { reinterpret_cast< const char* >( u8"Пылевой фильтр" ),&ctx::cfg->item_section_info[ render_items.first ].body_type.Body_DustFilter },
                            { reinterpret_cast< const char* >( u8"Боковые вентеляторы" ),&ctx::cfg->item_section_info[ render_items.first ].body_type.Body_SideFans },
                            { reinterpret_cast< const char* >( u8"Нижные вентеляторы" ),&ctx::cfg->item_section_info[ render_items.first ].body_type.Body_LowerFans },
                            { reinterpret_cast< const char* >( u8"Верхние вентеляторы" ),&ctx::cfg->item_section_info[ render_items.first ].body_type.Body_UpperFans },
                            { reinterpret_cast< const char* >( u8"Фронтальные вентеляторы" ),&ctx::cfg->item_section_info[ render_items.first ].body_type.Body_FrontFans },
                            { reinterpret_cast< const char* >( u8"Вентеляторы в комплекте" ),&ctx::cfg->item_section_info[ render_items.first ].body_type.Body_FansIncluded }

                        } };

                        ctx::cfg->ssd_render_edit =
                        {{

                            { reinterpret_cast< const char* >( u8"Объем памяти" ), &ctx::cfg->item_section_info[ render_items.first ].ssd_type.SSD_MemoryCapacity },
                            { reinterpret_cast< const char* >( u8"Физ. интерфейс" ), &ctx::cfg->item_section_info[ render_items.first ].ssd_type.SSD_PhysInterface },
                            { reinterpret_cast< const char* >( u8"Макс. скорость послед. записи" ), &ctx::cfg->item_section_info[ render_items.first ].ssd_type.SSD_MaxSpeedRecordings },
                            { reinterpret_cast< const char* >( u8"Макс. скорость послед. чтения" ), &ctx::cfg->item_section_info[ render_items.first ].ssd_type.SSD_MaxSpeedReadings },
                            { reinterpret_cast< const char* >( u8"Скорость произв. записи 4кб" ), &ctx::cfg->item_section_info[ render_items.first ].ssd_type.SSD_SpeedProductionRecords },
                            { reinterpret_cast< const char* >( u8"Скорость произв. чтения 4кб" ), &ctx::cfg->item_section_info[ render_items.first ].ssd_type.SSD_SpeedProductionReadings }

                        }};

                        ctx::cfg->hard_drivers_render_edit =
                        {{

                            { reinterpret_cast< const char* >( u8"Объем памяти" ), &ctx::cfg->item_section_info[ render_items.first ].hard_drivers_type.HardDrives_MemoryCapacity },
                            { reinterpret_cast< const char* >( u8"Интерфейс" ), &ctx::cfg->item_section_info[ render_items.first ].hard_drivers_type.HardDrives_PhysInterface },
                            { reinterpret_cast< const char* >( u8"Пропускная способность" ), &ctx::cfg->item_section_info[ render_items.first ].hard_drivers_type.HardDrives_Bandwidth },
                            { reinterpret_cast< const char* >( u8"Среднее время задержки" ), &ctx::cfg->item_section_info[ render_items.first ].hard_drivers_type.HardDrives_AverageDelayTime },
                            { reinterpret_cast< const char* >( u8"Макс. скорость передачи данных" ), &ctx::cfg->item_section_info[ render_items.first ].hard_drivers_type.HardDrives_MaxDataTransferRate }

                        }};

                        ctx::cfg->computer_cooling_render_edit =
                        {{

                            { reinterpret_cast< const char* >( u8"Сокет" ), &ctx::cfg->item_section_info[ render_items.first ].computer_cooling_type.ComputerCooling_Socket },
                            { reinterpret_cast< const char* >( u8"Размер" ), &ctx::cfg->item_section_info[ render_items.first ].computer_cooling_type.ComputerCooling_Size },
                            { reinterpret_cast< const char* >( u8"Разъем для поключения" ), &ctx::cfg->item_section_info[ render_items.first ].computer_cooling_type.ComputerCooling_ConnectorForConnection },
                            { reinterpret_cast< const char* >( u8"Макс. скорость вращения" ), &ctx::cfg->item_section_info[ render_items.first ].computer_cooling_type.ComputerCooling_MaxRotationSpeed },
                            { reinterpret_cast< const char* >( u8"Мин. скорость вращения" ), &ctx::cfg->item_section_info[ render_items.first ].computer_cooling_type.ComputerCooling_MinRotationSpeed },
                            { reinterpret_cast< const char* >( u8"Регулировка скорости" ), &ctx::cfg->item_section_info[ render_items.first ].computer_cooling_type.ComputerCooling_SpeedAdjustment },
                            { reinterpret_cast< const char* >( u8"Макс. уровень шума" ), &ctx::cfg->item_section_info[ render_items.first ].computer_cooling_type.ComputerCooling_MaxNoiseLevel }

                        }};
                    }

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
        else if ( ctx::cfg->global.subtab_sections == ctx::subtab_sections_id::add_new_item_in_currect_section )
        {

            ImColor quality_color = ImColor( 226, 76, 84 );

            switch ( 1 )
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

            ImGui::GetWindowDrawList()->AddRectFilled( ImGui::GetWindowPos() + ImVec2( 10.0f, 0.0f ),
                ImGui::GetWindowPos() + ImVec2( 280.0f, 248.0f ), IM_COL32( 28, 30, 33, 255 ), 5.0f, ImDrawCornerFlags_Top | ImDrawCornerFlags_Bot );

            if ( ctx::cfg->item_section.picture )
            {
                ImGui::SetCursorPos( { 25.0f,44.0f } );
                ImGui::Image( ctx::cfg->item_section.picture, { 230.0f,160.0f } );
            }


            ImGui::SetCursorPos( { 10.0f, 255.0f } );
            if ( ImGui::Button( reinterpret_cast< const char* >( u8"Выбрать картинку" ), ImVec2( 270.0f, 25.0f ) ) )
            {
                memset( filename, 0, MAX_PATH );

                OPENFILENAMEA ofn;
                ZeroMemory( &filename, sizeof( filename ) );
                ZeroMemory( &ofn, sizeof( ofn ) );
                ofn.lStructSize = sizeof( ofn );
                ofn.hwndOwner = NULL;
                ofn.lpstrFilter = "Png Files\0*.png\0Any File\0*.*\0";
                ofn.lpstrFile = filename;
                ofn.nMaxFile = MAX_PATH;
                ofn.lpstrTitle = "Select Image file";
                ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

                if ( GetOpenFileNameA( &ofn ) )
                {
                    std::cout << "You chose the file \"" << filename << "\"\n";
                }
                else
                {
                    // All this stuff below is to tell you exactly how you messed up above. 
                    // Once you've got that fixed, you can often (not always!) reduce it to a 'user cancelled' assumption.
                    switch ( CommDlgExtendedError() )
                    {
                    case CDERR_DIALOGFAILURE: std::cout << "CDERR_DIALOGFAILURE\n";   break;
                    case CDERR_FINDRESFAILURE: std::cout << "CDERR_FINDRESFAILURE\n";  break;
                    case CDERR_INITIALIZATION: std::cout << "CDERR_INITIALIZATION\n";  break;
                    case CDERR_LOADRESFAILURE: std::cout << "CDERR_LOADRESFAILURE\n";  break;
                    case CDERR_LOADSTRFAILURE: std::cout << "CDERR_LOADSTRFAILURE\n";  break;
                    case CDERR_LOCKRESFAILURE: std::cout << "CDERR_LOCKRESFAILURE\n";  break;
                    case CDERR_MEMALLOCFAILURE: std::cout << "CDERR_MEMALLOCFAILURE\n"; break;
                    case CDERR_MEMLOCKFAILURE: std::cout << "CDERR_MEMLOCKFAILURE\n";  break;
                    case CDERR_NOHINSTANCE: std::cout << "CDERR_NOHINSTANCE\n";     break;
                    case CDERR_NOHOOK: std::cout << "CDERR_NOHOOK\n";          break;
                    case CDERR_NOTEMPLATE: std::cout << "CDERR_NOTEMPLATE\n";      break;
                    case CDERR_STRUCTSIZE: std::cout << "CDERR_STRUCTSIZE\n";      break;
                    case FNERR_BUFFERTOOSMALL: std::cout << "FNERR_BUFFERTOOSMALL\n";  break;
                    case FNERR_INVALIDFILENAME: std::cout << "FNERR_INVALIDFILENAME\n"; break;
                    case FNERR_SUBCLASSFAILURE: std::cout << "FNERR_SUBCLASSFAILURE\n"; break;
                    default: std::cout << "You cancelled.\n";
                    }
                }

                if ( filename[ 0 ] != '\0' )
                {
                    std::vector<uint8_t> raw_image = { 0 };
                    helpers::read_file_to_memory( filename, &raw_image );

                    std::string str( raw_image.begin(), raw_image.end() );

                    D3DXCreateTextureFromFileInMemoryEx( render::g_pd3dDevice, ( LPVOID )str.c_str(), str.size(), 173, 143, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &ctx::cfg->item_section.picture );

                    raw_image.clear();
                    str.clear();
                }

            }

            ImGui::PushItemWidth( 270.0f );

            ImGui::SetCursorPos( { 10.0f,285.0f } );
            ImGui::Combo( "##type_item", &ctx::cfg->global.currect_type_item, ctx::type_item, IM_ARRAYSIZE( ctx::type_item ) );

            ImGui::PopItemWidth();

            ImGui::PushStyleColor( ImGuiCol_Button, ImVec4( 0.906f, 0.133f, 0.259f, 1.000f ) );

            ImGui::SetCursorPos( { 10.0f, 310.0f } );
            if ( ImGui::Button( reinterpret_cast< const char* >( u8"Добавить" ), ImVec2( 270.0f, 35.0f ) ) )
            {
                std::vector<uint8_t> raw_image = { 0 };
                helpers::read_file_to_memory( filename, &raw_image );

                std::string str( raw_image.begin(), raw_image.end() );
                auto out = helpers::b64encode( str );

                str.clear();
                auto id_image = database::add_image( std::format( R"(INSERT INTO "images" ("ImageFile") VALUES ('{}'); SELECT max("Id") FROM "images";)", out ) );

                out.clear();
                raw_image.clear();


                std::string sql_request_type_item = {};

                if ( ctx::cfg->global.currect_type_item == 0 )
                {
                    sql_request_type_item = std::format( R"(INSERT INTO "items" ("Name","ImageId","TypeId","SectionsId","Price","Guarantee","Processor_ProcessorGeneration","Processor_Socket","Processor_NumberOfCores","Processor_MaxNumberOfThreads","Processor_Core","Processor_CacheL1Instructions","Processor_CacheL1Data","Processor_L2CacheSize","Processor_L3CacheSize","Processor_BaseFrequency","Processor_MaxFrequencyTurbo", "Processor_MemoryType") VALUES ('{}',{},{},{},'{}','{}','{}','{}','{}','{}','{}','{}','{}','{}','{}','{}','{}','{}'); )", ctx::cfg->item_section.name, id_image, ctx::cfg->global.currect_type_item + 1, ctx::cfg->section_info[ select_id_category ].id, ctx::cfg->item_section.price, ctx::cfg->item_section.guarantee, ctx::cfg->item_section.processor_type.Processor_ProcessorGeneration, ctx::cfg->item_section.processor_type.Processor_Socket, ctx::cfg->item_section.processor_type.Processor_NumberOfCores, ctx::cfg->item_section.processor_type.Processor_MaxNumberOfThreads, ctx::cfg->item_section.processor_type.Processor_Core, ctx::cfg->item_section.processor_type.Processor_CacheL1Instructions, ctx::cfg->item_section.processor_type.Processor_CacheL1Data, ctx::cfg->item_section.processor_type.Processor_L2CacheSize, ctx::cfg->item_section.processor_type.Processor_L3CacheSize, ctx::cfg->item_section.processor_type.Processor_BaseFrequency, ctx::cfg->item_section.processor_type.Processor_MaxFrequencyTurbo, ctx::cfg->item_section.processor_type.Processor_MemoryType );
                }
                else if ( ctx::cfg->global.currect_type_item == 1 )
                {
                    sql_request_type_item = std::format( R"(INSERT INTO "items" ("Name","ImageId","TypeId","SectionsId","Price","Guarantee","MotherBoard_FormFactor","MotherBoard_Height","MotherBoard_Thickness","MotherBoard_Socket","MotherBoard_Chipset","MotherBoard_CountRAMSlots","MotherBoard_MemoryFormFactor","MotherBoard_MemoryType","MotherBoard_CountMemoryChannels","MotherBoard_MaxMemoryCapacity","MotherBoard_MaxMemoryFrequency","MotherBoard_RAMFrequency","MotherBoard_CountConnectorsM2","MotherBoard_ConnectorsM2","MotherBoard_CountSATAPorts","MotherBoard_OtherDriveConnectors") VALUES ('{}',{},{},{},'{}','{}', '{}','{}','{}','{}','{}','{}','{}','{}','{}','{}','{}','{}','{}','{}','{}','{}'); )", ctx::cfg->item_section.name, id_image, ctx::cfg->global.currect_type_item + 1, ctx::cfg->section_info[ select_id_category ].id, ctx::cfg->item_section.price, ctx::cfg->item_section.guarantee, ctx::cfg->item_section.motherboard_type.MotherBoard_FormFactor, ctx::cfg->item_section.motherboard_type.MotherBoard_Height, ctx::cfg->item_section.motherboard_type.MotherBoard_Thickness, ctx::cfg->item_section.motherboard_type.MotherBoard_Socket, ctx::cfg->item_section.motherboard_type.MotherBoard_Chipset, ctx::cfg->item_section.motherboard_type.MotherBoard_CountRAMSlots, ctx::cfg->item_section.motherboard_type.MotherBoard_MemoryFormFactor, ctx::cfg->item_section.motherboard_type.MotherBoard_MemoryType, ctx::cfg->item_section.motherboard_type.MotherBoard_CountMemoryChannels, ctx::cfg->item_section.motherboard_type.MotherBoard_MaxMemoryCapacity, ctx::cfg->item_section.motherboard_type.MotherBoard_MaxMemoryFrequency, ctx::cfg->item_section.motherboard_type.MotherBoard_RAMFrequency, ctx::cfg->item_section.motherboard_type.MotherBoard_CountConnectorsM2, ctx::cfg->item_section.motherboard_type.MotherBoard_ConnectorsM2, ctx::cfg->item_section.motherboard_type.MotherBoard_CountSATAPorts, ctx::cfg->item_section.motherboard_type.MotherBoard_OtherDriveConnectors );
                }
                else if ( ctx::cfg->global.currect_type_item == 2 )
                {
                    sql_request_type_item = std::format( R"(INSERT INTO "items" ("Name","ImageId","TypeId","SectionsId","Price","Guarantee","VideoCard_FormFactor","VideoCard_Length","VideoCard_Thickness","VideoCard_MemoryCapacity","VideoCard_MemoryType","VideoCard_VideoConnectors","VideoCard_HDMIVersion","VideoCard_MaxResolution","VideoCard_CountMonitors","VideoCard_ConnectionInterface","VideoCard_PCIExpressVersion") VALUES ('{}',{},{},{},'{}','{}', '{}','{}','{}','{}','{}','{}','{}','{}','{}','{}','{}');)", ctx::cfg->item_section.name, id_image, ctx::cfg->global.currect_type_item + 1, ctx::cfg->section_info[ select_id_category ].id, ctx::cfg->item_section.price, ctx::cfg->item_section.guarantee, ctx::cfg->item_section.videocard_type.VideoCard_FormFactor, ctx::cfg->item_section.videocard_type.VideoCard_Length, ctx::cfg->item_section.videocard_type.VideoCard_Thickness, ctx::cfg->item_section.videocard_type.VideoCard_MemoryCapacity, ctx::cfg->item_section.videocard_type.VideoCard_MemoryType, ctx::cfg->item_section.videocard_type.VideoCard_VideoConnectors, ctx::cfg->item_section.videocard_type.VideoCard_HDMIVersion, ctx::cfg->item_section.videocard_type.VideoCard_MaxResolution, ctx::cfg->item_section.videocard_type.VideoCard_CountMonitors, ctx::cfg->item_section.videocard_type.VideoCard_ConnectionInterface, ctx::cfg->item_section.videocard_type.VideoCard_PCIExpressVersion );
                }
                else if ( ctx::cfg->global.currect_type_item == 3 )
                {
                    sql_request_type_item = std::format( R"(INSERT INTO "items" ("Name","ImageId","TypeId","SectionsId","Price","Guarantee","RAM_MemoryType","RAM_FormFactor","RAM_MemoryCapacity","RAM_ClockFrequency") VALUES ('{}',{},{},{},'{}','{}', '{}','{}','{}','{}');)", ctx::cfg->item_section.name, id_image, ctx::cfg->global.currect_type_item + 1, ctx::cfg->section_info[ select_id_category ].id, ctx::cfg->item_section.price, ctx::cfg->item_section.guarantee, ctx::cfg->item_section.ram_type.RAM_MemoryType, ctx::cfg->item_section.ram_type.RAM_FormFactor, ctx::cfg->item_section.ram_type.RAM_MemoryCapacity, ctx::cfg->item_section.ram_type.RAM_ClockFrequency );
                }
                else if ( ctx::cfg->global.currect_type_item == 4 )
                {
                    sql_request_type_item = std::format( R"(INSERT INTO "items" ("Name","ImageId","TypeId","SectionsId","Price","Guarantee","PowerSupplyUnit_FormFactor","PowerSupplyUnit_Power","PowerSupplyUnit_DetachableCables","PowerSupplyUnit_PCIEPowerSupply","PowerSupplyUnit_Thickness","PowerSupplyUnit_Height") VALUES ('{}',{},{},{},'{}','{}', '{}','{}','{}','{}','{}','{}');)", ctx::cfg->item_section.name, id_image, ctx::cfg->global.currect_type_item + 1, ctx::cfg->section_info[ select_id_category ].id, ctx::cfg->item_section.price, ctx::cfg->item_section.guarantee, ctx::cfg->item_section.power_supply_type.PowerSupplyUnit_FormFactor, ctx::cfg->item_section.power_supply_type.PowerSupplyUnit_Power, ctx::cfg->item_section.power_supply_type.PowerSupplyUnit_DetachableCables, ctx::cfg->item_section.power_supply_type.PowerSupplyUnit_PCIEPowerSupply, ctx::cfg->item_section.power_supply_type.PowerSupplyUnit_Thickness, ctx::cfg->item_section.power_supply_type.PowerSupplyUnit_Height );
                }
                else if ( ctx::cfg->global.currect_type_item == 5 )
                {
                    sql_request_type_item = std::format( R"(INSERT INTO "items" ("Name","ImageId","TypeId","SectionsId","Price","Guarantee","Body_FormFactor","Body_OrientationMatBoards","Body_Length","Body_Thickness","Body_Height","Body_FormFactorMatBoards","Body_BPFormFactor","Body_BP_Placement","Body_LocationIOPanel","Body_DustFilter","Body_SideFans","Body_LowerFans","Body_UpperFans","Body_FrontFans","Body_FansIncluded") VALUES ('{}',{},{},{},'{}','{}', '{}','{}','{}','{}','{}','{}','{}','{}','{}','{}','{}','{}','{}','{}','{}');)", ctx::cfg->item_section.name, id_image, ctx::cfg->global.currect_type_item + 1, ctx::cfg->section_info[ select_id_category ].id, ctx::cfg->item_section.price, ctx::cfg->item_section.guarantee, ctx::cfg->item_section.body_type.Body_FormFactor, ctx::cfg->item_section.body_type.Body_OrientationMatBoards, ctx::cfg->item_section.body_type.Body_Length, ctx::cfg->item_section.body_type.Body_Thickness, ctx::cfg->item_section.body_type.Body_Height, ctx::cfg->item_section.body_type.Body_FormFactorMatBoards, ctx::cfg->item_section.body_type.Body_BPFormFactor, ctx::cfg->item_section.body_type.Body_BP_Placement, ctx::cfg->item_section.body_type.Body_LocationIOPanel, ctx::cfg->item_section.body_type.Body_DustFilter, ctx::cfg->item_section.body_type.Body_SideFans, ctx::cfg->item_section.body_type.Body_LowerFans, ctx::cfg->item_section.body_type.Body_UpperFans, ctx::cfg->item_section.body_type.Body_FrontFans, ctx::cfg->item_section.body_type.Body_FansIncluded );
                }
                else if ( ctx::cfg->global.currect_type_item == 6 )
                {
                    sql_request_type_item = std::format( R"(INSERT INTO "items" ("Name","ImageId","TypeId","SectionsId","Price","Guarantee","SSD_MemoryCapacity","SSD_PhysInterface","SSD_MaxSpeedRecordings","SSD_MaxSpeedReadings","SSD_SpeedProductionRecords","SSD_SpeedProductionReadings") VALUES ('{}',{},{},{},'{}','{}', '{}','{}','{}','{}','{}','{}');)", ctx::cfg->item_section.name, id_image, ctx::cfg->global.currect_type_item + 1, ctx::cfg->section_info[ select_id_category ].id, ctx::cfg->item_section.price, ctx::cfg->item_section.guarantee, ctx::cfg->item_section.ssd_type.SSD_MemoryCapacity, ctx::cfg->item_section.ssd_type.SSD_PhysInterface, ctx::cfg->item_section.ssd_type.SSD_MaxSpeedRecordings, ctx::cfg->item_section.ssd_type.SSD_MaxSpeedReadings, ctx::cfg->item_section.ssd_type.SSD_SpeedProductionRecords, ctx::cfg->item_section.ssd_type.SSD_SpeedProductionReadings );
                }
                else if ( ctx::cfg->global.currect_type_item == 7 )
                {

                    sql_request_type_item = std::format( R"(INSERT INTO "items" ("Name","ImageId","TypeId","SectionsId","Price","Guarantee","HardDrives_MemoryCapacity","HardDrives_PhysInterface","HardDrives_Bandwidth","HardDrives_AverageDelayTime","HardDrives_MaxDataTransferRate") VALUES ('{}',{},{},{},'{}','{}', '{}','{}','{}','{}','{}');)", ctx::cfg->item_section.name, id_image, ctx::cfg->global.currect_type_item + 1, ctx::cfg->section_info[ select_id_category ].id, ctx::cfg->item_section.price, ctx::cfg->item_section.guarantee, ctx::cfg->item_section.hard_drivers_type.HardDrives_MemoryCapacity, ctx::cfg->item_section.hard_drivers_type.HardDrives_PhysInterface, ctx::cfg->item_section.hard_drivers_type.HardDrives_Bandwidth, ctx::cfg->item_section.hard_drivers_type.HardDrives_AverageDelayTime, ctx::cfg->item_section.hard_drivers_type.HardDrives_MaxDataTransferRate );
                }
                else if ( ctx::cfg->global.currect_type_item == 8 )
                {

                    sql_request_type_item = std::format( R"(INSERT INTO "items" ("Name","ImageId","TypeId","SectionsId","Price","Guarantee","ComputerCooling_Socket","ComputerCooling_Size","ComputerCooling_ConnectorForConnection","ComputerCooling_MaxRotationSpeed","ComputerCooling_MinRotationSpeed","ComputerCooling_SpeedAdjustment","ComputerCooling_MaxNoiseLevel") VALUES ('{}',{},{},{},'{}','{}', '{}','{}','{}','{}','{}','{}','{}');)", ctx::cfg->item_section.name, id_image, ctx::cfg->global.currect_type_item + 1, ctx::cfg->section_info[ select_id_category ].id, ctx::cfg->item_section.price, ctx::cfg->item_section.guarantee, ctx::cfg->item_section.computer_cooling_type.ComputerCooling_Socket, ctx::cfg->item_section.computer_cooling_type.ComputerCooling_Size, ctx::cfg->item_section.computer_cooling_type.ComputerCooling_ConnectorForConnection, ctx::cfg->item_section.computer_cooling_type.ComputerCooling_MaxRotationSpeed, ctx::cfg->item_section.computer_cooling_type.ComputerCooling_MinRotationSpeed, ctx::cfg->item_section.computer_cooling_type.ComputerCooling_SpeedAdjustment, ctx::cfg->item_section.computer_cooling_type.ComputerCooling_MaxNoiseLevel );
                }


                database::add_item_in_currect_section( sql_request_type_item );

                sql_request_type_item.clear();

                database::get_item_in_currect_section( ctx::cfg->section_info[ select_id_category ].id );

                ctx::cfg->global.subtab_sections = ctx::subtab_sections_id::open_currect_section;
            }

            ImGui::PopStyleColor();


            //ImGui::PushStyleColor( ImGuiCol_ChildBg, ImVec4( 1, 0, 0, 1 ) );
            ImGui::SetCursorPos( { 300.0f,0 } );
            ImGui::BeginChild( "##info_item1", ImVec2( 280.0, 390.0f ) );
            {
                ImGui::SetCursorPos( { 15.0f,10.0f } );
                ImGui::BeginGroup();
                {
                    ImGui::PushItemWidth( 250.0f );

                    ImGui::PushStyleVar( ImGuiStyleVar_FramePadding, ImVec2( 24.0f, 8.0f ) );
                    ImGui::PushStyleVar( ImGuiStyleVar_FrameBorderSize, 1.5f );
                    ImGui::PushStyleVar( ImGuiStyleVar_FrameRounding, 3.0f );

                    //all input item
                    ImGui::Text( reinterpret_cast< const char* >( u8"Наименование товара" ) );
                    ImGui::InputText( "##name_item", &ctx::cfg->item_section.name);

                    ImGui::Text( reinterpret_cast< const char* >( u8"Цена" ) );
                    ImGui::InputText( "##price_item", &ctx::cfg->item_section.price );

                    ImGui::Text( reinterpret_cast< const char* >( u8"Гарантия" ) );
                    ImGui::InputText( "##guarantee_item", &ctx::cfg->item_section.guarantee );

                    if ( ctx::cfg->global.currect_type_item == 0 ) //лень делать рендер в цикле
                    {
                        int i = 0;
                        for ( auto processor_get : ctx::cfg->processor_render )
                        {
                            ImGui::Text( processor_get.first );
                            ImGui::PushID( i );
                            ImGui::InputText( "##name_p", processor_get.second );
                            ImGui::PopID();

                            i++;
                        }
                       
                    }
                    else if ( ctx::cfg->global.currect_type_item == 1 )
                    {

                        int i = 0;
                        for ( auto motherboard_get : ctx::cfg->motherboard_render )
                        {
                            ImGui::Text( motherboard_get.first );
                            ImGui::PushID( i );
                            ImGui::InputText( "##name_m", motherboard_get.second );
                            ImGui::PopID();

                            i++;
                        }
                    }
                    else if ( ctx::cfg->global.currect_type_item == 2 )
                    {

                        int i = 0;
                        for ( auto video_get : ctx::cfg->videocard_render )
                        {
                            ImGui::Text( video_get.first );
                            ImGui::PushID( i );
                            ImGui::InputText( "##name_v", video_get.second );
                            ImGui::PopID();

                            i++;
                        }

                    }
                    else if ( ctx::cfg->global.currect_type_item == 3 )
                    {
                            int i = 0;
                            for ( auto ram_get : ctx::cfg->ram_render)
                            {
                                ImGui::Text( ram_get.first );
                                ImGui::PushID( i );
                                ImGui::InputText("##name", ram_get.second);
                                ImGui::PopID();

                                i++;
                            }
                    }
                    else if ( ctx::cfg->global.currect_type_item == 4 )
                    {
                        int i = 0;
                        for ( auto power_get : ctx::cfg->power_supply_render )
                        {
                            ImGui::Text( power_get.first );
                            ImGui::PushID( i );
                            ImGui::InputText( "##name_p", power_get.second );
                            ImGui::PopID();

                            i++;
                        }
                    }
                    else if ( ctx::cfg->global.currect_type_item == 5 )
                    {
                        int i = 0;
                        for ( auto body_get : ctx::cfg->body_render )
                        {
                            ImGui::Text( body_get.first );
                            ImGui::PushID( i );
                            ImGui::InputText( "##name_b", body_get.second );
                            ImGui::PopID();

                            i++;
                        }
                    }
                    else if ( ctx::cfg->global.currect_type_item == 6 )
                    {
                        int i = 0;
                        for ( auto ssd_get : ctx::cfg->ssd_render )
                        {
                            ImGui::Text( ssd_get.first );
                            ImGui::PushID( i );
                            ImGui::InputText( "##name_s", ssd_get.second );
                            ImGui::PopID();

                            i++;
                        }
                    }
                    else if ( ctx::cfg->global.currect_type_item == 7 )
                    {
                        int i = 0;
                        for ( auto hard_drivers_get : ctx::cfg->hard_drivers_render )
                        {
                            ImGui::Text( hard_drivers_get.first );
                            ImGui::PushID( i );
                            ImGui::InputText( "##name_h", hard_drivers_get.second );
                            ImGui::PopID();

                            i++;
                        }
                    }
                    else if ( ctx::cfg->global.currect_type_item == 8 )
                    {
                        int i = 0;
                        for ( auto computer_cooling_get : ctx::cfg->computer_cooling_render )
                        {
                            ImGui::Text( computer_cooling_get.first );
                            ImGui::PushID( i );
                            ImGui::InputText( "##name_cc", computer_cooling_get.second );
                            ImGui::PopID();

                            i++;
                        }
                    }

                    ImGui::PopStyleVar( 3 );
                    ImGui::PopItemWidth();
                }
                ImGui::EndGroup();

            }
            ImGui::EndChild();
        }
        else if ( ctx::cfg->global.subtab_sections == ctx::subtab_sections_id::open_currect_item_in_section )
        {

            ImColor quality_color = ImColor( 226, 76, 84 );

            switch ( 1 )
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

            ImGui::GetWindowDrawList()->AddRectFilled( ImGui::GetWindowPos() + ImVec2( 10.0f, 0.0f ),
                ImGui::GetWindowPos() + ImVec2( 280.0f, 248.0f ), IM_COL32( 28, 30, 33, 255 ), 5.0f, ImDrawCornerFlags_Top | ImDrawCornerFlags_Bot );


            if ( ctx::cfg->item_section_info[ select_item_in_category ].picture )
            {
                ImGui::SetCursorPos( { 25.0f,44.0f } );
                ImGui::Image( ctx::cfg->item_section_info[ select_item_in_category ].picture, { 230.0f,160.0f } );
            }


            ImGui::PushStyleVar( ImGuiStyleVar_FrameRounding, 3.0f );

            if ( ctx::cfg->user_settings.admin_panel )
            {
                ImGui::SetCursorPos( { 10.0f, 255.0f } );
                if ( ImGui::Button( reinterpret_cast< const char* >( u8"Выбрать картинку" ), ImVec2( 270.0f, 25.0f ) ) )
                {
                    memset( filename, 0, MAX_PATH );

                    OPENFILENAMEA ofn;
                    ZeroMemory( &filename, sizeof( filename ) );
                    ZeroMemory( &ofn, sizeof( ofn ) );
                    ofn.lStructSize = sizeof( ofn );
                    ofn.hwndOwner = NULL;
                    ofn.lpstrFilter = "Png Files\0*.png\0Any File\0*.*\0";
                    ofn.lpstrFile = filename;
                    ofn.nMaxFile = MAX_PATH;
                    ofn.lpstrTitle = "Select Image file";
                    ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

                    if ( GetOpenFileNameA( &ofn ) )
                    {
                        std::cout << "You chose the file \"" << filename << "\"\n";
                    }
                    else
                    {
                        // All this stuff below is to tell you exactly how you messed up above. 
                        // Once you've got that fixed, you can often (not always!) reduce it to a 'user cancelled' assumption.
                        switch ( CommDlgExtendedError() )
                        {
                        case CDERR_DIALOGFAILURE: std::cout << "CDERR_DIALOGFAILURE\n";   break;
                        case CDERR_FINDRESFAILURE: std::cout << "CDERR_FINDRESFAILURE\n";  break;
                        case CDERR_INITIALIZATION: std::cout << "CDERR_INITIALIZATION\n";  break;
                        case CDERR_LOADRESFAILURE: std::cout << "CDERR_LOADRESFAILURE\n";  break;
                        case CDERR_LOADSTRFAILURE: std::cout << "CDERR_LOADSTRFAILURE\n";  break;
                        case CDERR_LOCKRESFAILURE: std::cout << "CDERR_LOCKRESFAILURE\n";  break;
                        case CDERR_MEMALLOCFAILURE: std::cout << "CDERR_MEMALLOCFAILURE\n"; break;
                        case CDERR_MEMLOCKFAILURE: std::cout << "CDERR_MEMLOCKFAILURE\n";  break;
                        case CDERR_NOHINSTANCE: std::cout << "CDERR_NOHINSTANCE\n";     break;
                        case CDERR_NOHOOK: std::cout << "CDERR_NOHOOK\n";          break;
                        case CDERR_NOTEMPLATE: std::cout << "CDERR_NOTEMPLATE\n";      break;
                        case CDERR_STRUCTSIZE: std::cout << "CDERR_STRUCTSIZE\n";      break;
                        case FNERR_BUFFERTOOSMALL: std::cout << "FNERR_BUFFERTOOSMALL\n";  break;
                        case FNERR_INVALIDFILENAME: std::cout << "FNERR_INVALIDFILENAME\n"; break;
                        case FNERR_SUBCLASSFAILURE: std::cout << "FNERR_SUBCLASSFAILURE\n"; break;
                        default: std::cout << "You cancelled.\n";
                        }
                    }

                    if ( filename[ 0 ] != '\0' )
                    {
                        std::vector<uint8_t> raw_image = { 0 };
                        helpers::read_file_to_memory( filename, &raw_image );

                        std::string str( raw_image.begin(), raw_image.end() );

                        D3DXCreateTextureFromFileInMemoryEx( render::g_pd3dDevice, ( LPVOID )str.c_str(), str.size(), 173, 143, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &ctx::cfg->item_section_info[ select_item_in_category ].picture );

                        raw_image.clear();
                        str.clear();
                    }

                }
            }

            int get_id = ctx::cfg->item_section_info[ select_item_in_category ].type_item_id - 1;
     
            ImGui::PushItemWidth( 270.0f );
            ImGui::SetCursorPos( { 10.0f, ctx::cfg->user_settings.admin_panel ? 285.0f : 255.0f } );

            if ( ctx::cfg->user_settings.admin_panel )
            {
                ImGui::Combo( "##type_item_select_item", &get_id, ctx::type_item, IM_ARRAYSIZE( ctx::type_item ) );
            }
            else
            {
                ImGui::BeginGroup();
                {
                    ImGui::Text( reinterpret_cast< const char* >( u8"Количество" ) );
                    ImGui::InputInt( "##count_add", &ctx::cfg->global.count_add_items );
                }
                ImGui::EndGroup();
            }
           
            ImGui::PopItemWidth();

            ImGui::PushStyleColor( ImGuiCol_Button, ImVec4( 0.906f, 0.133f, 0.259f, 1.000f ) );

            ImGui::SetCursorPos( { 10.0f, 310.0f } );
            if ( ImGui::Button( ctx::cfg->user_settings.admin_panel ? reinterpret_cast< const char* >( u8"Сохранить изменения" ) :reinterpret_cast< const char* >( u8"Добавить в корзину" ), ImVec2( 270.0f, 35.0f ) ) )
            {
                if ( ctx::cfg->user_settings.admin_panel )
                {
                    int image_id = ctx::cfg->item_section_info[ select_item_in_category ].image_id;

                    if ( filename[ 0 ] != '\0' )
                    {
                        std::vector<uint8_t> raw_image = { 0 };
                        helpers::read_file_to_memory( filename, &raw_image );

                        std::string str( raw_image.begin(), raw_image.end() );
                        auto out = helpers::b64encode( str );

                        str.clear();
                        auto id_image = database::add_image( std::format( R"(INSERT INTO "images" ("ImageFile") VALUES ('{}'); SELECT max("Id") FROM "images";)", out ) );


                        image_id = atoi(id_image.data());

                        out.clear();
                        raw_image.clear();
                    }


                    std::cout << "ImageId:" << image_id << std::endl << "TypeId:" << get_id << std::endl << "IdItem:" << ctx::cfg->item_section_info[ select_item_in_category ].id << std::endl;

                    auto sql_request_item_edit = helpers::generate_update_item( image_id, get_id, select_item_in_category );

                    database::add_item_in_currect_section( sql_request_item_edit );


                    database::get_item_in_currect_section( ctx::cfg->section_info[select_id_category].id );

                    ctx::cfg->global.subtab_sections = ctx::subtab_sections_id::open_currect_section;

                    sql_request_item_edit.clear();

                }
                else
                {
                    const auto finaly_index = ctx::cfg->global.last_index_bakset == 0 ? ctx::cfg->global.count_add_items + ctx::cfg->global.last_index_bakset : ctx::cfg->global.count_add_items + ctx::cfg->global.last_index_bakset + 1;

                    const auto get_i = ctx::cfg->global.last_index_bakset == 0 ? 0 : ctx::cfg->global.last_index_bakset + 1;
                    for ( size_t i = get_i; i < finaly_index; i++ )
                    {
                        auto get_item = ctx::cfg_t::items_in_basket_t();
                        get_item.name = ctx::cfg->item_section_info[ select_item_in_category ].name;
                        get_item.item_id = ctx::cfg->item_section_info[ select_item_in_category ].id;
                        get_item.price = atoi( ctx::cfg->item_section_info[ select_item_in_category ].price.c_str() );
                        get_item.picture = ctx::cfg->item_section_info[ select_item_in_category ].picture;

                        ctx::cfg->basket_info[ i ] = std::move( get_item );

                        ctx::cfg->global.last_index_bakset = i;

                        std::cout << finaly_index << std::endl;


                    }

                    ctx::cfg->global.subtab_sections = ctx::subtab_sections_id::open_currect_section;
                }
            }

            ImGui::PopStyleColor();

            if ( ctx::cfg->user_settings.admin_panel )
            {
                ImGui::SetCursorPos( { 10.0f, 355.0f } );
                if ( ImGui::Button( reinterpret_cast< const char* >( u8"Удалить товар" ), ImVec2( 270.0f, 35.0f ) ) )
                {

                    database::delete_item_in_currect_section( ctx::cfg->item_section_info[ select_item_in_category ].id, ctx::cfg->section_info[select_id_category].id );
                    ctx::cfg->global.subtab_sections = ctx::subtab_sections_id::open_currect_section;
                }
            }

            ImGui::PopStyleVar();

            //ImGui::PushStyleColor( ImGuiCol_ChildBg, ImVec4( 1, 0, 0, 1 ) );
            ImGui::SetCursorPos( { 300.0f,0 } );
            ImGui::BeginChild( "##info_item_open", ImVec2( 280.0, 390.0f ) );
            {
                ImGui::SetCursorPos( { 15.0f,10.0f } );
                ImGui::BeginGroup();
                {
                    ImGui::PushItemWidth( 250.0f );

                    ImGui::PushStyleVar( ImGuiStyleVar_FramePadding, ImVec2( 24.0f, 8.0f ) );
                    ImGui::PushStyleVar( ImGuiStyleVar_FrameBorderSize, 1.5f );
                    ImGui::PushStyleVar( ImGuiStyleVar_FrameRounding, 3.0f );

                    //all input item
                    ImGui::Text( reinterpret_cast< const char* >( u8"Наименование товара" ) );
                    ImGui::InputText( "##name_item", &ctx::cfg->item_section_info[ select_item_in_category ].name, ImGuiInputTextFlags_ReadOnly );

                    ImGui::Text( reinterpret_cast< const char* >( u8"Цена" ) );
                    ImGui::InputText( "##price_item", &ctx::cfg->item_section_info[ select_item_in_category ].price, ImGuiInputTextFlags_ReadOnly );

                    ImGui::Text( reinterpret_cast< const char* >( u8"Гарантия" ) );
                    ImGui::InputText( "##guarantee_item", &ctx::cfg->item_section_info[ select_item_in_category ].guarantee, ImGuiInputTextFlags_ReadOnly );

                    if ( ctx::cfg->item_section_info[ select_item_in_category ].type_item_id - 1 == 0 ) //лень делать рендер в цикле
                    {
                        int i = 0;
                        for ( auto processor_get : ctx::cfg->processor_render_edit )
                        {
                            ImGui::Text( processor_get.first );
                            ImGui::PushID( i );
                            ImGui::InputText( "##name_p", processor_get.second, ImGuiInputTextFlags_ReadOnly );
                            ImGui::PopID();

                            i++;
                        }

                    }
                    else if ( ctx::cfg->item_section_info[ select_item_in_category ].type_item_id - 1 == 1 )
                    {

                        int i = 0;
                        for ( auto motherboard_get : ctx::cfg->motherboard_render_edit )
                        {
                            ImGui::Text( motherboard_get.first );
                            ImGui::PushID( i );
                            ImGui::InputText( "##name_m", motherboard_get.second, ImGuiInputTextFlags_ReadOnly );
                            ImGui::PopID();

                            i++;
                        }
                    }
                    else if ( ctx::cfg->item_section_info[ select_item_in_category ].type_item_id - 1 == 2 )
                    {

                        int i = 0;
                        for ( auto video_get : ctx::cfg->videocard_render_edit )
                        {
                            ImGui::Text( video_get.first );
                            ImGui::PushID( i );
                            ImGui::InputText( "##name_v", video_get.second, ImGuiInputTextFlags_ReadOnly );
                            ImGui::PopID();

                            i++;
                        }

                    }
                    else if ( ctx::cfg->item_section_info[ select_item_in_category ].type_item_id - 1 == 3 )
                    {
                        int i = 0;
                        for ( auto ram_get : ctx::cfg->ram_render_edit )
                        {
                            ImGui::Text( ram_get.first );
                            ImGui::PushID( i );
                            ImGui::InputText( "##name", ram_get.second, ImGuiInputTextFlags_ReadOnly );
                            ImGui::PopID();

                            i++;
                        }
                    }
                    else if ( ctx::cfg->item_section_info[ select_item_in_category ].type_item_id - 1 == 4 )
                    {
                        int i = 0;
                        for ( auto power_get : ctx::cfg->power_supply_render_edit )
                        {
                            ImGui::Text( power_get.first );
                            ImGui::PushID( i );
                            ImGui::InputText( "##name_p", power_get.second, ImGuiInputTextFlags_ReadOnly );
                            ImGui::PopID();

                            i++;
                        }
                    }
                    else if ( ctx::cfg->item_section_info[ select_item_in_category ].type_item_id - 1 == 5 )
                    {
                        int i = 0;
                        for ( auto body_get : ctx::cfg->body_render_edit )
                        {
                            ImGui::Text( body_get.first );
                            ImGui::PushID( i );
                            ImGui::InputText( "##name_b", body_get.second, ImGuiInputTextFlags_ReadOnly );
                            ImGui::PopID();

                            i++;
                        }
                    }
                    else if ( ctx::cfg->item_section_info[ select_item_in_category ].type_item_id - 1 == 6 )
                    {
                        int i = 0;
                        for ( auto ssd_get : ctx::cfg->ssd_render_edit )
                        {
                            ImGui::Text( ssd_get.first );
                            ImGui::PushID( i );
                            ImGui::InputText( "##name_s", ssd_get.second, ImGuiInputTextFlags_ReadOnly );
                            ImGui::PopID();

                            i++;
                        }
                    }
                    else if ( ctx::cfg->item_section_info[ select_item_in_category ].type_item_id - 1 == 7 )
                    {
                        int i = 0;
                        for ( auto hard_drivers_get : ctx::cfg->hard_drivers_render_edit )
                        {
                            ImGui::Text( hard_drivers_get.first );
                            ImGui::PushID( i );
                            ImGui::InputText( "##name_h", hard_drivers_get.second, ImGuiInputTextFlags_ReadOnly );
                            ImGui::PopID();

                            i++;
                        }
                    }
                    else if ( ctx::cfg->item_section_info[ select_item_in_category ].type_item_id - 1 == 8 )
                    {
                        int i = 0;
                        for ( auto computer_cooling_get : ctx::cfg->computer_cooling_render_edit )
                        {
                            ImGui::Text( computer_cooling_get.first );
                            ImGui::PushID( i );
                            ImGui::InputText( "##name_cc", computer_cooling_get.second, ImGuiInputTextFlags_ReadOnly );
                            ImGui::PopID();

                            i++;
                        }
                    }

                    ImGui::PopStyleVar( 3 );
                    ImGui::PopItemWidth();
                }
                ImGui::EndGroup();

            }
            ImGui::EndChild();

        }
    }
    ImGui::EndChild();

    ImGui::PopStyleColor();
}