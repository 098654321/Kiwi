#pragma once 

#include "../net.hh"
#include <fstream>


namespace kiwi::hardware {
    class Track;
    class Bump;
}

namespace kiwi::circuit {

    class TrackToBumpNet : public Net {
    public:
        TrackToBumpNet(hardware::Track* begin_track, hardware::Bump* end_bump);
        virtual ~TrackToBumpNet() noexcept override;
    
    public:
        virtual auto update_tob_postion(hardware::TOB* prev_tob, hardware::TOB* next_tob) -> void override;
        virtual auto route(hardware::Interposer* interposer, const algo::RouteStrategy& strategy) -> void override;
        virtual auto priority() const -> Priority override;

    public:
        auto begin_track() const -> hardware::Track* { return this->_begin_track; }
        auto end_bump() const -> hardware::Bump* { return this->_end_bump; }
        virtual auto coords() const -> std::Vector<hardware::Coord> override;
        auto checkNetInfo(std::ofstream& outputfile) -> void override;

    private:
        hardware::Track* _begin_track;
        hardware::Bump* _end_bump;
    };

}