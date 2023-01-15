#include <nlohmann/json.hpp>
#include <sstream>
#include "PowerState.h"

using namespace std;
using json = nlohmann::json;

string roundFloatToStr(float number, int precision)
{
	stringstream stream;
	stream << fixed << setprecision(precision) << number;
	return stream.str();
}

PowerState::PowerState(json json_data)
{
	timestamp = json_data["zeitstempel"];

	self_suffiecency = json_data["aktuell"]["autarkie"]["wert"];
	from_grid = json_data["aktuell"]["netzbezug"]["wert"];
	to_grid = json_data["aktuell"]["netzeinspeisung"]["wert"];
	to_battery = json_data["aktuell"]["speicherbeladung"]["wert"];
	from_battery = json_data["aktuell"]["speicherentnahme"]["wert"];
	battery_soc = json_data["aktuell"]["speicherfuellstand"]["wert"];
	generation = json_data["aktuell"]["stromerzeugung"]["wert"];
	usage = json_data["aktuell"]["stromverbrauch"]["wert"];
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

string PowerState::getGridState()
{
	// Returns formatted string in kW, to-grid->positive, from-grid->negative

	if (from_grid < 100 && to_grid < 100)
	{
		// Grid neglageble
		return "0 kWh";
	}

	if (from_grid > to_grid) {
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

	if (from_battery > to_grid) {
		// Drawing power from battery
		return roundFloatToStr(-1 * from_battery / 1000, 2) + " kW";
	}
	else
	{
		// Supplying power to battery
		return roundFloatToStr(to_grid / 1000, 2) + " kW";
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


string PowerState::getBatterySOC()
{
	// Returns battery StateOfCharge in percent
	return roundFloatToStr(battery_soc, 2) + " kW";
}


