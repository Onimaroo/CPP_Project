#include "aircraft_manager.hpp"
#include "airport.hpp"
#include <numeric>
using namespace std;

bool AircraftManager::move()
{
    sort(aircrafts.begin(), aircrafts.end(), [](const std::unique_ptr<Aircraft> &airplane_1, const std::unique_ptr<Aircraft> &airplane_2)
         {
        assert(airplane_1 != nullptr && airplane_2 != nullptr);
        return (airplane_1->get_fuel() < airplane_2->get_fuel()); });

    for (auto it = aircrafts.begin(); it != aircrafts.end();)
    {
        try
        {
            if (!(*it)->move())
            {
                it = aircrafts.erase(it);
            }
            else
            {
                ++it;
            }
        }
        catch (const AircraftCrash &crash)
        {
            it = aircrafts.erase(it);
            crash_counter++;
            std::cerr << "Error: " << crash.what() << std::endl;
        }
    }
    return true;
}

void AircraftManager::create_aircraft(std::unique_ptr<Aircraft> aircraft)
{
    std::cout << "Added aircraft " << aircraft->get_flight_num() << " with " << aircraft->get_fuel() << std::endl;
    aircrafts.push_back(std::move(aircraft));
    std::cout << "List of Aircrafts:" << std::endl;
    for (auto it = aircrafts.begin(); it != aircrafts.end();)
    {
        std::cout << "Aircraft " << (*it)->get_flight_num() << " with " << (*it)->get_fuel() << std::endl;
        it++;
    }
}

unsigned int AircraftManager::get_required_fuel() const
{
    int sum = 0;
    for (auto it = aircrafts.begin(); it != aircrafts.end();)
    {
        assert((*it) != nullptr);
        if ((*it)->is_low_on_fuel() && (*it)->is_on_ground())
        {
            sum += (3000 - (*it)->get_fuel());
        }
        else
        {
            it++;
        }
    }
    return sum;
}