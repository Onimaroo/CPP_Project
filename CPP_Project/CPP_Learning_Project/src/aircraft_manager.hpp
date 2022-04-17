#pragma once

#include <vector>
#include <memory>
#include <algorithm>
#include "aircraft.hpp"

class AircraftManager : public GL::DynamicObject
{
private:
    std::vector<std::unique_ptr<Aircraft>> aircrafts;
    int crash_counter = 0;

public:
    bool move() override;
    void create_aircraft(std::unique_ptr<Aircraft> aircraft);
    unsigned int get_required_fuel() const;
    int get_number_of_crashes() const { return crash_counter; };
};