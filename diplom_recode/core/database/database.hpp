#pragma once
#include <string>

namespace database
{
	bool reg_program( std::string_view sql );
	bool auth_program();

	void get_all_users();
	void update_currect_user( std::string_view sql );
	void delete_currect_user( const int id );


	std::string_view add_image( std::string_view sql );
	void get_image( const int image_id, std::string& out );


	void get_sections();
	void add_sections( std::string_view sql );
	void delete_currect_sections( const int id );

	void add_item_in_currect_section( std::string_view sql ); //update and add
	void get_item_in_currect_section( const int id_section, bool update_info = false );
	void delete_item_in_currect_section( const int id_item, const int id_section );


	void get_orders();
	int add_order( std::string_view sql );
	void get_order_by_id( const int id );
	void delete_currect_order( const int id );
	void accept_currect_order( const int id );



	void add_basket_items( std::string_view sql );



	void get_all_guarantee();


	void get_reports();
	void get_info_report( const int id);

}