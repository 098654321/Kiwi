#pragma once

#include <global/std/string.hh>
#include <global/std/format.hh>

namespace kiwi::debug
{

    enum class DebugLevel {
        Debug,
        Info,
        Warning,
        Error,
        Fatal,
    };

    auto initial_log(std::String log_path) -> void;
    auto set_debug_level(DebugLevel level) -> void;
    auto is_debug_level_enough(DebugLevel level) -> bool;

    auto debug(std::StringView message) -> void;
    auto info(std::StringView message) -> void;
    auto warning(std::StringView message) -> void;
    auto error(std::StringView message) -> void;
    auto fatal(std::StringView message) -> void;

    template <typename... Args>
    auto debug_fmt(std::FormatString<Args...> fmt, Args &&... args) -> void {
        auto str = std::format(fmt, std::forward<Args>(args)...);
        if (is_debug_level_enough(DebugLevel::Debug)) {
            debug(str);
        }
    }

    template <typename... Args>
    auto info_fmt(std::FormatString<Args...> fmt, Args &&... args) -> void {
        auto str = std::format(fmt, std::forward<Args>(args)...);
        if (is_debug_level_enough(DebugLevel::Info)) {
            info(str);
        }
    }

    template <typename... Args>
    auto warning_fmt(std::FormatString<Args...> fmt, Args &&... args) -> void {
        auto str = std::format(fmt, std::forward<Args>(args)...);
        if (is_debug_level_enough(DebugLevel::Warning)) {
            warning(str);
        }
    }

    template <typename... Args>
    auto error_fmt(std::FormatString<Args...> fmt, Args &&... args) -> void {
        auto str = std::format(fmt, std::forward<Args>(args)...);
        if (is_debug_level_enough(DebugLevel::Error)) {
            error(str);
        }
    }

    template <typename... Args>
    auto fatal_fmt(std::FormatString<Args...> fmt, Args &&... args) -> void {
        auto str = std::format(fmt, std::forward<Args>(args)...);
        if (is_debug_level_enough(DebugLevel::Fatal)) {
            fatal(str);
        }
    }

}