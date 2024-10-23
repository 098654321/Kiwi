#pragma once

#include <fstream>
#include "../hardware/interposer.hh"
#include "../global/std/memory.hh"
#include "../circuit/basedie.hh"
#include "../circuit/net/net.hh"
#include "./register.hh"
#include "../circuit/net/nets.hh"


namespace kiwi::circuit
{
  class BumpToBumpNet;
  class BumpToBumpsNet;
  class BumpToBumpSyncNet;
  class BumpToTrackNet;
  class BumpToTracksNet;
  class BumpToTrackSyncNet;
  class TrackToBumpNet;
  class TrackToBumpsNet;
  class TrackToBumpSyncNet;
  class TracksToBumpsNet;
}


namespace kiwi::parser{

using namespace std;
using namespace kiwi::hardware;
using namespace kiwi::circuit;

class Parser {
 public:
  auto inputParse(const String&, BaseDie&, Interposer&) -> void;
  auto output(
    const String& filename, Interposer& interposer, const std::Array<const String, 10> cobRegName,
    const std::Array<const String, 6> tobRegName
    ) -> void;

 private:
  std::Rc<Interposer> _pInterposer;
  std::Rc<BaseDie> _pBaseDie;
  static const int ExtIOIndex[];
  Register _register;
};

}
