#include <nlohmann/json.hpp>
#include "PowerUsage.h"

using namespace std;
using json = nlohmann::json;

PowerUsage::PowerUsage(json json_data)
{
	self_suffiecency = json_data["aktuell"]["autarkie"];
	from_grid = json_data["aktuell"]["netzbezug"];
	to_grid = json_data["aktuell"]["netzeinspeisung"];
	to_battery = json_data["aktuell"]["speicherbeladung"];
	from_battery = json_data["aktuell"]["speicherentnahme"];
	battery_soc = json_data["aktuell"]["speicherfuellstand"];
	generation = json_data["aktuell"]["stromerzeugung"];
	usage = json_data["aktuell"]["stromverbrauch"];

	std::string timestamp = json_data["zeitstempel"];
}
