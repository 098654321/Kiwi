#include "./tsbsnet.hh"
#include "../../../hardware/node/bump.hh"
#include "../../../algo/router/routestrategy.hh"
#include "../../../hardware/node/trackcoord.hh"


namespace kiwi::circuit {

    TracksToBumpsNet::TracksToBumpsNet(std::Vector<hardware::Track*>& begin_tracks, std::Vector<hardware::Bump*> end_bumps):
    Net(SingleNETtype::TsBsnet), _begin_tracks(std::move(begin_tracks)), _end_bumps(std::move(end_bumps))
    {}

    TracksToBumpsNet::~TracksToBumpsNet() {}

    auto TracksToBumpsNet::update_tob_postion(hardware::TOB* prev_tob, hardware::TOB* next_tob) -> void {
        for (auto& bump : this->_end_bumps) {
            bump = hardware::Bump::update_bump(bump, prev_tob, next_tob);
        }
    }

    auto TracksToBumpsNet::route(hardware::Interposer* interposer, const algo::RouteStrategy& strategy) -> void {
        return strategy.route_tracks_to_bumps_net(interposer, this);
    }

    auto TracksToBumpsNet::priority() const -> Priority {
        return {3};
    }

    auto TracksToBumpsNet::coords() const -> std::Vector<hardware::Coord> {
        auto coords = std::Vector<hardware::Coord>{};
        for (auto track : this->begin_tracks()) {
            coords.emplace_back(track->coord());
        }
        for (auto bump : this->end_bumps()) {
            coords.emplace_back(bump->coord());
        }
        return coords;
    }

    auto TracksToBumpsNet::checkNetInfo(std::ofstream& outputfile) -> void
    {
        outputfile << "Net type: TracksToBumpsNet" << std::endl;
        outputfile << "Begin tracks: ";
        for(auto& beginT: _begin_tracks)
        {
            std::string dir;
            switch(beginT->coord().dir)
            {
                case hardware::TrackDirection::Horizontal:
                    dir = "H";
                    break;
                case hardware::TrackDirection::Vertical:
                    dir = "V";
                    break;
            }
            outputfile << dir << "(" << beginT->coord().row << ", " << beginT->coord().col << ")" << ", index=" << beginT->coord().index << std::endl;
        }
        outputfile << "End bumps: " << std::endl;
        for (auto& endB: _end_bumps)
        {
            outputfile << "(" << endB->coord().row << ", " << endB->coord().col << ")" << ", index=" << endB->index() << std::endl;
        }
        outputfile << std::endl;
    }

}
