#pragma once

#include <nlohmann/json.hpp>
#include <string>

class PowerUsage
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

public:
	PowerUsage(nlohmann::json json_data);

};

