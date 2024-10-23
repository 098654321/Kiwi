#include <hardware/cob/cobdirection.hh>
#include <hardware/cob/cob.hh>
#include <global/std/integer.hh>
#include <global/std/utility.hh>
#include <global/std/string.hh>

#include <iostream>
#include <ostream>

using namespace kiwi::hardware;

auto str_to_dir(std::StringView str) -> std::Option<COBDirection> {
    if (str == "left") return { COBDirection::Left };
    if (str == "l") return { COBDirection::Left };
    if (str == "right") return { COBDirection::Right };
    if (str == "r") return { COBDirection::Right };
    if (str == "up") return { COBDirection::Up };
    if (str == "u") return { COBDirection::Up };
    if (str == "down") return { COBDirection::Down };
    if (str == "d") return { COBDirection::Down };
    return std::nullopt;
}

auto ensure_str_to_dir(std::StringView str) -> COBDirection {
    auto opt_dir = str_to_dir(str);
    if (!opt_dir.has_value()) {
        std::cout << "Error: '" << str << "' is not a valid direction literal!" << std::endl;
        std::exit(-1);
    }
    return *opt_dir;
}

auto ensure_str_to_usize(std::StringView str) -> std::usize {
    try {
        return std::stoll(str.data());
    } catch (const std::exception& err) {
        std::cout << "Error: parse number failed, '" << err.what() << '\'' << std::endl;
        std::exit(-1);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 5) {
        std::cout << "Usage: cob_map <cob_index/track_index> <from_dir> <from_index> <to_dir>" << std::endl;
        std::exit(-1);
    }

    auto command = std::StringView{argv[1]};
    auto from_dir_str = std::StringView{argv[2]};
    auto from_index_str = std::StringView{argv[3]};
    auto to_dir_str = std::StringView{argv[4]};

    auto from_dir = ensure_str_to_dir(from_dir_str);
    auto from_index = ensure_str_to_usize(from_index_str);
    auto to_dir = ensure_str_to_dir(to_dir_str);

    std::usize to_index = 0;
    if (command == "cob_index") {
        to_index = COB::cob_index_map(from_dir, from_index, to_dir);   
    } else if (command == "tracl_index") {
        to_index = COB::track_index_map(from_dir, from_index, to_dir);   
    } else {
        std::cout << "Error: unkown command '" << command << "'" << std::endl;
    }

    std::cout << "to_index = " << to_index << std::endl;

    return 0;
}