#include "tower_sim.hpp"

#include "GL/opengl_interface.hpp"
#include "aircraft.hpp"
#include "aircraft_manager.hpp"
#include "airport.hpp"
#include "config.hpp"
#include "img/image.hpp"
#include "img/media_path.hpp"

#include <cassert>
#include <cstdlib>
#include <ctime>

using namespace std::string_literals;

TowerSimulation::~TowerSimulation()
{
    delete airport;
}

void TowerSimulation::create_aircraft()
{
    std::unique_ptr<Aircraft> aircraft = aircraft_factory.create_random_aircraft(airport);
    auto aircraftSet = aircraft_factory.getAircraftsSet();
    if (aircraftSet.find(aircraft->get_flight_num()) != aircraftSet.end())
    {
        std::cout << "Aircraft already created" << std::endl;
        return;
    }
    aircraft_factory.add_flight_number(aircraft->get_flight_num());
    aircraft_manager.create_aircraft(std::move(aircraft));
}

void TowerSimulation::create_keystrokes()
{
    GL::keystrokes.emplace('x', []()
                           { GL::exit_loop(); });
    GL::keystrokes.emplace('q', []()
                           { GL::exit_loop(); });
    // GL::keystrokes.emplace('c', [this]() { create_random_aircraft(); });
    GL::keystrokes.emplace('c', [this]()
                           { create_aircraft(); });
    GL::keystrokes.emplace('+', []()
                           { GL::change_zoom(0.95f); });
    GL::keystrokes.emplace('-', []()
                           { GL::change_zoom(1.05f); });
    GL::keystrokes.emplace('f', []()
                           { GL::toggle_fullscreen(); });
    GL::keystrokes.emplace('a', []()
                           { GL::ticks_per_sec += 10; });
    GL::keystrokes.emplace('d', []()
                           { GL::ticks_per_sec -= 10; });
    GL::keystrokes.emplace('p', []()
                           { 
        if(GL::is_paused == false) {
            GL::is_paused = true;
        }
        else {
            GL::is_paused = false;
        } });
    GL::keystrokes.emplace('m', [this]()
                           { std::cout << "Nombre d'avions qui ont crash :" << aircraft_manager.get_number_of_crashes() << std::endl; });
}

void TowerSimulation::display_help() const
{
    std::cout << "This is an airport tower simulator" << std::endl
              << "the following keysstrokes have meaning:" << std::endl;

    for (const auto &ks_pair : GL::keystrokes)
    {
        std::cout << ks_pair.first << ' ';
    }

    std::cout << std::endl;
}

void TowerSimulation::init_airport()
{
    airport = new Airport{one_lane_airport, Point3D{0, 0, 0},
                          new img::Image{one_lane_airport_sprite_path.get_full_path()}, aircraft_manager};

    // GL::display_queue.emplace_back(airport);
    GL::move_queue.emplace(airport);
}

void TowerSimulation::init_aircraftManager()
{
    GL::move_queue.emplace(&aircraft_manager);
}

void TowerSimulation::launch()
{
    if (help)
    {
        display_help();
        return;
    }

    init_airport();
    init_aircraftManager();
    // init_aircraft_types();

    GL::loop();
}
