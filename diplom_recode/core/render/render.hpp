#pragma once

#include "../../dependencies/imgui/imgui.h"
#include "../../dependencies/imgui/imgui_impl_dx9.h"
#include "../../dependencies/imgui/imgui_impl_win32.h"
#include "../../dependencies/imgui/imgui_internal.h"
#include "../../dependencies/imgui/imstb_rectpack.h"
#include "../../dependencies/imgui/imstb_textedit.h"
#include "../../dependencies/imgui/imstb_truetype.h"
#include "../../dependencies/imgui/imgui_stdlib.h"

// dx lib
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

namespace render
{


	extern LPDIRECT3D9  g_pD3D;
	extern LPDIRECT3DDEVICE9  g_pd3dDevice;
	extern D3DPRESENT_PARAMETERS g_d3dpp;

	bool CreateDeviceD3D( HWND hWnd );
	void CleanupDeviceD3D();
	void ResetDevice();
	LRESULT WINAPI WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );


	void render_tab( const std::string_view name_tab, const int index, bool user = false );

	bool Button( const char* label, const char* header, const char* description, const ImVec2& size_arg, bool delete_item = false, ImGuiButtonFlags flags = 0 );

	bool ImageButton( std::string_view item, std::string_view paintkit, int rarity, ImTextureID user_texture_id, const ImVec2& size,
		const char* item_id, int frame_padding, bool draw_image = true, bool draw_quality = true );

	const ImVec2 menu_size = ImVec2( 780.0f, 450.0f );

	const int flag_begin = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoTitleBar;
	const int flag_child = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar;

	const int flag_login = ImGuiInputTextFlags_AutoSelectAll;
	const int flag_pass = ImGuiInputTextFlags_Password | ImGuiInputTextFlags_AutoSelectAll;

	inline bool select_table = false;

	namespace image_texture 
	{
		extern  IDirect3DTexture9* user_logo;
	}
}