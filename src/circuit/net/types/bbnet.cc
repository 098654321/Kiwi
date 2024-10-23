#include "./bbnet.hh"
#include "../../../hardware/node/bump.hh"
#include "../../../algo/router/routestrategy.hh"

namespace kiwi::circuit {

    BumpToBumpNet::BumpToBumpNet(hardware::Bump* begin_bump, hardware::Bump* end_bump) :
        _begin_bump{begin_bump},
        _end_bump{end_bump},
        Net(SingleNETtype::BBnet)
    {
    }

    BumpToBumpNet::~BumpToBumpNet() noexcept {
    }

    auto BumpToBumpNet::update_tob_postion(hardware::TOB* prev_tob, hardware::TOB* next_tob) -> void {
        this->_begin_bump = hardware::Bump::update_bump(this->_begin_bump, prev_tob, next_tob);
        this->_end_bump = hardware::Bump::update_bump(this->_end_bump, prev_tob, next_tob);
    }

    auto BumpToBumpNet::route(hardware::Interposer* interposer, const algo::RouteStrategy& strategy) -> void {
        return strategy.route_bump_to_bump_net(interposer, this);
    }

    auto BumpToBumpNet::priority() const -> Priority {
        return {2};
    }

    auto BumpToBumpNet::coords() const -> std::Vector<hardware::Coord> {
        return std::Vector<hardware::Coord>{this->begin_bump()->coord(), this->end_bump()->coord()};
    }

    auto BumpToBumpNet::checkNetInfo(ofstream& outputfile) -> void 
    {
        outputfile << "Net type: BumpToBumpNet" << std::endl;
        outputfile << "Begin bump: " << std::endl;
        outputfile << "(" << _begin_bump->coord().row << ", " << _begin_bump->coord().col << ")" << ", index=" << _begin_bump->index() << std::endl;
        outputfile << "End bump: " << std::endl;
        outputfile << "(" << _end_bump->coord().row << ", " << _end_bump->coord().col << ")" << ", index=" << _end_bump->index() << std::endl;
    }

}
