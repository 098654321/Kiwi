#include "./tbsnet.hh"
#include "../../../hardware/node/bump.hh"
#include "../../../algo/router/routestrategy.hh"
#include "../../../hardware/node/trackcoord.hh"


namespace kiwi::circuit {

    TrackToBumpsNet::TrackToBumpsNet(hardware::Track* begin_track, std::Vector<hardware::Bump*> end_bumps) :
        _begin_track{begin_track},
        _end_bumps{std::move(end_bumps)},
        Net(SingleNETtype::TBsnet)
    {
    }

    TrackToBumpsNet::~TrackToBumpsNet() noexcept {

    }

    auto TrackToBumpsNet::update_tob_postion(hardware::TOB* prev_tob, hardware::TOB* next_tob) -> void {
        for (auto& bump : this->_end_bumps) {
            bump = hardware::Bump::update_bump(bump, prev_tob, next_tob);
        }
    }

    auto TrackToBumpsNet::route(hardware::Interposer* interposer, const algo::RouteStrategy& strategy) -> void {
        return strategy.route_track_to_bumps_net(interposer, this);
    }

    auto TrackToBumpsNet::priority() const -> Priority {
        return {1};
    }

    auto TrackToBumpsNet::coords() const -> std::Vector<hardware::Coord> {
        auto coords = std::Vector<hardware::Coord>{};
        for (auto bump : this->end_bumps()) {
            coords.emplace_back(bump->coord());
        }
        coords.emplace_back(this->begin_track()->coord());
        return coords;
    }

    auto TrackToBumpsNet::checkNetInfo(std::ofstream& outputfile) -> void
    {
        outputfile << "Net type: TrackToBumpsNet" << std::endl;
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
        outputfile << "End bumps: " << std::endl;
        for (auto& endB: _end_bumps)
        {
            outputfile << "(" << endB->coord().row << ", " << endB->coord().col << ")" << ", index=" << endB->index() << std::endl;
        }
        outputfile << std::endl;
    }

}
