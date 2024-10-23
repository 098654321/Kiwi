#include "./routeerror.hh"
#include <circuit/net/net.hh>
#include <global/std/format.hh>
#include <global/std/integer.hh>
#include <global/std/memory.hh>


namespace kiwi::algo {

    RouteError::RouteError(circuit::Net* net) :
        _net{net} {}

    RouteError::~RouteError() noexcept {}

    auto RouteError::what() const -> std::String {
        // TODO
        return std::format(
            "Route Failed: when routing net '{}'", 
            reinterpret_cast<std::usize>(this->_net)
        );
    }

}