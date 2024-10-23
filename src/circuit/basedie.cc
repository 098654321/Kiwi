#include "basedie.hh"


namespace kiwi::circuit
{
    auto BaseDie::set_net(std::Vector<std::Box<Net>>& other) -> void
    {
        for (auto& box_net: other)
        {
            _nets.emplace_back(std::move(box_net));
        }
    }
}

