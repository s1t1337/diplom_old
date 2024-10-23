#include <iostream>
#include <fstream>
#include <Windows.h>
#include <pqxx/pqxx>

#include "database.hpp"
#include "../../context/cfg.hpp"
#include "../../dependencies/hash/sha1.hpp"
#include "../helpers/helpers.hpp"
#include "../render/render.hpp"

namespace database
{
	const char* connect_db = R"(host=127.0.0.1 port=5432 dbname=diplom user=postgres password=NewCordq1703)";

    std::string convert_text_to_utf8( std::string_view text )
    {
        int size = MultiByteToWideChar( CP_ACP, MB_COMPOSITE, text.data(),
            text.length(), nullptr, 0 );
        std::wstring utf16_str( size, '\0' );
        MultiByteToWideChar( CP_ACP, MB_COMPOSITE, text.data(),
            text.length(), &utf16_str[ 0 ], size );

        int utf8_size = WideCharToMultiByte( CP_UTF8, 0, utf16_str.c_str(),
            utf16_str.length(), nullptr, 0,
            nullptr, nullptr );
        std::string utf8_str( utf8_size, '\0' );
        WideCharToMultiByte( CP_UTF8, 0, utf16_str.c_str(),
            utf16_str.length(), &utf8_str[ 0 ], utf8_size,
            nullptr, nullptr );

        return utf8_str;
    }

    
    bool reg_program( std::string_view sql )
    {
        try
        {
            pqxx::connection db_program( connect_db );
            pqxx::work  xact( db_program, "SampleSelect" );

            try
            {

                pqxx::result res = xact.exec( std::format( R"(SELECT "Login" FROM "users" WHERE "Login" = '{}')", ctx::cfg->user_settings.login ) );

                for ( auto get_info : res )
                {
                    if ( ctx::cfg->user_settings.login == get_info[ 0 ].c_str() )
                    {
                        MessageBoxA( 0, "Данный логин занят", "Error geg", 0 );

                        res.clear();
                        db_program.close();
                        return false;
                    }
                }

                res.clear();
                res = xact.exec( sql );
                res.clear();

                res = xact.exec( std::format( R"(SELECT "Id" FROM "users" WHERE "Login" = '{}')", ctx::cfg->user_settings.login ) );
                xact.commit();

                for ( auto get_info : res )
                {
                    MessageBoxA( 0, std::to_string( get_info[ 0 ].as<int>() ).c_str(), "reg_program", 0 );
                    ctx::cfg->user_settings.id = get_info[ 0 ].as<int>();
                }
                res.clear();
                db_program.close();

                return true;
            }
            catch ( const pqxx::sql_error& error )
            {
                MessageBoxA( 0, convert_text_to_utf8( error.what()).c_str(), error.sqlstate().c_str(), 0 );

                return false;
            }
        }
        catch ( const pqxx::broken_connection& error )
        {
            MessageBoxA( 0, error.what(), "error bd", 0 );
            return false;
        }
    }

    bool  auth_program()
    {
        try
        {
            pqxx::connection db_program( connect_db );
            pqxx::work  xact( db_program, "SampleSelect" );

            try
            {

                pqxx::result res = xact.exec( std::format( R"(SELECT * FROM "users" WHERE "Login" = '{}')", ctx::cfg->user_settings.login ) );
                xact.commit();
                if ( !res.size() )
                {
                    MessageBoxA( 0, "Данный пользователь не найден", "Error Auth", 0 );

                    res.clear();
                    db_program.close();

                    return false;
                }

                for ( auto get_info : res )
                {

                    if ( sha1( ctx::cfg->user_settings.password ) != get_info[ 2 ].c_str() )
                    {

                        MessageBoxA( 0, "Неверный пароль", "Error Auth", 0 );

                        res.clear();
                        db_program.close();
                        return false;


                    }

                    if ( get_info[ 7 ].as<bool>() == true )
                    {

                        ctx::cfg->user_settings.is_admin = true;
                    }

                    ctx::cfg->user_settings.FIO = get_info[ 3 ].c_str();
                    ctx::cfg->user_settings.number = get_info[ 4 ].c_str();
                    ctx::cfg->user_settings.email = get_info[ 5 ].c_str();
                    ctx::cfg->user_settings.date = get_info[ 6 ].c_str();
                    ctx::cfg->user_settings.id = get_info[ 0 ].as<int>();
                }

                res.clear();
                db_program.close();

                return true;
            }
            catch ( const pqxx::sql_error& error )
            {
                MessageBoxA( 0, convert_text_to_utf8( error.what() ).c_str(), error.sqlstate().c_str(), 0 );

                return false;
            }
        }
        catch ( const pqxx::broken_connection& error )
        {
            MessageBoxA( 0, error.what(), "error bd", 0 );

            return false;
        }
    }






