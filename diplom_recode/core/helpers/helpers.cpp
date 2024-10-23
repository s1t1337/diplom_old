#include <format>
#include <uchar.h>
#include <fstream>


#include "helpers.hpp"
#include "../../context/cfg.hpp"
#include <iostream>



namespace helpers
{
    static const char* B64chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    static const int B64index[ 256 ] =
    {
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  62, 63, 62, 62, 63,
        52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 0,  0,  0,  0,  0,  0,
        0,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14,
        15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 0,  0,  0,  0,  63,
        0,  26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
        41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51
    };

    const std::string b64encode( const void* data, const size_t& len )
    {
        std::string result( ( len + 2 ) / 3 * 4, '=' );
        unsigned char* p = ( unsigned  char* )data;
        char* str = &result[ 0 ];
        size_t j = 0, pad = len % 3;
        const size_t last = len - pad;

        for ( size_t i = 0; i < last; i += 3 )
        {
            int n = int( p[ i ] ) << 16 | int( p[ i + 1 ] ) << 8 | p[ i + 2 ];
            str[ j++ ] = B64chars[ n >> 18 ];
            str[ j++ ] = B64chars[ n >> 12 & 0x3F ];
            str[ j++ ] = B64chars[ n >> 6 & 0x3F ];
            str[ j++ ] = B64chars[ n & 0x3F ];
        }
        if ( pad )  /// Set padding
        {
            int n = --pad ? int( p[ last ] ) << 8 | p[ last + 1 ] : p[ last ];
            str[ j++ ] = B64chars[ pad ? n >> 10 & 0x3F : n >> 2 ];
            str[ j++ ] = B64chars[ pad ? n >> 4 & 0x03F : n << 4 & 0x3F ];
            str[ j++ ] = pad ? B64chars[ n << 2 & 0x3F ] : '=';
        }
        return result;
    }

    const std::string b64decode( const void* data, const size_t& len )
    {
        if ( len == 0 ) return "";

        unsigned char* p = ( unsigned char* )data;
        size_t j = 0,
            pad1 = len % 4 || p[ len - 1 ] == '=',
            pad2 = pad1 && ( len % 4 > 2 || p[ len - 2 ] != '=' );
        const size_t last = ( len - pad1 ) / 4 << 2;
        std::string result( last / 4 * 3 + pad1 + pad2, '\0' );
        unsigned char* str = ( unsigned char* )&result[ 0 ];

        for ( size_t i = 0; i < last; i += 4 )
        {
            int n = B64index[ p[ i ] ] << 18 | B64index[ p[ i + 1 ] ] << 12 | B64index[ p[ i + 2 ] ] << 6 | B64index[ p[ i + 3 ] ];
            str[ j++ ] = n >> 16;
            str[ j++ ] = n >> 8 & 0xFF;
            str[ j++ ] = n & 0xFF;
        }
        if ( pad1 )
        {
            int n = B64index[ p[ last ] ] << 18 | B64index[ p[ last + 1 ] ] << 12;
            str[ j++ ] = n >> 16;
            if ( pad2 )
            {
                n |= B64index[ p[ last + 2 ] ] << 6;
                str[ j++ ] = n >> 8 & 0xFF;
            }
        }
        return result;
    }

    std::string b64encode( const std::string& str )
    {
        return b64encode( str.c_str(), str.size() );
    }

    std::string b64decode( const std::string& str64 )
    {
        return b64decode( str64.c_str(), str64.size() );
    }

    int read_file_to_memory( const std::string_view file_path, std::vector<uint8_t>* out_buffer )
    {
        std::ifstream file_ifstream( file_path.data(), std::ios::binary );

        if ( !file_ifstream )
            return false;

        out_buffer->assign( ( std::istreambuf_iterator<char>( file_ifstream ) ), std::istreambuf_iterator<char>() );
        int size = file_ifstream.tellg();
        file_ifstream.close();

        return size;
    }


    const char* processor_names[] =
    {
        "Processor_ProcessorGeneration",
        "Processor_Socket",
        "Processor_NumberOfCores",
        "Processor_MaxNumberOfThreads",
        "Processor_Core",
        "Processor_CacheL1Instructions",
        "Processor_CacheL1Data",
        "Processor_L2CacheSize",
        "Processor_L3CacheSize",
        "Processor_BaseFrequency",
        "Processor_MaxFrequencyTurbo",
        "Processor_MemoryType"
    };

    const char* motherboard_names[] =
    {
        "MotherBoard_FormFactor",
        "MotherBoard_Height",
        "MotherBoard_Thickness",
        "MotherBoard_Socket",
        "MotherBoard_Chipset",
        "MotherBoard_CountRAMSlots",
        "MotherBoard_MemoryFormFactor",
        "MotherBoard_MemoryType",
        "MotherBoard_CountMemoryChannels",
        "MotherBoard_MaxMemoryCapacity",
        "MotherBoard_MaxMemoryFrequency",
        "MotherBoard_RAMFrequency",
        "MotherBoard_CountConnectorsM2",
        "MotherBoard_ConnectorsM2",
        "MotherBoard_CountSATAPorts",
        "MotherBoard_OtherDriveConnectors"
    };

