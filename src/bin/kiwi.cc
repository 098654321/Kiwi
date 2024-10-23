#include <global/debug/console.hh>
#include <global/debug/log.hh>
#include <global/debug/debug.hh>
#include <global/std/format.hh>

using namespace kiwi;

int main(int argc, char** argv) {

    debug::debug_fmt("Hello {}!", "kiwi");

    return 0;
}