    //users
    void get_all_users()
    {
        try
        {
            pqxx::connection db_program( connect_db );
            pqxx::work  xact( db_program, "SampleSelect" );

            try
            {

                ctx::cfg->users_info.clear();

                pqxx::result res = xact.exec( R"(SELECT * FROM "users")" );
                xact.commit();
                if ( !res.size() )
                {
                   // MessageBoxA( 0, std::to_string( res.size() ).c_str(), "Empty size", 0 );

                    res.clear();
                    db_program.close();
                    return;
                }

                int i = 0;
                for ( auto get_info : res )
                {

                    auto user = ctx::cfg_t::user_settings_t();

                    user.id = get_info[ 0 ].as<int>();
                    user.login = get_info[ 1 ].c_str();
                    user.password = get_info[ 2 ].c_str();
                    user.FIO = get_info[ 3 ].c_str();
                    user.number = get_info[ 4 ].c_str();
                    user.email = get_info[ 5 ].c_str();
                    user.date = get_info[ 6 ].c_str();
                    user.is_admin = get_info[ 7 ].as<bool>();

                    ctx::cfg->users_info[ i ] = std::move( user );

                    i++;

                }
                res.clear();
                db_program.close();
            }
            catch ( const pqxx::sql_error& error )
            {
                MessageBoxA( 0, error.query().c_str(), error.sqlstate().c_str(), 0 );
            }
        }
        catch ( const pqxx::broken_connection& error )
        {
            MessageBoxA( 0, error.what(), "error bd", 0 );
        }
    }

    void update_currect_user( std::string_view sql )
    {
        try
        {
            pqxx::connection db_program( connect_db );
            pqxx::work  xact( db_program, "SampleSelect" );

            try
            {

                pqxx::result res = xact.exec( sql );
                xact.commit();

                res.clear();
                db_program.close();

                get_all_users();
            }
            catch ( const pqxx::sql_error& error )
            {
                MessageBoxA( 0, error.query().c_str(), error.sqlstate().c_str(), 0 );
            }
        }
        catch ( const pqxx::broken_connection& error )
        {
            MessageBoxA( 0, error.what(), "error bd", 0 );
        }
    }

    void delete_currect_user( const int id )
    {
        try
        {
            pqxx::connection db_program( connect_db );
            pqxx::work  xact( db_program, "SampleSelect" );

            try
            {

                pqxx::result res = xact.exec( std::format( R"(DELETE FROM "users" WHERE "Id" = {})", id ) );
                xact.commit();

                res.clear();
                db_program.close();

                get_all_users();
            }
            catch ( const pqxx::sql_error& error )
            {
                MessageBoxA( 0, error.query().c_str(), error.sqlstate().c_str(), 0 );
            }
        }
        catch ( const pqxx::broken_connection& error )
        {
            MessageBoxA( 0, error.what(), "error bd", 0 );
        }
    }






    std::string_view add_image(std::string_view sql )
    {
        std::string_view id_image;
        try
        {
            pqxx::connection db_program( connect_db );
            pqxx::work  xact( db_program, "SampleSelect" );

            try
            {

                pqxx::result res = xact.exec( sql );
                xact.commit();

                for ( const auto get_info: res )
                {
                    id_image = get_info[ 0 ].c_str();
                }

                res.clear();
                db_program.close();

                return id_image;
            }
            catch ( const pqxx::sql_error& error )
            {
                MessageBoxA( 0, error.query().c_str(), error.sqlstate().c_str(), 0 );
            }
        }
        catch ( const pqxx::broken_connection& error )
        {
            MessageBoxA( 0, error.what(), "error bd", 0 );
        }

        return "0";
    }

