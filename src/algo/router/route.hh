#pragma once

#include <global/std/memory.hh>
#include <global/std/collection.hh>
#include <global/debug/exception.hh>

namespace kiwi::hardware {
    class Interposer;
}

namespace kiwi::circuit {
    class Net;
}

namespace kiwi::algo {

    class RouteStrategy;
    
    auto route_nets(
        hardware::Interposer* interposer,
        std::Vector<std::Box<circuit::Net>>& nets,
        const RouteStrategy& strategy
    ) -> void;

}