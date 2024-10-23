#include "./debug.hh"
#include <global/std/integer.hh>
#include <global/debug/console.hh>
#include <global/debug/log.hh>

namespace kiwi::debug {

    static auto debug_level = DebugLevel::Debug;

    static auto debug_level_to_number(DebugLevel level) -> std::i64 {
        switch (level) {
            case DebugLevel::Debug:   return 0;
            case DebugLevel::Info:    return 1;
            case DebugLevel::Warning: return 2;
            case DebugLevel::Error:   return 3;
            case DebugLevel::Fatal:   return 4;
        }
        return -1;
    }

    auto initial_log(std::String log_path) -> void {
        log::initial(log_path);
    }

    auto set_debug_level(DebugLevel level) -> void {
        debug_level = level;
    }

    auto is_debug_level_enough(DebugLevel level) -> bool {
        return level >= debug_level;
    }

    auto debug(std::StringView message) -> void {
        console::debug(message);
        log::debug(message);
    }

    auto info(std::StringView message) -> void {
        console::info(message);
        log::info(message);
    }

    auto warning(std::StringView message) -> void {
        console::warning(message);
        log::warning(message);
    }

    auto error(std::StringView message) -> void {
        console::error(message);
        log::error(message);
    }

    auto fatal(std::StringView message) -> void {
        console::fatal(message);
        log::fatal(message);
    }

}