    void get_image( const int image_id, std::string& out )
    {

        try
        {
            pqxx::connection db_program( connect_db );
            pqxx::work  xact( db_program, "SampleSelect" );

            try
            {

                pqxx::result res = xact.exec( std::format(R"(SELECT "ImageFile" FROM "images" WHERE "Id" = {})", image_id ));
                xact.commit();


                for ( auto get_info : res )
                {
                    out = get_info[ 0 ].c_str();
                }

                res.clear();
                db_program.close();
            }
            catch ( const pqxx::sql_error& error )
            {
                MessageBoxA( 0, error.query().c_str(), error.sqlstate().c_str(), 0 );
            }
        }
        catch ( const pqxx::broken_connection& error )
        {
            MessageBoxA( 0, error.what(), "error bd", 0 );
        }
    }






    void add_sections( std::string_view sql )
    {
        try
        {
            pqxx::connection db_program( connect_db );
            pqxx::work  xact( db_program, "SampleSelect" );

            try
            {

                pqxx::result res = xact.exec( sql );
                xact.commit();

              /*  for ( auto get_info : res )
                {
                    std::cout << get_info[ 0 ].c_str() << std::endl;
                }*/

                res.clear();
                db_program.close();

                get_sections();
            }
            catch ( const pqxx::sql_error& error )
            {
                MessageBoxA( 0, error.query().c_str(), error.sqlstate().c_str(), 0 );
            }
        }
        catch ( const pqxx::broken_connection& error )
        {
            MessageBoxA( 0, error.what(), "error bd", 0 );
        }
    }

    void delete_currect_sections( const int id )
    {
        try
        {
            pqxx::connection db_program( connect_db );
            pqxx::work  xact( db_program, "SampleSelect" );

            try
            {

                pqxx::result res = xact.exec( std::format( R"(DELETE FROM "sections" WHERE "Id" = {})", ctx::cfg->section_info[id].id ) );
                xact.commit();

                for ( auto get_info : res )
                {
                    std::cout << get_info[ 0 ].c_str() << std::endl;
                }

                res.clear();
                db_program.close();
            }
            catch ( const pqxx::sql_error& error )
            {
                MessageBoxA( 0, error.query().c_str(), error.sqlstate().c_str(), 0 );
            }
        }
        catch ( const pqxx::broken_connection& error )
        {
            MessageBoxA( 0, error.what(), "error bd", 0 );
        }
    }

    void get_sections()
    {
        std::string get_image_from_id;
        try
        {
            pqxx::connection db_program( connect_db );
            pqxx::work  xact( db_program, "SampleSelect" );

            try
            {

                for (auto clear_info : ctx::cfg->section_info )
                {
                    if( clear_info.second.picture )
                    clear_info.second.picture->Release();
                }

                ctx::cfg->section_info.clear();

                pqxx::result res = xact.exec( R"(SELECT * FROM "sections")" );
                xact.commit();
                if ( !res.size() )
                {
                   // MessageBoxA( 0, std::to_string( res.size() ).c_str(), "Empty size", 0 );
                    res.clear();
                    db_program.close();
                    return;
                }

                int i = 0;
                for ( auto get_info : res )
                {

                    auto get_section = ctx::cfg_t::section_t();

                    get_section.id = get_info[ 0 ].as<int>();
                    get_section.name = get_info[ 1 ].c_str();
                    get_section.image_id = get_info[ 2 ].as<int>();

                    database::get_image( get_info[ 2 ].as<int>(), get_image_from_id );

                    std::string out_buffer =  helpers::b64decode( get_image_from_id );

                    get_image_from_id.clear();

                    D3DXCreateTextureFromFileInMemoryEx( render::g_pd3dDevice, ( LPVOID )out_buffer.c_str(), out_buffer.size(), 173, 143, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &get_section.picture );

                    ctx::cfg->section_info[ i ] = std::move( get_section );

                    out_buffer.clear();

                    i++;
                }
     
                res.clear();
                db_program.close();
            }
            catch ( const pqxx::sql_error& error )
            {
                MessageBoxA( 0, error.query().c_str(), error.sqlstate().c_str(), 0 );
            }
        }
        catch ( const pqxx::broken_connection& error )
        {
            MessageBoxA( 0, error.what(), "error bd", 0 );
        }
    }






    void add_item_in_currect_section( std::string_view sql )
    {
        try
        {
            pqxx::connection db_program( connect_db );
            pqxx::work  xact( db_program, "SampleSelect" );

            try
            {

                pqxx::result res = xact.exec( sql );
                xact.commit();

                res.clear();
                db_program.close();
            }
            catch ( const pqxx::sql_error& error )
            {
                MessageBoxA( 0, error.query().c_str(), error.sqlstate().c_str(), 0 );
            }
        }
        catch ( const pqxx::broken_connection& error )
        {
            MessageBoxA( 0, error.what(), "error bd", 0 );
        }
    }

