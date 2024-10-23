#include "topdie.hh"

namespace kiwi::circuit
{
    TopDie::TopDie(const String& name, HashMap<String, usize> pins_map)
    :_name(name), _pins_map(pins_map)
    {}

    TopDie::TopDie(const String& name):_name(name)
    {}

    void TopDie::add_pin(const String& pin_name, const usize& index)
    {
        this->_pins_map.emplace(pin_name, index);
    }

    auto TopDie::checkInfo(std::ofstream& outputfile) -> void
    {
        outputfile << "TopDie: " << _name << std::endl;
        outputfile << "Pins map: " << std::endl;
        for (auto& pin: _pins_map)
        {
            outputfile << pin.first << ", index=" << pin.second << std::endl; 
        }
        outputfile << std::endl;
    }
}