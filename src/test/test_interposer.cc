#include "global/std/collection.hh"
#include "hardware/tob/tob.hh"
#include "test/utilty.hh"
#include "hardware/node/track.hh"
#include "hardware/interposer.hh"


using namespace kiwi::hardware;

void test_adjacent_tracks() {
    auto i = Interposer{};

    auto track1 = i.get_track(TrackCoord{3, 3, TrackDirection::Horizontal, 12});
    ASSERT_EQ(i.adjacent_tracks(track1).size(), 6);

    auto track2 = i.get_track(TrackCoord{0, 3, TrackDirection::Vertical, 12});
    ASSERT_EQ(i.adjacent_tracks(track2).size(), 3);
}

void test_adjacent_idle_tracks1() {
    auto i = Interposer{};

    auto track1 = i.get_track(0, 0, TrackDirection::Horizontal, 0);
    auto res1 = i.adjacent_idle_tracks(track1);
    auto tracks1 = std::HashSet<Track*>{};

    for (auto [t, c] : res1) {
        tracks1.emplace(t);
    }

    ASSERT_EQ(res1.size(), 3);
    ASSERT_EQ(tracks1.size(), 3);

    ASSERT(tracks1.find(i.get_track(0, 1, TrackDirection::Horizontal, 0)) != tracks1.end());
    ASSERT(tracks1.find(i.get_track(0, 0, TrackDirection::Vertical, 8)) != tracks1.end());
    ASSERT(tracks1.find(i.get_track(1, 0, TrackDirection::Vertical, 48)) != tracks1.end());

    /////////////////////////////////////////////////////

    auto track2 = i.get_track(0, 1, TrackDirection::Horizontal, 0);
    auto res2 = i.adjacent_idle_tracks(track2);
    auto tracks2 = std::HashSet<Track*>{};

    for (auto [t, c] : res2) {
        tracks2.emplace(t);
    }

    ASSERT_EQ(res2.size(), 6);
    ASSERT_EQ(tracks2.size(), 6);

    ASSERT(tracks2.find(i.get_track(0, 0, TrackDirection::Vertical, 48)) != tracks2.end());
}

void test_adjacent_idle_tracks2() {
    auto i = Interposer{};

    auto track = i.get_track(2, 2, TrackDirection::Horizontal, 0);
    auto res1 = i.adjacent_idle_tracks(track);
    ASSERT_EQ(res1.size(), 6);

    std::get<1>(res1[0]).connect();
    std::get<1>(res1[1]).connect();

    auto res2 = i.adjacent_idle_tracks(track);
    ASSERT_EQ(res2.size(), 4);
}

void test_available_tracks() {
    auto i = Interposer{};

    auto bump = i.get_bump(0, 0, 23);
    auto tracks = i.available_tracks(bump, TOBSignalDirection::BumpToTrack);
    ASSERT_EQ(tracks.size(), 128);

    tracks.begin()->second.connect();

    tracks = i.available_tracks(bump, TOBSignalDirection::BumpToTrack);
    ASSERT_NE(tracks.size(), 128);
}

void test_get_bump() {
    auto i = Interposer{};
    auto bump = i.get_bump(0, 0, 23);
    ASSERT_EQ(bump->coord().row, 1);
    ASSERT_EQ(bump->coord().col, 0);
    ASSERT_EQ(bump->index(), 23);
    ASSERT_EQ(bump->tob()->coord().row, 0);
    ASSERT_EQ(bump->tob()->coord().col, 0);

    bump = i.get_bump(2, 2, 90);
    ASSERT_EQ(bump->coord().row, 5);
    ASSERT_EQ(bump->coord().col, 6);
    ASSERT_EQ(bump->index(), 90);
    ASSERT_EQ(bump->tob()->coord().row, 2);
    ASSERT_EQ(bump->tob()->coord().col, 2);

    bump = i.get_bump(3, 1, 34);
    ASSERT_EQ(bump->coord().row, 7);
    ASSERT_EQ(bump->coord().col, 3);
    ASSERT_EQ(bump->index(), 34);
    ASSERT_EQ(bump->tob()->coord().row, 3);
    ASSERT_EQ(bump->tob()->coord().col, 1);
}

int test_interposer_main() {

    test_adjacent_tracks();
    test_adjacent_idle_tracks1();
    test_adjacent_idle_tracks2();    
    test_available_tracks();
    test_get_bump();

    return 0;
}