    void get_item_in_currect_section( const int id_section, bool update_info )
    {
        std::string get_image_from_id;

        try
        {
            pqxx::connection db_program( connect_db );
            pqxx::work  xact( db_program, "SampleSelect" );

            try
            {
                if ( update_info )
                {
                    for ( auto item_clear : ctx::cfg->item_section_info )
                    {
                        if ( item_clear.second.picture )
                            item_clear.second.picture->Release();
                    }
                }

                ctx::cfg->item_section_info.clear();

                pqxx::result res = xact.exec( std::format( R"(SELECT * FROM "items" WHERE "SectionsId" = {})", id_section ) );
                xact.commit();
                if ( !res.size() )
                {
                    res.clear();
                    db_program.close();
                    return;
                }

                int i = 0;
                for ( auto get_info : res )
                {

                    auto item = ctx::cfg_t::item_in_section_t();

                    item.id = get_info[ 0 ].as<int>();
                    item.name = get_info[ 1 ].c_str();
                    item.image_id = get_info[ 2 ].as<int>();
                    item.type_item_id = get_info[ 3 ].as<int>();
                    item.section_id = get_info[ 4 ].as<int>();
                    item.price = get_info[ 5 ].c_str();
                    item.guarantee = get_info[ 6 ].c_str();

                    item.processor_type.Processor_ProcessorGeneration = get_info[ 7 ].c_str();
                    item.processor_type.Processor_Socket = get_info[ 8 ].c_str();
                    item.processor_type.Processor_NumberOfCores = get_info[ 9 ].c_str();
                    item.processor_type.Processor_MaxNumberOfThreads = get_info[ 10 ].c_str();
                    item.processor_type.Processor_Core = get_info[ 11 ].c_str();
                    item.processor_type.Processor_CacheL1Instructions = get_info[ 12 ].c_str();
                    item.processor_type.Processor_CacheL1Data = get_info[ 13 ].c_str();
                    item.processor_type.Processor_L2CacheSize = get_info[ 14 ].c_str();
                    item.processor_type.Processor_L3CacheSize = get_info[ 15 ].c_str();
                    item.processor_type.Processor_BaseFrequency = get_info[ 16 ].c_str();
                    item.processor_type.Processor_MaxFrequencyTurbo = get_info[ 17 ].c_str();
                    item.processor_type.Processor_MemoryType = get_info[ 18 ].c_str();

                    item.motherboard_type.MotherBoard_FormFactor = get_info[ 19 ].c_str();
                    item.motherboard_type.MotherBoard_Height = get_info[ 20 ].c_str();
                    item.motherboard_type.MotherBoard_Thickness = get_info[ 21 ].c_str();
                    item.motherboard_type.MotherBoard_Socket = get_info[ 22 ].c_str();
                    item.motherboard_type.MotherBoard_Chipset = get_info[ 23 ].c_str();
                    item.motherboard_type.MotherBoard_CountRAMSlots = get_info[ 24 ].c_str();
                    item.motherboard_type.MotherBoard_MemoryFormFactor = get_info[ 25 ].c_str();
                    item.motherboard_type.MotherBoard_MemoryType = get_info[ 26 ].c_str();
                    item.motherboard_type.MotherBoard_CountMemoryChannels = get_info[ 27 ].c_str();
                    item.motherboard_type.MotherBoard_MaxMemoryCapacity = get_info[ 28 ].c_str();
                    item.motherboard_type.MotherBoard_MaxMemoryFrequency = get_info[ 29 ].c_str();
                    item.motherboard_type.MotherBoard_RAMFrequency = get_info[ 30 ].c_str();
                    item.motherboard_type.MotherBoard_CountConnectorsM2 = get_info[ 31 ].c_str();
                    item.motherboard_type.MotherBoard_ConnectorsM2 = get_info[ 32 ].c_str();
                    item.motherboard_type.MotherBoard_CountSATAPorts = get_info[ 33 ].c_str();
                    item.motherboard_type.MotherBoard_OtherDriveConnectors = get_info[ 34 ].c_str();

                    item.videocard_type.VideoCard_FormFactor = get_info[ 35 ].c_str();
                    item.videocard_type.VideoCard_Length = get_info[ 36 ].c_str();
                    item.videocard_type.VideoCard_Thickness = get_info[ 37 ].c_str();
                    item.videocard_type.VideoCard_MemoryCapacity = get_info[ 38 ].c_str();
                    item.videocard_type.VideoCard_MemoryType = get_info[ 39 ].c_str();
                    item.videocard_type.VideoCard_VideoConnectors = get_info[ 40 ].c_str();
                    item.videocard_type.VideoCard_HDMIVersion = get_info[ 41 ].c_str();
                    item.videocard_type.VideoCard_MaxResolution = get_info[ 42 ].c_str();
                    item.videocard_type.VideoCard_CountMonitors = get_info[ 43 ].c_str();
                    item.videocard_type.VideoCard_ConnectionInterface = get_info[ 44 ].c_str();
                    item.videocard_type.VideoCard_PCIExpressVersion = get_info[ 45 ].c_str();

                    item.ram_type.RAM_MemoryType = get_info[ 46 ].c_str();
                    item.ram_type.RAM_FormFactor = get_info[ 47 ].c_str();
                    item.ram_type.RAM_MemoryCapacity = get_info[ 48 ].c_str();
                    item.ram_type.RAM_ClockFrequency = get_info[ 49 ].c_str();

                    item.power_supply_type.PowerSupplyUnit_FormFactor = get_info[ 50 ].c_str();
                    item.power_supply_type.PowerSupplyUnit_Power = get_info[ 51 ].c_str();
                    item.power_supply_type.PowerSupplyUnit_DetachableCables = get_info[ 52 ].c_str();
                    item.power_supply_type.PowerSupplyUnit_PCIEPowerSupply = get_info[ 53 ].c_str();
                    item.power_supply_type.PowerSupplyUnit_Thickness = get_info[ 54 ].c_str();
                    item.power_supply_type.PowerSupplyUnit_Height = get_info[ 55 ].c_str();

                    item.body_type.Body_FormFactor = get_info[ 56 ].c_str();
                    item.body_type.Body_OrientationMatBoards = get_info[ 57 ].c_str();
                    item.body_type.Body_Length = get_info[ 58 ].c_str();
                    item.body_type.Body_Thickness = get_info[ 59 ].c_str();
                    item.body_type.Body_Height = get_info[ 60 ].c_str();
                    item.body_type.Body_FormFactorMatBoards = get_info[ 61 ].c_str();
                    item.body_type.Body_BPFormFactor = get_info[ 62 ].c_str();
                    item.body_type.Body_BP_Placement = get_info[ 63 ].c_str();
                    item.body_type.Body_LocationIOPanel = get_info[ 64 ].c_str();
                    item.body_type.Body_DustFilter = get_info[ 65 ].c_str();
                    item.body_type.Body_SideFans = get_info[ 66 ].c_str();
                    item.body_type.Body_LowerFans = get_info[ 67 ].c_str();
                    item.body_type.Body_UpperFans = get_info[ 68 ].c_str();
                    item.body_type.Body_FrontFans = get_info[ 69 ].c_str();
                    item.body_type.Body_FansIncluded = get_info[ 70 ].c_str();

                    item.ssd_type.SSD_MemoryCapacity = get_info[ 71 ].c_str();
                    item.ssd_type.SSD_PhysInterface = get_info[ 72 ].c_str();
                    item.ssd_type.SSD_MaxSpeedRecordings = get_info[ 73 ].c_str();
                    item.ssd_type.SSD_MaxSpeedReadings = get_info[ 74 ].c_str();
                    item.ssd_type.SSD_SpeedProductionRecords = get_info[ 75 ].c_str();
                    item.ssd_type.SSD_SpeedProductionReadings = get_info[ 76 ].c_str();

                    item.hard_drivers_type.HardDrives_MemoryCapacity = get_info[ 77 ].c_str();
                    item.hard_drivers_type.HardDrives_PhysInterface = get_info[ 78 ].c_str();
                    item.hard_drivers_type.HardDrives_Bandwidth = get_info[ 79 ].c_str();
                    item.hard_drivers_type.HardDrives_AverageDelayTime = get_info[ 80 ].c_str();
                    item.hard_drivers_type.HardDrives_MaxDataTransferRate = get_info[ 81 ].c_str();


                    item.computer_cooling_type.ComputerCooling_Socket = get_info[ 82 ].c_str();
                    item.computer_cooling_type.ComputerCooling_Size = get_info[ 83 ].c_str();
                    item.computer_cooling_type.ComputerCooling_ConnectorForConnection = get_info[ 84 ].c_str();
                    item.computer_cooling_type.ComputerCooling_MaxRotationSpeed = get_info[ 85 ].c_str();
                    item.computer_cooling_type.ComputerCooling_MinRotationSpeed = get_info[ 86 ].c_str();
                    item.computer_cooling_type.ComputerCooling_SpeedAdjustment = get_info[ 87 ].c_str();
                    item.computer_cooling_type.ComputerCooling_MaxNoiseLevel = get_info[ 88 ].c_str();




                    database::get_image( get_info[ 2 ].as<int>(), get_image_from_id );

                    std::string out_buffer = helpers::b64decode( get_image_from_id );

                    get_image_from_id.clear();

                    D3DXCreateTextureFromFileInMemoryEx( render::g_pd3dDevice, ( LPVOID )out_buffer.c_str(), out_buffer.size(), 173, 143, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &item.picture );

                    ctx::cfg->item_section_info[ i ] = std::move( item );

                    out_buffer.clear();

                    i++;

                }
                res.clear();
                db_program.close();
            }
            catch ( const pqxx::sql_error& error )
            {
                MessageBoxA( 0, error.query().c_str(), error.sqlstate().c_str(), 0 );
            }
        }
        catch ( const pqxx::broken_connection& error )
        {
            MessageBoxA( 0, error.what(), "error bd", 0 );
        }
    }

