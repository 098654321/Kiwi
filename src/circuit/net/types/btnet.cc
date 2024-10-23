#include "./btnet.hh"
#include "../../../hardware/node/bump.hh"
#include "../../../algo/router/routestrategy.hh"
#include "../../../hardware/node/trackcoord.hh"

namespace kiwi::circuit {

    BumpToTrackNet::BumpToTrackNet(hardware::Bump* begin_bump, hardware::Track* end_track) :
        _begin_bump{begin_bump},
        _end_track{end_track},
        Net(SingleNETtype::Btnet)
    {
    }

    BumpToTrackNet::~BumpToTrackNet() noexcept {
    }

    auto BumpToTrackNet::update_tob_postion(hardware::TOB* prev_tob, hardware::TOB* next_tob) -> void {
        this->_begin_bump = hardware::Bump::update_bump(this->_begin_bump, prev_tob, next_tob);
    }

    auto BumpToTrackNet::route(hardware::Interposer* interposer, const algo::RouteStrategy& strategy) -> void {
        return strategy.route_bump_to_track_net(interposer, this);
    }

    auto BumpToTrackNet::priority() const -> Priority {
        return {2};
    }

    auto BumpToTrackNet::coords() const -> std::Vector<hardware::Coord> {
        return std::Vector<hardware::Coord>{this->begin_bump()->coord(), this->end_track()->coord()};
    }

    auto BumpToTrackNet::checkNetInfo(std::ofstream& outputfile) -> void
    {
        outputfile << "Net type: BumpToTrackNet" << std::endl;
        outputfile << "Begin bump: " << std::endl;
        outputfile << "(" << _begin_bump->coord().row << ", " << _begin_bump->coord().col << ")" << ", index=" << _begin_bump->index() << std::endl;
        outputfile << "End track: " << std::endl;
        std::string dir;
        switch(_end_track->coord().dir)
        {
            case hardware::TrackDirection::Horizontal:
                dir = "H";
                break;
            case hardware::TrackDirection::Vertical:
                dir = "V";
                break;
        }
        outputfile << dir << "(" << _end_track->coord().row << ", " << _end_track->coord().col << ")" << ", index=" << _end_track->coord().index << std::endl;
    }

}