    const char* videocard_names[] =
    {
        "VideoCard_FormFactor",
        "VideoCard_Length",
        "VideoCard_Thickness",
        "VideoCard_MemoryCapacity",
        "VideoCard_MemoryType",
        "VideoCard_VideoConnectors",
        "VideoCard_HDMIVersion",
        "VideoCard_MaxResolution",
        "VideoCard_CountMonitors",
        "VideoCard_ConnectionInterface",
        "VideoCard_PCIExpressVersion"
    };

    const char* ram_names[] =
    {
        "RAM_MemoryType",
        "RAM_FormFactor",
        "RAM_MemoryCapacity",
        "RAM_ClockFrequency"
    };

    const char* powe_supply_names[] =
    {
        "PowerSupplyUnit_FormFactor",
        "PowerSupplyUnit_Power",
        "PowerSupplyUnit_DetachableCables",
        "PowerSupplyUnit_PCIEPowerSupply",
        "PowerSupplyUnit_Thickness",
        "PowerSupplyUnit_Height"
    };

    const char* body_names[] =
    {
        "Body_FormFactor",
        "Body_OrientationMatBoards",
        "Body_Length",
        "Body_Thickness",
        "Body_Height",
        "Body_FormFactorMatBoards",
        "Body_BPFormFactor",
        "Body_BP_Placement",
        "Body_LocationIOPanel",
        "Body_DustFilter",
        "Body_SideFans",
        "Body_LowerFans",
        "Body_UpperFans",
        "Body_FrontFans",
        "Body_FansIncluded"
    };

    const char* ssd_names[] = 
    {
        "SSD_MemoryCapacity",
        "SSD_PhysInterface",
        "SSD_MaxSpeedRecordings",
        "SSD_MaxSpeedReadings",
        "SSD_SpeedProductionRecords",
        "SSD_SpeedProductionReadings"
    };

    const char* hard_drivers_names[]
    {
        "HardDrives_MemoryCapacity",
        "HardDrives_PhysInterface",
        "HardDrives_Bandwidth",
        "HardDrives_AverageDelayTime",
        "HardDrives_MaxDataTransferRate"
    };

    const char* computer_cooling_names[] =
    {
        "ComputerCooling_Socket",
        "ComputerCooling_Size",
        "ComputerCooling_ConnectorForConnection",
        "ComputerCooling_MaxRotationSpeed",
        "ComputerCooling_MinRotationSpeed",
        "ComputerCooling_SpeedAdjustment",
        "ComputerCooling_MaxNoiseLevel"
    };


