#include "./bump.hh"
#include "../tob/tob.hh"
#include <cassert>

namespace kiwi::hardware {

    Bump::Bump(Coord const& coord, std::usize const index, TOB* tob) : 
        _coord{coord},
        _tob{tob},
        _index{index},
        _connected_track{nullptr},
        _signal_dir{TOBSignalDirection::DisConnected}
    {
    }

    auto Bump::set_connected_track(Track* track, TOBSignalDirection signal_dir) -> void {
        assert(signal_dir != TOBSignalDirection::DisConnected);

        this->_connected_track = track;
        this->_signal_dir = signal_dir;

        track->set_connected_bump(this, signal_dir);
    }

    auto Bump::update_bump(Bump* bump, TOB* prev_tob, TOB* next_tob) -> Bump* {
        if (bump->_tob == prev_tob) {
            return next_tob->bump(bump->_index);
        } else {
            return bump;
        }
    }

}