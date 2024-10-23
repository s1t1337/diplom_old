#pragma once
#include <Windows.h>
#include <array>
#include <memory>
#include <string>
#include <map>
#include <d3dx9.h>

namespace ctx
{
	enum tabs_id : int
	{
		none = -1,
		orders = 0,
		reports,
		sections,
		users,
		settings,
		auth,
		reg
	};


	enum subtab_orders_id : int
	{
		orders_info = 0,
		edit_orders,
	};

	enum subtab_users_id : int
	{
		list_all_users = 0,
		edit_user
	};

	enum subtab_sections_id : int
	{
		all_sections = 0,
		add_new_section,
		open_currect_section,
		add_new_item_in_currect_section,
		open_currect_item_in_section
	};

	enum subtab_reports_id : int
	{
		all_reports,
		select_report
	};

	struct cfg_t
	{

		struct select_reports_t
		{
			int type_id_item = 0;
			int price = 0;
		};

		struct reports_t
		{
			int id = 0;
			std::string date = {};
			int all_sum = 0;
		};


		struct guarantee_t
		{
			std::string name = {};
			std::string date = {};
		};

		struct select_order_t
		{
			int id = 0;
			int orders_id = 0;
			int type_item_id = 0;
			int count = 1;
			int price = 0;

			std::string name = {};
		};

		struct orders_t
		{
			int id = 0;
			int user_id = 0;		
			int all_count = 0;
			int all_price = 0;
			bool is_ready = false;
			std::string time = {};

		};


		struct items_in_basket_t
		{
			std::string name = {};

			int item_id = 0;
			int price = 0;

			void* picture = nullptr;
		};

		struct user_settings_t
		{
			int id = 0;
			std::string login = {};
			std::string password = {};
			std::string FIO = {};
			std::string number = {};
			std::string email = {};
			std::string date = {};

			bool is_admin = false;
			bool admin_panel = false;

		}user_settings;

		struct section_t
		{
			int id = 0;
			int image_id = 0;
			std::string name = {};
			IDirect3DTexture9* picture = nullptr;

		};


		struct processor_t
		{
			std::string Processor_ProcessorGeneration = {};
			std::string Processor_Socket = {};
			std::string Processor_NumberOfCores = {};
			std::string Processor_MaxNumberOfThreads = {};
			std::string Processor_Core = {};
			std::string Processor_CacheL1Instructions = {};
			std::string Processor_CacheL1Data = {};
			std::string Processor_L2CacheSize = {};
			std::string Processor_L3CacheSize = {};
			std::string Processor_BaseFrequency = {};
			std::string Processor_MaxFrequencyTurbo = {};
			std::string Processor_MemoryType = {};
		};

		struct motherboard_t
		{
			std::string  MotherBoard_FormFactor = {};
			std::string  MotherBoard_Height = {};
			std::string  MotherBoard_Thickness = {};
			std::string  MotherBoard_Socket = {};
			std::string  MotherBoard_Chipset = {};
			std::string  MotherBoard_CountRAMSlots = {};
			std::string  MotherBoard_MemoryFormFactor = {};
			std::string  MotherBoard_MemoryType = {};
			std::string  MotherBoard_CountMemoryChannels = {};
			std::string  MotherBoard_MaxMemoryCapacity = {};
			std::string  MotherBoard_MaxMemoryFrequency = {};
			std::string  MotherBoard_RAMFrequency = {};
			std::string  MotherBoard_CountConnectorsM2 = {};
			std::string  MotherBoard_ConnectorsM2 = {};
			std::string  MotherBoard_CountSATAPorts = {};
			std::string  MotherBoard_OtherDriveConnectors = {};
		};

		struct videocard_t
		{
			std::string VideoCard_FormFactor = {};
			std::string VideoCard_Length = {};
			std::string VideoCard_Thickness = {};
			std::string VideoCard_MemoryCapacity = {};
			std::string VideoCard_MemoryType = {};
			std::string VideoCard_VideoConnectors = {};
			std::string VideoCard_HDMIVersion = {};
			std::string VideoCard_MaxResolution = {};
			std::string VideoCard_CountMonitors = {};
			std::string VideoCard_ConnectionInterface = {};
			std::string VideoCard_PCIExpressVersion = {};
		};

		struct ram_t
		{
			std::string RAM_MemoryType = {};
			std::string RAM_FormFactor = {};
			std::string RAM_MemoryCapacity = {};
			std::string RAM_ClockFrequency = {};
		};

