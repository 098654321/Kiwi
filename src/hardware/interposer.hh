#pragma once

#include "./cob/cobcoord.hh"
#include "./cob/cob.hh"

#include "./tob/tobcoord.hh"
#include "./tob/tob.hh"

#include "./node/track.hh"
#include "./node/bump.hh"
#include "../global/std/integer.hh"

#include "../global/std/utility.hh"
#include "../global/std/collection.hh"

namespace kiwi::hardware {

    class Interposer {
    public:
        enum {
            COB_ARRAY_WIDTH   = 13,
            COB_ARRAY_HEIGHT  = 8,

            TOB_ARRAY_WIDTH   = 4,
            TOB_ARRAY_HEIGHT  = 4,
        };

    public:
        Interposer();

    public:
        auto available_tracks(Bump* bump, TOBSignalDirection dir) -> std::HashMap<Track*, TOBConnector>;
        auto available_tracks_bump_to_track(Bump* bump) -> std::HashMap<Track*, TOBConnector>;
        auto available_tracks_track_to_bump(Bump* bump) -> std::HashMap<Track*, TOBConnector>;

    public:
        auto adjacent_idle_tracks(Track* track) -> std::Vector<std::Tuple<Track*, COBConnector>>;
        auto adjacent_tracks(Track* track) -> std::Vector<std::Tuple<Track*, COBConnector>>;

    public:
        auto is_idle_tracks(Track* track) -> bool;

    public:
        auto get_cob(const COBCoord& coord) -> std::Option<COB*>;
        auto get_cob(std::i64 row, std::i64 col) -> std::Option<COB*>;
         
        auto get_tob(const TOBCoord& coord) -> std::Option<TOB*>;
        auto get_tob(std::i64 row, std::i64 col) -> std::Option<TOB*>;

        auto get_track(const TrackCoord& coord) -> Track*;
        auto get_track(std::i64 row, std::i64 col, TrackDirection dir, std::usize index) -> Track*;

        auto get_bump(const TOBCoord& coord, std::usize index) -> Bump*;
        auto get_bump(std::i64 row, std::i64 col, std::usize index) -> Bump*;

        auto get_a_idle_tob() -> std::Option<TOB*>;
    
    private:
        std::HashMap<COBCoord, COB> _cobs;
        std::HashMap<TOBCoord, TOB> _tobs;

        std::HashMap<TrackCoord, Track> _tracks;    //用到的时候再创建

        // Channel coord
        std::HashMap<TOBCoord, std::Tuple<std::i64, std::i64, TrackDirection>> _tob_postion_map;
    };

}