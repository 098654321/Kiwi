#pragma once 

#include "../net.hh"
#include "./bbnet.hh"
#include "../../../global/std/collection.hh"
#include <fstream>

namespace kiwi::hardware {
    class Track;
    class Bump;
}

namespace kiwi::circuit {

    class BumpToBumpSyncNet : public Net {
    
    public:
        BumpToBumpSyncNet(Vector<BumpToBumpNet>&);
        virtual auto update_tob_postion(hardware::TOB* prev_tob, hardware::TOB* next_tob) -> void override;
        virtual auto route(hardware::Interposer* interposer, const algo::RouteStrategy& strategy) -> void override;
        virtual auto priority() const -> Priority override;
        virtual auto coords() const -> std::Vector<hardware::Coord> override;

        auto nets() const -> const std::Vector<BumpToBumpNet>& { return this->_nets; }
        auto checkNetInfo(ofstream& outputfile) -> void override;

    private: 
        std::Vector<BumpToBumpNet> _nets;
    };

}