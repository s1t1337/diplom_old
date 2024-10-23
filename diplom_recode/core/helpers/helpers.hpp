#pragma once
#include <string>
#include <vector>

namespace helpers
{
    std::string b64encode( const std::string& str );
    std::string b64decode( const std::string& str64 );

	int read_file_to_memory( const std::string_view file_path, std::vector<uint8_t>* out_buffer );

    std::string generate_update_item( const int image_id, const int type_id, const int index_item );
}