		struct power_supply_t
		{
			std::string PowerSupplyUnit_FormFactor = {};
			std::string PowerSupplyUnit_Power = {};
			std::string PowerSupplyUnit_DetachableCables = {};
			std::string PowerSupplyUnit_PCIEPowerSupply = {};
			std::string PowerSupplyUnit_Thickness = {};
			std::string PowerSupplyUnit_Height = {};
		};

		struct body_t
		{
			std::string Body_FormFactor = {};
			std::string Body_OrientationMatBoards = {};
			std::string Body_Length = {};
			std::string Body_Thickness = {};
			std::string Body_Height = {};
			std::string Body_FormFactorMatBoards = {};
			std::string Body_BPFormFactor = {};
			std::string Body_BP_Placement = {};
			std::string Body_LocationIOPanel = {};
			std::string Body_DustFilter = {};
			std::string Body_SideFans = {};
			std::string Body_LowerFans = {};
			std::string Body_UpperFans = {};
			std::string Body_FrontFans = {};
			std::string Body_FansIncluded = {};
		};

		struct ssd_t
		{
			std::string SSD_MemoryCapacity = {};
			std::string SSD_PhysInterface = {};
			std::string SSD_MaxSpeedRecordings = {};
			std::string SSD_MaxSpeedReadings = {};
			std::string SSD_SpeedProductionRecords = {};
			std::string SSD_SpeedProductionReadings = {};
		};

		struct hard_drivers_t
		{
			std::string HardDrives_MemoryCapacity = {};
			std::string HardDrives_PhysInterface = {};
			std::string HardDrives_Bandwidth = {};
			std::string HardDrives_AverageDelayTime = {};
			std::string HardDrives_MaxDataTransferRate = {};
		};
		

		struct computer_cooling_t
		{
			std::string ComputerCooling_Socket = {};
			std::string ComputerCooling_Size = {};
			std::string ComputerCooling_ConnectorForConnection = {};
			std::string ComputerCooling_MaxRotationSpeed = {};
			std::string ComputerCooling_MinRotationSpeed = {};
			std::string ComputerCooling_SpeedAdjustment = {};
			std::string ComputerCooling_MaxNoiseLevel = {};
		};
		

		struct item_in_section_t
		{
			int id = 0;
			int section_id = 0;
			int type_item_id = 0;
			int image_id = 0;

			std::string guarantee = {};
			std::string price = {};
			
			std::string name = {};

			processor_t processor_type;
			motherboard_t motherboard_type;
			videocard_t videocard_type;
			ram_t ram_type;
			power_supply_t power_supply_type;
			body_t body_type;
			ssd_t ssd_type;
			hard_drivers_t hard_drivers_type;
			computer_cooling_t computer_cooling_type;

			IDirect3DTexture9* picture = nullptr;
		}item_section;


		struct global_t
		{
			int active_tab = tabs_id::auth;

			int subtab_orders = 0;
			int subtab_reports = 0;
			int subtab_sections = 0;
			int subtab_users = 0;

			int currect_type_item = 0;

			int last_index_bakset = 0;

			int count_add_items = 1;

			std::string search_info = {}; 

		}global;

		

		std::map<int, user_settings_t> users_info;

		std::map<int, section_t> section_info;

		std::map<int, item_in_section_t> item_section_info;

		std::map<int, items_in_basket_t> basket_info;

		std::map<int, orders_t> orders_info;

		std::map<int, select_order_t> select_orders_info;

		std::map<int, guarantee_t> guarantee_info;

		std::map<int, reports_t> reports_info;

		std::map<int, select_reports_t> reports_select_info;

		std::array<std::pair<const char*, std::string*>, 4> ram_render =
		{{

			{ reinterpret_cast< const char* >( u8"Тип памяти" ), &item_section.ram_type.RAM_MemoryType },
			{ reinterpret_cast< const char* >( u8"Форм-фактор" ), &item_section.ram_type.RAM_FormFactor },
			{ reinterpret_cast< const char* >( u8"Объем памяти" ), &item_section.ram_type.RAM_MemoryCapacity },
			{ reinterpret_cast< const char* >( u8"Тактовая частота" ),&item_section.ram_type.RAM_ClockFrequency }

		}};

