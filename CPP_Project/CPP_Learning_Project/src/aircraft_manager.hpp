#pragma once

#include <vector>
#include <memory>
#include <algorithm>
#include "aircraft.hpp"

class AircraftManager : public GL::DynamicObject
{
private:
    std::vector<std::unique_ptr<Aircraft>> aircrafts;

public:
    bool move() override;
    void create_aircraft(std::unique_ptr<Aircraft> aircraft);
    int get_required_fuel();
};