    std::string generate_update_item( const int image_id, const int type_id,const int index_item)
    {


        std::cout << "ImageId:" << image_id << std::endl << "TypeId:" << type_id << std::endl << "IdItem:" << index_item << std::endl;

        std::string sql_request = R"(UPDATE "items" SET "Name" = '%s',"ImageId" = %d, "TypeId" = %d, "Price" = '%s', "Guarantee" = '%s',)";


        if ( type_id == 0 )
        {
            for ( size_t i = 0; i < _countof( processor_names ); i++ )
            {

                if ( i == _countof( processor_names ) - 1 )
                    sql_request += std::format( R"("{}" = '{}' WHERE "Id" = {})", processor_names[ i ], ctx::cfg->processor_render_edit.at( i ).second->c_str(), ctx::cfg->item_section_info[ index_item ].id );

                if ( i != _countof( processor_names ) - 1 )
                    sql_request += std::format( R"("{}" = '{}',)", processor_names[ i ], ctx::cfg->processor_render_edit.at( i ).second->c_str() );
            }
        }
        else if ( type_id == 1 )
        {
            for ( size_t i = 0; i < _countof( motherboard_names ); i++ )
            {

                if ( i == _countof( motherboard_names ) - 1 )
                    sql_request += std::format( R"("{}" = '{}' WHERE "Id" = {})", motherboard_names[ i ], ctx::cfg->motherboard_render_edit.at( i ).second->c_str(), ctx::cfg->item_section_info[ index_item ].id );

                if ( i != _countof( motherboard_names ) - 1 )
                    sql_request += std::format( R"("{}" = '{}',)", motherboard_names[ i ], ctx::cfg->motherboard_render_edit.at( i ).second->c_str() );
            }
        }
        else if ( type_id == 2 )
        {
            for ( size_t i = 0; i < _countof( videocard_names ); i++ )
            {

                if ( i == _countof( videocard_names ) - 1 )
                    sql_request += std::format( R"("{}" = '{}' WHERE "Id" = {})", videocard_names[ i ], ctx::cfg->videocard_render_edit.at( i ).second->c_str(), ctx::cfg->item_section_info[ index_item ].id );

                if ( i != _countof( videocard_names ) - 1 )
                    sql_request += std::format( R"("{}" = '{}',)", videocard_names[ i ], ctx::cfg->videocard_render_edit.at( i ).second->c_str() );
            }
        }
        else if ( type_id == 3 )
        {
            for ( size_t i = 0; i < _countof( ram_names ); i++ )
            {

                if ( i == _countof( ram_names ) - 1 )
                    sql_request += std::format( R"("{}" = '{}' WHERE "Id" = {})", ram_names[ i ], ctx::cfg->ram_render_edit.at( i ).second->c_str(), ctx::cfg->item_section_info[ index_item ].id );

                if ( i != _countof( ram_names ) - 1 )
                    sql_request += std::format( R"("{}" = '{}',)", ram_names[ i ], ctx::cfg->ram_render_edit.at( i ).second->c_str() );
            }
        }
        else if ( type_id == 4 )
        {
            for ( size_t i = 0; i < _countof( powe_supply_names ); i++ )
            {

                if ( i == _countof( powe_supply_names ) - 1 )
                    sql_request += std::format( R"("{}" = '{}' WHERE "Id" = {})", powe_supply_names[ i ], ctx::cfg->power_supply_render_edit.at( i ).second->c_str(), ctx::cfg->item_section_info[ index_item ].id );

                if ( i != _countof( powe_supply_names ) - 1 )
                    sql_request += std::format( R"("{}" = '{}',)", powe_supply_names[ i ], ctx::cfg->power_supply_render_edit.at( i ).second->c_str() );
            }
        }
        else if ( type_id == 5 )
        {
            for ( size_t i = 0; i < _countof( body_names ); i++ )
            {

                if ( i == _countof( body_names ) - 1 )
                    sql_request += std::format( R"("{}" = '{}' WHERE "Id" = {})", body_names[ i ], ctx::cfg->body_render_edit.at( i ).second->c_str(), ctx::cfg->item_section_info[ index_item ].id );

                if ( i != _countof( body_names ) - 1 )
                    sql_request += std::format( R"("{}" = '{}',)", body_names[ i ], ctx::cfg->body_render_edit.at( i ).second->c_str() );
            }
        }
        else if ( type_id == 6 )
        {
            for ( size_t i = 0; i < _countof( ssd_names ); i++ )
            {

                if ( i == _countof( ssd_names ) - 1 )
                    sql_request += std::format( R"("{}" = '{}' WHERE "Id" = {})", ssd_names[ i ], ctx::cfg->ssd_render_edit.at( i ).second->c_str(), ctx::cfg->item_section_info[ index_item ].id );

                if ( i != _countof( ssd_names ) - 1 )
                    sql_request += std::format( R"("{}" = '{}',)", ssd_names[ i ], ctx::cfg->ssd_render_edit.at( i ).second->c_str() );
            }
        }
        else if ( type_id == 7 )
        {
            for ( size_t i = 0; i < _countof( hard_drivers_names ); i++ )
            {

                if ( i == _countof( hard_drivers_names ) - 1 )
                    sql_request += std::format( R"("{}" = '{}' WHERE "Id" = {})", hard_drivers_names[ i ], ctx::cfg->hard_drivers_render_edit.at( i ).second->c_str(), ctx::cfg->item_section_info[ index_item ].id );

                if ( i != _countof( hard_drivers_names ) - 1 )
                    sql_request += std::format( R"("{}" = '{}',)", hard_drivers_names[ i ], ctx::cfg->hard_drivers_render_edit.at( i ).second->c_str() );
            }
        }
        else if ( type_id == 8 )
        {
            for ( size_t i = 0; i < _countof( computer_cooling_names ); i++ )
            {

                if ( i == _countof( computer_cooling_names ) - 1 )
                    sql_request += std::format( R"("{}" = '{}' WHERE "Id" = {})", computer_cooling_names[ i ], ctx::cfg->computer_cooling_render_edit.at( i ).second->c_str(), ctx::cfg->item_section_info[ index_item ].id );

                if ( i != _countof( computer_cooling_names ) - 1 )
                    sql_request += std::format( R"("{}" = '{}',)", computer_cooling_names[ i ], ctx::cfg->computer_cooling_render_edit.at( i ).second->c_str() );
            }
        }


        auto size = std::snprintf( nullptr, 0, sql_request.c_str(), ctx::cfg->item_section_info[ index_item ].name.c_str(), image_id, type_id + 1, ctx::cfg->item_section_info[ index_item ].price.c_str(), ctx::cfg->item_section_info[ index_item ].guarantee.c_str() );
        std::string output( size + 1, '\0' );
        std::sprintf( &output[ 0 ], sql_request.c_str(), ctx::cfg->item_section_info[ index_item ].name.c_str(), image_id, type_id + 1, ctx::cfg->item_section_info[ index_item ].price.c_str(), ctx::cfg->item_section_info[ index_item ].guarantee.c_str() );

        sql_request.clear();

        std::cout << output << std::endl;

        return output;
    }
}