    void delete_item_in_currect_section( const int id_item, const int id_section )
    {
        try
        {
            pqxx::connection db_program( connect_db );
            pqxx::work  xact( db_program, "SampleSelect" );

            try
            {

                pqxx::result res = xact.exec( std::format( R"(DELETE FROM "items" WHERE "Id" = {})", id_item ) );
                xact.commit();

                res.clear();
                db_program.close();

                get_item_in_currect_section( id_section );
            }
            catch ( const pqxx::sql_error& error )
            {
                MessageBoxA( 0, error.query().c_str(), error.sqlstate().c_str(), 0 );
            }
        }
        catch ( const pqxx::broken_connection& error )
        {
            MessageBoxA( 0, error.what(), "error bd", 0 );
        }
    }






    void add_basket_items( std::string_view sql )
    {
        try
        {
            pqxx::connection db_program( connect_db );
            pqxx::work  xact( db_program, "SampleSelect" );

            try
            {

                pqxx::result res = xact.exec( sql );
                xact.commit();



                res.clear();
                db_program.close();
            }
            catch ( const pqxx::sql_error& error )
            {
                MessageBoxA( 0, error.query().c_str(), error.sqlstate().c_str(), 0 );
            }
        }
        catch ( const pqxx::broken_connection& error )
        {
            MessageBoxA( 0, error.what(), "error bd", 0 );
        }
    }






