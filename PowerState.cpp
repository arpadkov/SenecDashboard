#include "PowerState.h"
#include "Exceptions.h"
#include <nlohmann/json.hpp>
#include <sstream>

using string = std::string;
using json = nlohmann::json;

string roundFloatToStr(float number, int precision)
{
	std::stringstream stream;
	stream << std::fixed << std::setprecision(precision) << number;
	return stream.str();
}

PowerState::PowerState(json json_data)
{
	timestamp = json_data["zeitstempel"];

	try
	{
		// All values in W
		self_suffiecency = json_data["aktuell"]["autarkie"]["wert"];
		from_grid = json_data["aktuell"]["netzbezug"]["wert"];
		to_grid = json_data["aktuell"]["netzeinspeisung"]["wert"];
		to_battery = json_data["aktuell"]["speicherbeladung"]["wert"];
		from_battery = json_data["aktuell"]["speicherentnahme"]["wert"];
		battery_soc = json_data["aktuell"]["speicherfuellstand"]["wert"];
		generation = json_data["aktuell"]["stromerzeugung"]["wert"];
		usage = json_data["aktuell"]["stromverbrauch"]["wert"];
	}
	catch (const std::exception& ex)
	{
		throw IncorrectDashboardResponseException();
	}

	maxGridUtilization = 5000;
	maxBatteryUtilization = 2000;
	maxGenerationUtilization = 5000;
	maxUsageUtilization = 5000;
}

string PowerState::getSelfSuffiency()
{
	// Returns self-sufficency formatted string in percent
	return roundFloatToStr(self_suffiecency, 2) + " %";
}

string PowerState::getTimeStamp()
{
	// Formatting the timestamp (a littlebit)
	string formatted_time = timestamp.replace(10, 1, " ").replace(19, 1, "");
	return formatted_time;
}

string PowerState::getBatterySOC()
{
	// Returns battery StateOfCharge in percent
	return roundFloatToStr(battery_soc, 2) + " %";
}

string PowerState::getGridState()
{
	// Returns formatted string in kW, to-grid->positive, from-grid->negative

	if (from_grid < 100 && to_grid < 100)
	{
		// Grid neglageble
		return "0 kWh";
	}

	if (drawingFromGrid()) {
		// Drawing power from grid
		return roundFloatToStr(-1 * from_grid / 1000, 2) + " kW";
	}
	else
	{
		// Supplying power to grid
		return roundFloatToStr(to_grid / 1000, 2) + " kW";
	}
}

string PowerState::getBatteryState()
{
	// Returns formatted string in kWh, to-battery->positive, from-battery->negative

	if (from_battery < 100 && to_battery < 100)
	{
		// Battery usage neglageble
		return "0 kWh";
	}

	if (drawingFromBattery()) {
		// Drawing power from battery
		return roundFloatToStr(-1 * from_battery / 1000, 2) + " kW";
	}
	else
	{
		// Supplying power to battery
		return roundFloatToStr(to_battery / 1000, 2) + " kW";
	}
}

string PowerState::getGeneration()
{
	// Returns power generation formatted string in kW
	return roundFloatToStr(generation / 1000, 2) + " kW";
}


string PowerState::getUsage()
{
	// Returns power usage formatted string in kWh
	return roundFloatToStr(usage / 1000, 2) + " kW";
}

float PowerState::getGridUtilization()
{
	// Returns utilization, used to adjust arrow width
	// Max utilization defined in PowerState class

	float utilization;

	if (from_grid < 100 && to_grid < 100)
	{
		// Grid neglageble
		return 0;
	}

	if (drawingFromGrid()) {
		// Drawing power from grid
		utilization = from_grid / maxGridUtilization;
	}
	else
	{
		// Supplying power to grid
		utilization = to_grid / maxGridUtilization;
	}

	if (utilization > 1)
	{
		return 1;
	}

	return utilization;

}

float PowerState::getBatteryUtilization()
{
	// Returns utilization, used to adjust arrow width
	// Max utilization defined in PowerState class

	float utilization;

	if (from_battery < 100 && to_battery < 100)
	{
		// Battery usage neglageble
		return 0;
	}

	if (drawingFromBattery()) {
		// Drawing power from battery
		utilization = from_battery / maxBatteryUtilization;
	}
	else
	{
		// Supplying power to battery
		utilization = to_battery / maxBatteryUtilization;
	}

	if (utilization > 1)
	{
		return 1;
	}

	return utilization;
}

float PowerState::getGenerationUtilization()
{
	// Returns utilization, used to adjust arrow width
	// Max utilization defined in PowerState class

	float utilization = generation / maxGenerationUtilization;

	if (utilization > 1)
	{
		return 1;
	}

	return utilization;
}

float PowerState::getUsageUtilization()
{
	// Returns utilization, used to adjust arrow width
	// Max utilization defined in PowerState class

	float utilization = usage / maxUsageUtilization;

	if (utilization > 1)
	{
		return 1;
	}

	return utilization;
}

bool PowerState::drawingFromGrid()
{
	if (from_grid > to_grid) {
		return true;
	}
	
	return false;
}


bool PowerState::drawingFromBattery()
{
	if (from_battery > to_battery) {
		return true;
	}

	return false;
}

const char* PowerState::getBatteryIconPath()
{
	if (battery_soc <= 1)
	{
		// Battery empty
		return ":/battery_icon/resources/battery_empty.png";
	}

	if (battery_soc >= 99)
	{
		// Battery full
		return ":/battery_icon/resources/battery_full.png";
	}

	const char* batteryIconPath;

	if (drawingFromBattery())
	{
		if (battery_soc > 80)
		{
			batteryIconPath = ":/battery_icon/resources/battery_80_100_discharging.png";
		}
		else if (battery_soc > 50)
		{
			batteryIconPath = ":/battery_icon/resources/battery_50_80_discharging.png";
		}
		else if (battery_soc > 30)
		{
			batteryIconPath = ":/battery_icon/resources/battery_30_60_discharging.png";
		}
		else
		{
			batteryIconPath = ":/battery_icon/resources/battery_0_30_discharging.png";
		}
	}

	else
	{
		if (battery_soc > 80)
		{
			batteryIconPath = ":/battery_icon/resources/battery_80_100_charging.png";
		}
		else if (battery_soc > 50)
		{
			batteryIconPath = ":/battery_icon/resources/battery_50_80_charging.png";
		}
		else if (battery_soc > 30)
		{
			batteryIconPath = ":/battery_icon/resources/battery_30_60_charging.png";
		}
		else
		{
			batteryIconPath = ":/battery_icon/resources/battery_0_30_charging.png";
		}
	}

	return batteryIconPath;
}
