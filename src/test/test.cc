#include "global/std/string.hh"
#include <global/std/collection.hh>
#include <global/debug/console.hh>
#include <cassert>
#include <cstring>
#include <string.h>

using TestFunction = void(*)(void);

extern void test_cob_main();
extern void test_interposer_main();
extern void test_tob_main();
extern void test_router_main();
extern void test_placer_main();
extern void test_debug_main();

#define REGISTER_TEST(test_name)\
functions.emplace(#test_name, & test_##test_name##_main);\
if (strcmp(argv[1], #test_name) == 0) {\
    test_##test_name##_main();\
    return 0;\
}\

int main(int argc, char** argv) {
    auto functions = std::HashMap<std::StringView, TestFunction>{};

    assert(argc == 2);
    REGISTER_TEST(cob)
    REGISTER_TEST(tob)
    REGISTER_TEST(interposer)
    REGISTER_TEST(router)
    REGISTER_TEST(placer)
    REGISTER_TEST(debug)

    if (strcmp(argv[1], "all") == 0) {
        for (auto [test_name, test_func] : functions) {
            kiwi::console::println_fmt("Run test '{}'", test_name);
            test_func();
            kiwi::console::println("");
        }
    }
    
    return 0;
}