    int add_order( std::string_view sql )
    {
        int index_id = 0;
        try
        {
            pqxx::connection db_program( connect_db );
            pqxx::work  xact( db_program, "SampleSelect" );

            try
            {

                pqxx::result res = xact.exec( sql );
                xact.commit();


                for ( auto get_info : res )
                {
                    index_id = get_info[ 0 ].as<int>();
                }


                res.clear();
                db_program.close();

                return index_id;
            }
            catch ( const pqxx::sql_error& error )
            {
                MessageBoxA( 0, error.query().c_str(), error.sqlstate().c_str(), 0 );
            }
        }
        catch ( const pqxx::broken_connection& error )
        {
            MessageBoxA( 0, error.what(), "error bd", 0 );
        }

        return -1;
    }

    void get_orders()
    {
        try
        {
            pqxx::connection db_program( connect_db );
            pqxx::work  xact( db_program, "SampleSelect" );

            try
            {

                ctx::cfg->orders_info.clear();

                //"SELECT * FROM "orders"
                pqxx::result res = xact.exec( R"(SELECT DISTINCT "orders"."Id","orders"."AllCount","orders"."AllPrice","orders"."IsReady","orders"."UserId","orders"."Time", "basket_items"."ItemsId" FROM "orders","basket_items" WHERE "basket_items"."OrdersId" = "orders"."Id")" );
                xact.commit();
                if ( !res.size() )
                {
                    //MessageBoxA( 0, std::to_string( res.size() ).c_str(), "Empty size", 0 );
                    res.clear();
                    db_program.close();

                    return;
                }

                int i = 0;
                for ( auto get_info : res )
                {

                    auto orders = ctx::cfg_t::orders_t();

                    orders.id = get_info[ 0 ].as<int>();
                    orders.all_count = get_info[ 1 ].as<int>();
                    orders.all_price = get_info[ 2 ].as<int>();
                    orders.is_ready = get_info[ 3 ].as<bool>();
                    orders.user_id = get_info[ 4 ].as<int>();
                    orders.time = get_info[ 5 ].c_str();

                    ctx::cfg->orders_info[ i ] = std::move( orders );

                    i++;

                }

                res.clear();
                db_program.close();
            }
            catch ( const pqxx::sql_error& error )
            {
                MessageBoxA( 0, error.query().c_str(), error.sqlstate().c_str(), 0 );
            }
        }
        catch ( const pqxx::broken_connection& error )
        {
            MessageBoxA( 0, error.what(), "error bd", 0 );
        }
    }



