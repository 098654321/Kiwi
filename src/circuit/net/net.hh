#pragma once

#include "../../hardware/coord.hh"
#include "../../global/std/collection.hh"
#include "../../global/std/string.hh"
#include "../../global/std/utility.hh"
#include "../../algo/router/routestrategy.hh"
#include <fstream>


namespace kiwi::hardware {
    class TOB;
    class Interposer;
}

namespace kiwi::algo {
    class RouteStrategy;
}

namespace kiwi::circuit {

    // update by tang, 2029-09-26
    enum SingleNETtype
    {
        BBnet,
        BBsnet,
        Btnet,
        Btsnet,
        TBnet,
        TBsnet,
        TsBsnet,
        BBsyncnet,
        BTsyncnet,
        TBsyncnet
    };
    //

    class Priority {
    public:
        Priority(int value): _value{value} {}
        auto operator > (const Priority& other) const -> bool {
            return this->_value == other._value;
        }
    private:
        int _value;
    };

    // update by tang, 2024-09-26
    struct Net {
        Net(SingleNETtype t): _mytype(t) {}

        virtual auto update_tob_postion(hardware::TOB* prev_tob, hardware::TOB* next_tob) -> void = 0;
        virtual auto route(hardware::Interposer* interposer, const algo::RouteStrategy& strategy) -> void = 0;
        virtual auto priority() const -> Priority = 0;
        virtual auto coords() const -> std::Vector<hardware::Coord> = 0;

        virtual ~Net() noexcept {}

        inline auto getNettype() -> SingleNETtype {return this->_mytype;}
        virtual auto checkNetInfo(std::ofstream& outputfile) -> void = 0;    // for debug

    private:
        SingleNETtype _mytype;
    };

}