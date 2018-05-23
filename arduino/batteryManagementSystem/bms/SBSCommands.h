#ifndef SRC_SBMINFO_H_
#define SRC_SBMINFO_H_

// standard I2C address for Smart Battery packs
#define SBM_DEVICE_ADDRESS					0x0B

//for further detail check bq78350-R1 technical references (sluubd3c)

//SBS Commands Address
#define MANUFACTURER_ACCESS					0x00
#define MANUFACTURER_BLOCK_ACCESS			0x44

#define DESIGN_VOLTAGE						0x19
#define DESIGN_CAPACITY						0x18
#define CYCLE_COUNT							0x17
#define CHARGING_CURRENT					0x14
#define CHARGING_VOLTAGE					0x15
#define BATTERY_STATUS						0x16
#define BATTERY_MODE						0x03
#define RELATIVE_SOC						0x0D
#define ABSOLUTE_SOC						0x0E
#define REMAINING_CAPACITY					0x0F
#define FULL_CHARGE_CAPACITY				0x10
#define VOLTAGE								0x09
#define CURRENT								0x0A
#define AVERAGE_CURRENT						0x0B
#define AT_RATE								0x04
#define AT_RATE_TIME_TO_FULL				0x05
#define AT_RATE_TIME_TO_EMPTY				0x06
#define CELL3_VOLTAGE						0x3D
#define CELL2_VOLTAGE						0x3E
#define CELL1_VOLTAGE						0x3F

//Manufacturer Access Commands
#define DEVICE_RESET						0x0041
#define OPERATION_STATUS					0x0054
#define SECURITY_KEYS                       0x0035

//Data Flash Address
//Configuration
#define FET_OPTIONS							0x445F
#define SBS_GAUGING_CONFIG					0x4461
#define SMB_CONFIG							0x4462
#define POWER_CONFIG						0x4472
#define LED_CONFIG							0x448C
#define TEMPERATURE_ENABLE					0x44A9
#define TEMPERATURE_MODE					0x44AA
#define DA_CONFIGURATION					0x44AB
#define AFE_CELL_MAP						0x44AC
#define CEDV_GAUGING_CONFIG					0x458B
#define SOC_FLAG_CONFIG						0x45D4
#define BALANCING_CONFIG					0x460C

//Protection Settings
#define PROTECTION_CONFIG					0x44C5
#define PROTECTIONS_A_ENABLED				0x44C6
#define PROTECTIONS_B_ENABLED				0x44C7
#define PROTECTIONS_C_ENABLED				0x44C8

//Protections Value
#define CUV_THRESHOLD						0x44CC
#define CUV_DELAY							0x44CE
#define CUV_RECOVERY						0x44CF
#define COV_THRESHOLD						0x44D1
#define COV_DELAY							0x44D3
#define COV_RECOVERY						0x44D4
#define OCC_THRESHOLD						0x44D6
#define OCC_DELAY							0x44D8
#define OCC_RECOVERY_THRESHOLD				0x44D9
#define OCC_RECOVERY_DELAY					0x44DB
#define OCD_THRESHOLD						0x44DC
#define OCD_DELAY							0x44DE
#define OCD_RECOVERY_THRESHOLD				0x44DF
#define OCD_RECOVERY_DELAY					0x44E1

//Fuel Gauging Design
#define DESIGN_CAPACITY_MA_CONFIG			0x45CC
#define DESIGN_CAPACITY_CWH_CONFIG			0x45CE
#define DESIGN_VOLTAGE_CONFIG				0x45D0

//SBS Configuration
#define MANUFACTURER_NAME					0x406F
#define DEVICE_CHEMISTRY					0x4099
#define INITIAL_BATT_MODE					0x44A5
#define SPEC_INFO							0x44A7
#define MANUFACTURING_STATUS_INIT			0x42C8

#endif /* SRC_SBMINFO_H_ */