    void get_order_by_id( const int id )
    {
        try
        {
            pqxx::connection db_program( connect_db );
            pqxx::work  xact( db_program, "SampleSelect" );

            try
            {

                ctx::cfg->select_orders_info.clear();

                pqxx::result res = xact.exec( std::format( R"(SELECT "basket_items"."Id","basket_items"."Price","basket_items"."OrdersId","items"."Name","items"."TypeId" FROM "basket_items" INNER JOIN "items" ON "items"."Id" ="basket_items"."ItemsId" WHERE "OrdersId" = {})", ctx::cfg->orders_info[ id ].id ) );
                xact.commit();
                if ( !res.size() )
                {
                    //MessageBoxA( 0, std::to_string( res.size() ).c_str(), "Empty size", 0 );
                    res.clear();
                    db_program.close();
                    return;
                }

                int i = 0;
                for ( auto get_info : res )
                {

                    auto select_order = ctx::cfg_t::select_order_t();

                    select_order.id = get_info[ 0 ].as<int>();
                    select_order.price = get_info[ 1 ].as<int>();
                    select_order.orders_id = get_info[ 2 ].as<int>();               
                    select_order.name = get_info[ 3 ].c_str();
                    select_order.type_item_id = get_info[ 4 ].as<int>();


                    ctx::cfg->select_orders_info[ i ] = std::move( select_order );
                    i++;

                }

                res.clear();
                db_program.close();
            }
            catch ( const pqxx::sql_error& error )
            {
                MessageBoxA( 0, error.query().c_str(), error.sqlstate().c_str(), 0 );
            }
        }
        catch ( const pqxx::broken_connection& error )
        {
            MessageBoxA( 0, error.what(), "error bd", 0 );
        }
    }

    void delete_currect_order( const int id )
    {
        try
        {
            pqxx::connection db_program( connect_db );
            pqxx::work  xact( db_program, "SampleSelect" );

            try
            {

                pqxx::result res = xact.exec( std::format( R"(DELETE FROM "orders" WHERE "Id" = {};)", ctx::cfg->orders_info[ id ].id ) );
                xact.commit();

                res.clear();
                db_program.close();

                get_orders();
            }
            catch ( const pqxx::sql_error& error )
            {
                MessageBoxA( 0, error.query().c_str(), error.sqlstate().c_str(), 0 );
            }
        }
        catch ( const pqxx::broken_connection& error )
        {
            MessageBoxA( 0, error.what(), "error bd", 0 );
        }
    }

    void  accept_currect_order( const  int id )
    {

        try
        {
            pqxx::connection db_program( connect_db );
            pqxx::work  xact( db_program, "SampleSelect" );

            try
            {

                pqxx::result res = xact.exec( std::format( R"(UPDATE "orders" SET "IsReady" = true WHERE "Id" = {})", ctx::cfg->orders_info[ id ].id ) );

                res.clear();
                xact.commit();

                db_program.close();

                get_orders();
            }
            catch ( const pqxx::sql_error& error )
            {
                MessageBoxA( 0, error.query().c_str(), error.sqlstate().c_str(), 0 );
            }
        }
        catch ( const pqxx::broken_connection& error )
        {
            MessageBoxA( 0, error.what(), "error bd", 0 );
        }

    }






    //crutches solution

