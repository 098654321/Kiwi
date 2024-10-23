#pragma once

#include <global/std/memory.hh>
#include <global/std/collection.hh>

namespace kiwi::hardware {
    class Interposer;
}

namespace kiwi::circuit {
    class Net;
    class TopDieInstance;
}

namespace kiwi::algo {

    struct PlaceStrategy {
        virtual auto place(
            hardware::Interposer* interposer,
            std::Vector<circuit::TopDieInstance>& topdies
        ) const -> void = 0;
    };

}