		std::array<std::pair<const char*, std::string*>, 11> videocard_render =
		{ {

			{ reinterpret_cast< const char* >( u8"Форм-фактор" ), &item_section.videocard_type.VideoCard_FormFactor },
			{ reinterpret_cast< const char* >( u8"Длина" ), &item_section.videocard_type.VideoCard_Length },
			{ reinterpret_cast< const char* >( u8"Толщина" ), &item_section.videocard_type.VideoCard_Thickness },
			{ reinterpret_cast< const char* >( u8"Объем памяти" ),&item_section.videocard_type.VideoCard_MemoryCapacity },
			{ reinterpret_cast< const char* >( u8"Тип памяти" ),&item_section.videocard_type.VideoCard_MemoryType },
			{ reinterpret_cast< const char* >( u8"Видеоразъемы" ),&item_section.videocard_type.VideoCard_VideoConnectors },
			{ reinterpret_cast< const char* >( u8"Версия HDMI" ),&item_section.videocard_type.VideoCard_HDMIVersion },
			{ reinterpret_cast< const char* >( u8"Макс. разрешение" ),&item_section.videocard_type.VideoCard_MaxResolution },
			{ reinterpret_cast< const char* >( u8"Кол-во подкл. мониторов" ),&item_section.videocard_type.VideoCard_CountMonitors },
			{ reinterpret_cast< const char* >( u8"Интерфейс подключения" ),&item_section.videocard_type.VideoCard_ConnectionInterface },
			{ reinterpret_cast< const char* >( u8"Версия PCI Express" ),&item_section.videocard_type.VideoCard_PCIExpressVersion }

		} };

		std::array<std::pair<const char*, std::string*>, 16> motherboard_render =
		{ {

			{ reinterpret_cast< const char* >( u8"Форм-фактор" ), &item_section.motherboard_type.MotherBoard_FormFactor },
			{ reinterpret_cast< const char* >( u8"Высота" ), &item_section.motherboard_type.MotherBoard_Height },
			{ reinterpret_cast< const char* >( u8"Ширина" ), &item_section.motherboard_type.MotherBoard_Thickness },
			{ reinterpret_cast< const char* >( u8"Сокет" ),&item_section.motherboard_type.MotherBoard_Socket },
			{ reinterpret_cast< const char* >( u8"Чипсет" ),&item_section.motherboard_type.MotherBoard_Chipset },
			{ reinterpret_cast< const char* >( u8"Кол-во слотов ОЗУ" ),&item_section.motherboard_type.MotherBoard_CountRAMSlots },
			{ reinterpret_cast< const char* >( u8"Форм-фактор памяти" ),&item_section.motherboard_type.MotherBoard_MemoryFormFactor },
			{ reinterpret_cast< const char* >( u8"Тип памяти" ),&item_section.motherboard_type.MotherBoard_MemoryType },
			{ reinterpret_cast< const char* >( u8"Кол-во каналов памяти" ),&item_section.motherboard_type.MotherBoard_CountMemoryChannels },
			{ reinterpret_cast< const char* >( u8"Макс. объем памяти" ),&item_section.motherboard_type.MotherBoard_MaxMemoryCapacity },
			{ reinterpret_cast< const char* >( u8"Макс частота памяти" ),&item_section.motherboard_type.MotherBoard_MaxMemoryFrequency },

			{ reinterpret_cast< const char* >( u8" Частота ОЗУ" ),&item_section.motherboard_type.MotherBoard_RAMFrequency },
			{ reinterpret_cast< const char* >( u8"Кол-во разъемонов М.2" ),&item_section.motherboard_type.MotherBoard_CountConnectorsM2 },
			{ reinterpret_cast< const char* >( u8"Разъемы М.2" ),&item_section.motherboard_type.MotherBoard_ConnectorsM2 },
			{ reinterpret_cast< const char* >( u8"Кол-во портов SATA" ),&item_section.motherboard_type.MotherBoard_CountSATAPorts },
			{ reinterpret_cast< const char* >( u8"Другие разъемы накопителей" ),&item_section.motherboard_type.MotherBoard_OtherDriveConnectors }

		} };

		std::array<std::pair<const char*, std::string*>, 12> processor_render =
		{ {
				
			{ reinterpret_cast< const char* >( u8"Поколение процессоров" ), &item_section.processor_type.Processor_ProcessorGeneration },
			{ reinterpret_cast< const char* >( u8"Сокет" ), &item_section.processor_type.Processor_Socket },
			{ reinterpret_cast< const char* >( u8"Количество ядер" ), &item_section.processor_type.Processor_NumberOfCores },
			{ reinterpret_cast< const char* >( u8"Макс. число потоков" ), &item_section.processor_type.Processor_MaxNumberOfThreads },
			{ reinterpret_cast< const char* >( u8"Ядро" ), &item_section.processor_type.Processor_Core },
			{ reinterpret_cast< const char* >( u8"Кэш L1 инструкции" ), &item_section.processor_type.Processor_CacheL1Instructions },
			{ reinterpret_cast< const char* >( u8"Кэш L1 данные" ), &item_section.processor_type.Processor_CacheL1Data },
			{ reinterpret_cast< const char* >( u8"Объем кэша L2" ), &item_section.processor_type.Processor_L2CacheSize },
			{ reinterpret_cast< const char* >( u8"Объем кэша L3" ), &item_section.processor_type.Processor_L3CacheSize },
			{ reinterpret_cast< const char* >( u8"Базовая частота" ), &item_section.processor_type.Processor_BaseFrequency },
			{ reinterpret_cast< const char* >( u8"Макс. частота(турбо)" ), &item_section.processor_type.Processor_MaxFrequencyTurbo },
			{ reinterpret_cast< const char* >( u8"Тип памяти" ), &item_section.processor_type.Processor_MemoryType }

		} };

