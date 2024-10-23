#pragma once 

#include "../net.hh"
#include "../../../global/std/collection.hh"
#include <fstream>

namespace kiwi::hardware {
    class Track;
    class Bump;
}

namespace kiwi::circuit {

    class TracksToBumpsNet : public Net {
    public:
        TracksToBumpsNet(std::Vector<hardware::Track*>& begin_tracks, std::Vector<hardware::Bump*> end_bumps);
        ~TracksToBumpsNet() noexcept override;

        virtual auto update_tob_postion(hardware::TOB* prev_tob, hardware::TOB* next_tob) -> void override;
        virtual auto route(hardware::Interposer* interposer, const algo::RouteStrategy& strategy) -> void override;
        virtual auto priority() const -> Priority override;
        virtual auto coords() const -> std::Vector<hardware::Coord> override;

        auto begin_tracks() const -> const std::Vector<hardware::Track*>& { return this->_begin_tracks; }
        auto end_bumps() const -> const std::Vector<hardware::Bump*>& { return this->_end_bumps; }
        auto checkNetInfo(std::ofstream& outputfile) -> void override;

    private:
        std::Vector<hardware::Track*> _begin_tracks;
        std::Vector<hardware::Bump*>  _end_bumps;
    };

}