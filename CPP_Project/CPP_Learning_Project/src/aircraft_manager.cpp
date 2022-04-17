#include "aircraft_manager.hpp"
#include "airport.hpp"
using namespace std;

bool AircraftManager::move()
{
    sort(aircrafts.begin(), aircrafts.end(), [](const std::unique_ptr<Aircraft> &first, const std::unique_ptr<Aircraft> &second)
         {
        assert(first != nullptr && second != nullptr);
        return (first->get_fuel() < second->get_fuel()); });

    for (auto it = aircrafts.begin(); it != aircrafts.end();) // Notez bien le manque de l'incremenet ++it ici car on va incrementer it selon la réponse de move()
    {
        if (!(*it)->move())
        {
            // delete *it;               // c'est pas bien, mais necessaire parce qu'on a créé l'avion via new.... cela change dès qu'on trouve un propre owner des avions (TASK1)
            it = aircrafts.erase(it); // ici, on enleve *it de la move_queue d'une facon safe
        }
        else
        {
            ++it;
        }
    }
    return true;
}

void AircraftManager::create_aircraft(std::unique_ptr<Aircraft> aircraft)
{
    aircrafts.push_back(std::move(aircraft));
}

int AircraftManager::get_required_fuel()
{
    int sum = 0;
    for (auto it = aircrafts.begin(); it != aircrafts.end();)
    {
        if ((*it)->is_low_on_fuel() && (*it)->is_on_ground())
        {
            sum += (3000 - (*it)->get_fuel());
        }
    }
    return sum;
}