		std::array<std::pair<const char*, std::string*>, 6> power_supply_render =
		{ {

			{ reinterpret_cast< const char* >( u8"Форм-фактор" ), &item_section.power_supply_type.PowerSupplyUnit_FormFactor },
			{ reinterpret_cast< const char* >( u8"Мощность" ), &item_section.power_supply_type.PowerSupplyUnit_Power },
			{ reinterpret_cast< const char* >( u8"Отстегивающие кабели" ), &item_section.power_supply_type.PowerSupplyUnit_DetachableCables },
			{ reinterpret_cast< const char* >( u8"Питание PCI-E" ), &item_section.power_supply_type.PowerSupplyUnit_PCIEPowerSupply },
			{ reinterpret_cast< const char* >( u8"Ширина" ), &item_section.power_supply_type.PowerSupplyUnit_Thickness },
			{ reinterpret_cast< const char* >( u8"Высота" ), &item_section.power_supply_type.PowerSupplyUnit_Height }

		} };

		std::array<std::pair<const char*, std::string*>, 15> body_render =
		{{

			{ reinterpret_cast< const char* >( u8"Типоразмер корпуса" ), &item_section.body_type.Body_FormFactor },
			{ reinterpret_cast< const char* >( u8"Ориентация мат.платы" ), &item_section.body_type.Body_OrientationMatBoards },
			{ reinterpret_cast< const char* >( u8"Длина" ), &item_section.body_type.Body_Length },
			{ reinterpret_cast< const char* >( u8"Ширина" ),&item_section.body_type.Body_Thickness },
			{ reinterpret_cast< const char* >( u8"Высота" ),&item_section.body_type.Body_Height },
			{ reinterpret_cast< const char* >( u8"Форм-фактор мат.платы" ),&item_section.body_type.Body_FormFactorMatBoards },
			{ reinterpret_cast< const char* >( u8"Форм-фактор БП" ),&item_section.body_type.Body_BPFormFactor },
			{ reinterpret_cast< const char* >( u8"Размещение БП" ),&item_section.body_type.Body_BP_Placement },
			{ reinterpret_cast< const char* >( u8"Расположение I/O панели" ),&item_section.body_type.Body_LocationIOPanel },
			{ reinterpret_cast< const char* >( u8"Пылевой фильтр" ),&item_section.body_type.Body_DustFilter },
			{ reinterpret_cast< const char* >( u8"Боковые вентеляторы" ),&item_section.body_type.Body_SideFans },
			{ reinterpret_cast< const char* >( u8"Нижные вентеляторы" ),&item_section.body_type.Body_LowerFans },
			{ reinterpret_cast< const char* >( u8"Верхние вентеляторы" ),&item_section.body_type.Body_UpperFans },
			{ reinterpret_cast< const char* >( u8"Фронтальные вентеляторы" ),&item_section.body_type.Body_FrontFans },
			{ reinterpret_cast< const char* >( u8"Вентеляторы в комплекте" ),&item_section.body_type.Body_FansIncluded }

		}};

		std::array<std::pair<const char*, std::string*>, 6> ssd_render =
		{ {

			{ reinterpret_cast< const char* >( u8"Объем памяти" ), &item_section.ssd_type.SSD_MemoryCapacity },
			{ reinterpret_cast< const char* >( u8"Физ. интерфейс" ), &item_section.ssd_type.SSD_PhysInterface },
			{ reinterpret_cast< const char* >( u8"Макс. скорость послед. записи" ), &item_section.ssd_type.SSD_MaxSpeedRecordings },
			{ reinterpret_cast< const char* >( u8"Макс. скорость послед. чтения" ), &item_section.ssd_type.SSD_MaxSpeedReadings },
			{ reinterpret_cast< const char* >( u8"Скорость произв. записи 4кб" ), &item_section.ssd_type.SSD_SpeedProductionRecords },
			{ reinterpret_cast< const char* >( u8"Скорость произв. чтения 4кб" ), &item_section.ssd_type.SSD_SpeedProductionReadings }

		} };

