#pragma once 

#include "../net.hh"
#include "../../../global/std/collection.hh"
#include <fstream>

namespace kiwi::hardware {
    class Track;
    class Bump;
}

namespace kiwi::circuit {

    class TrackToBumpsNet : public Net {
    public:
        TrackToBumpsNet(hardware::Track* begin_track, std::Vector<hardware::Bump*> end_bumps);
        virtual ~TrackToBumpsNet() noexcept override;

    public:
        virtual auto update_tob_postion(hardware::TOB* prev_tob, hardware::TOB* next_tob) -> void override;
        virtual auto route(hardware::Interposer* interposer, const algo::RouteStrategy& strategy) -> void override;
        virtual auto priority() const -> Priority override;

        auto begin_track() const -> hardware::Track* { return this->_begin_track; }
        auto end_bumps() const -> const std::Vector<hardware::Bump*>& { return this->_end_bumps; }
        virtual auto coords() const -> std::Vector<hardware::Coord> override;
        auto checkNetInfo(std::ofstream& outputfile) -> void override;

    private:
        hardware::Track* _begin_track;
        std::Vector<hardware::Bump*> _end_bumps;
    };

}