    void get_all_guarantee()
    {
        try
        {
            pqxx::connection db_program( connect_db );
            pqxx::work  xact( db_program, "SampleSelect" );

            try
            {
                ctx::cfg->guarantee_info.clear();

                pqxx::result res = xact.exec(std::format( R"(SELECT "orders"."Id","orders"."Time","items"."Name","items"."Guarantee","items"."TypeId" FROM "orders" INNER JOIN "basket_items" ON "basket_items"."OrdersId" = "orders"."Id" INNER JOIN "items" ON "items"."Id" = "basket_items"."ItemsId" WHERE "UserId" = {} AND "IsReady" = true)",ctx::cfg->user_settings.id ) );
                xact.commit();
                if ( !res.size() )
                {
                    res.clear();
                    db_program.close();
                    return;
                }

                int i = 0;
                for ( auto get_info : res )
                {

                    auto guarantee = ctx::cfg_t::guarantee_t();
                    guarantee.name = ctx::type_item[get_info[4].as<int>() - 1] +  std::string(" ") + get_info[2].as<std::string>();


                    std::string get_time = get_info[ 1 ].c_str();
                    std::string back_up = get_time;
                    back_up.erase( 6, back_up.size() - 6 );
                    get_time.erase( 0, 6 );

                    guarantee.date = back_up + std::to_string( atoi( get_time.c_str() ) + get_info[ 3 ].as<int>() );

                    get_time.clear();
                    back_up.clear();


                    ctx::cfg->guarantee_info[ i ] = std::move( guarantee );

                    i++;
                }
              

                res.clear();
                db_program.close();
            }
            catch ( const pqxx::sql_error& error )
            {
                MessageBoxA( 0, error.query().c_str(), error.sqlstate().c_str(), 0 );
            }
        }
        catch ( const pqxx::broken_connection& error )
        {
            MessageBoxA( 0, error.what(), "error bd", 0 );
        }
    }









    void get_info_report(const int id)
    {
        try
        {
            pqxx::connection db_program( connect_db );
            pqxx::work  xact( db_program, "SampleSelect" );

            try
            {
                ctx::cfg->reports_select_info.clear();

                pqxx::result res = xact.exec( std::format( R"(SELECT "items"."Price","items"."TypeId" from "basket_items" INNER JOIN "items" ON "items"."Id" = "basket_items"."ItemsId" WHERE "OrdersId" = {};)",ctx::cfg->reports_info[id].id ) );
                xact.commit();
                if ( !res.size() )
                {
                    res.clear();
                    db_program.close();

                    return;
                }

                int i = 0;
                for ( auto get_info : res )
                {
                    auto report_select = ctx::cfg_t::select_reports_t();
                    report_select.price = get_info[ 0 ].as<int>();
                    report_select.type_id_item = get_info[ 1 ].as<int>();



                    ctx::cfg->reports_select_info[ i ] = std::move( report_select );
                    i++;

                }

                res.clear();
                db_program.close();
            }
            catch ( const pqxx::sql_error& error )
            {
                MessageBoxA( 0, error.query().c_str(), error.sqlstate().c_str(), 0 );
            }
        }
        catch ( const pqxx::broken_connection& error )
        {
            MessageBoxA( 0, error.what(), "error bd", 0 );
        }
    }

    void get_reports()
    {
        try
        {
            pqxx::connection db_program( connect_db );
            pqxx::work  xact( db_program, "SampleSelect" );

            try
            {

                ctx::cfg->reports_info.clear();

                pqxx::result res = xact.exec( R"(SELECT DISTINCT "orders"."Id", "orders"."AllPrice", "orders"."Time" FROM "orders","basket_items" WHERE "IsReady" = true and "basket_items"."OrdersId" = "orders"."Id")" );
                xact.commit();
                if ( !res.size() )
                {
                    res.clear();
                    db_program.close();

                    return;
                }

                int i = 0;
                for ( auto get_info : res )
                {
                    auto reports = ctx::cfg_t::reports_t();
                    reports.id = get_info[ 0 ].as<int>();
                    reports.all_sum = get_info[ 1 ].as<int>();
                    reports.date = get_info[ 2 ].c_str();


                    ctx::cfg->reports_info[ i ] = std::move( reports );
                    i++;

                }

                res.clear();
                db_program.close();
            }
            catch ( const pqxx::sql_error& error )
            {
                MessageBoxA( 0, error.query().c_str(), error.sqlstate().c_str(), 0 );
            }
        }
        catch ( const pqxx::broken_connection& error )
        {
            MessageBoxA( 0, error.what(), "error bd", 0 );
        }
    }

}