		std::array<std::pair<const char*, std::string*>, 5> hard_drivers_render =
		{ {

			{ reinterpret_cast< const char* >( u8"Объем памяти" ), &item_section.hard_drivers_type.HardDrives_MemoryCapacity },
			{ reinterpret_cast< const char* >( u8"Интерфейс" ), &item_section.hard_drivers_type.HardDrives_PhysInterface },
			{ reinterpret_cast< const char* >( u8"Пропускная способность" ), &item_section.hard_drivers_type.HardDrives_Bandwidth },
			{ reinterpret_cast< const char* >( u8"Среднее время задержки" ), &item_section.hard_drivers_type.HardDrives_AverageDelayTime },
			{ reinterpret_cast< const char* >( u8"Макс. скорость передачи данных" ), &item_section.hard_drivers_type.HardDrives_MaxDataTransferRate }

		} };

		std::array<std::pair<const char*, std::string*>, 7> computer_cooling_render =
		{ {

			{ reinterpret_cast< const char* >( u8"Сокет" ), &item_section.computer_cooling_type.ComputerCooling_Socket },
			{ reinterpret_cast< const char* >( u8"Размер" ), &item_section.computer_cooling_type.ComputerCooling_Size },
			{ reinterpret_cast< const char* >( u8"Разъем для поключения" ), &item_section.computer_cooling_type.ComputerCooling_ConnectorForConnection },
			{ reinterpret_cast< const char* >( u8"Макс. скорость вращения" ), &item_section.computer_cooling_type.ComputerCooling_MaxRotationSpeed },
			{ reinterpret_cast< const char* >( u8"Мин. скорость вращения" ), &item_section.computer_cooling_type.ComputerCooling_MinRotationSpeed },
			{ reinterpret_cast< const char* >( u8"Регулировка скорости" ), &item_section.computer_cooling_type.ComputerCooling_SpeedAdjustment },
			{ reinterpret_cast< const char* >( u8"Макс. уровень шума" ), &item_section.computer_cooling_type.ComputerCooling_MaxNoiseLevel }

		} };



		std::array<std::pair<const char*, std::string*>, 4> ram_render_edit;

		std::array<std::pair<const char*, std::string*>, 11> videocard_render_edit;

		std::array<std::pair<const char*, std::string*>, 16> motherboard_render_edit;

		std::array<std::pair<const char*, std::string*>, 12> processor_render_edit;

		std::array<std::pair<const char*, std::string*>, 6> power_supply_render_edit;

		std::array<std::pair<const char*, std::string*>, 15> body_render_edit;

		std::array<std::pair<const char*, std::string*>, 6> ssd_render_edit;

		std::array<std::pair<const char*, std::string*>, 5> hard_drivers_render_edit;

		std::array<std::pair<const char*, std::string*>, 7> computer_cooling_render_edit;

	};

	inline const char* name_tabs[] =
	{
	   reinterpret_cast< const char* >( u8"Заказы" ),
	   reinterpret_cast< const char* >( u8"Отчеты" ),
	   reinterpret_cast< const char* >( u8"Разделы" ),
	   reinterpret_cast< const char* >( u8"Пользователи" ),
	   reinterpret_cast< const char* >( u8"Настройки" )
	};

	inline const char* user_tabs[] =
	{

	   reinterpret_cast< const char* >( u8"Заказы" ),
	   reinterpret_cast< const char* >( u8"Гарантии" ),
	   reinterpret_cast< const char* >( u8"Разделы" ),
	   reinterpret_cast< const char* >( u8"Корзина" ),
	   reinterpret_cast< const char* >( u8"Настройки" )

	};

	inline const char* type_item[] =
	{
	   reinterpret_cast< const char* >( u8"Процессор" ),
	   reinterpret_cast< const char* >( u8"Материнская плата" ),
	   reinterpret_cast< const char* >( u8"Видеокарта" ),
	   reinterpret_cast< const char* >( u8"Оперативная память" ),
	   reinterpret_cast< const char* >( u8"Блок питания" ),
	   reinterpret_cast< const char* >( u8"Корпус" ),
	   reinterpret_cast< const char* >( u8"SSD накопители" ),
	   reinterpret_cast< const char* >( u8"Жесткие диски" ),
	   reinterpret_cast< const char* >( u8"Охлаждение компьютера" )

	};

	extern std::unique_ptr< cfg_t > cfg;
}