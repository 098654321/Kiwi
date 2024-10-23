#pragma once 

#include "../net.hh"
#include "./btnet.hh"
#include "../../../global/std/collection.hh"
#include <fstream>

namespace kiwi::hardware {
    class Track;
    class Bump;
}

namespace kiwi::circuit {

    class BumpToTrackSyncNet : public Net {
    public:
        BumpToTrackSyncNet(std::Vector<BumpToTrackNet>&);
        virtual auto update_tob_postion(hardware::TOB* prev_tob, hardware::TOB* next_tob) -> void override;
        virtual auto route(hardware::Interposer* interposer, const algo::RouteStrategy& strategy) -> void override;
        virtual auto priority() const -> Priority override;
        virtual auto coords() const -> std::Vector<hardware::Coord> override;

        auto nets() const -> const std::Vector<BumpToTrackNet>& { return this->_nets; }
        auto checkNetInfo(std::ofstream& outputfile) -> void override;

    private: 
        std::Vector<BumpToTrackNet> _nets;
    };

}