#pragma once

#include <nlohmann/json.hpp>
#include <string>

class PowerState
{
private:
	float self_suffiecency;
	float from_grid;
	float to_grid;
	float to_battery;
	float from_battery;
	float battery_soc;
	float generation;
	float usage;
	std::string timestamp;

	float maxGridUtilization;
	float maxBatteryUtilization;
	float maxGenerationUtilization;
	float maxUsageUtilization;

public:
	PowerState(nlohmann::json json_data);

	std::string getSelfSuffiency();
	std::string getTimeStamp();
	std::string getBatterySOC();

	std::string getGridState();
	std::string getBatteryState();
	std::string getGeneration();
	std::string getUsage();

	float getGridUtilization();
	float getBatteryUtilization();
	float getGenerationUtilization();
	float getUsageUtilization();

	bool drawingFromGrid();
	bool drawingFromBattery();
};

