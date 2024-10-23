#include "./tbnet.hh"
#include "../../../hardware/node/bump.hh"
#include "../../../hardware/coord.hh"
#include "../../../algo/router/routestrategy.hh"
#include "../../../hardware/node/track.hh"
#include "../../../hardware/tob/tob.hh"
#include "../../../hardware/interposer.hh"

namespace kiwi::circuit {

    TrackToBumpNet::TrackToBumpNet(hardware::Track* begin_track, hardware::Bump* end_bump) :
        _begin_track{begin_track},
        _end_bump{end_bump},
        Net(SingleNETtype::TBnet)
    {
    }

    TrackToBumpNet::~TrackToBumpNet() noexcept {
    }

    auto TrackToBumpNet::update_tob_postion(hardware::TOB* prev_tob, hardware::TOB* next_tob) -> void {
        this->_end_bump = hardware::Bump::update_bump(this->_end_bump, prev_tob, next_tob);
    }

    auto TrackToBumpNet::route(hardware::Interposer* interposer, const algo::RouteStrategy& strategy) -> void {
        return strategy.route_track_to_bump_net(interposer, this);
    }

    auto TrackToBumpNet::priority() const -> Priority {
        return {2};
    }

    auto TrackToBumpNet::coords() const -> std::Vector<hardware::Coord> {
        return std::Vector<hardware::Coord>{this->begin_track()->coord(), this->end_bump()->coord()};
    }

    auto TrackToBumpNet::checkNetInfo(std::ofstream& outputfile) -> void
    {
        outputfile << "Net type: TrackToBumpNet" << std::endl;
        outputfile << "Begin track: " << std::endl;
        std::string dir;
        switch(_begin_track->coord().dir)
        {
            case hardware::TrackDirection::Horizontal:
                dir = "H";
                break;
            case hardware::TrackDirection::Vertical:
                dir = "V";
                break;
        }
        outputfile << dir << "(" << _begin_track->coord().row << ", " << _begin_track->coord().col << ")" << ", index=" << _begin_track->coord().index << std::endl;
        outputfile << "End bump: " << std::endl;
        outputfile << "(" << _end_bump->coord().row << ", " << _end_bump->coord().col << ")" << ", index=" << _end_bump->index() << std::endl;
    }

}
