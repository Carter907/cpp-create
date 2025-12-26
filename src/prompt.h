#pragma once
#include <regex>
#include <string>
namespace prompt {

std::string get_name(std::string_view prompt);
std::string get_name(std::string_view prompt, std::regex name_constraint);
std::pair<int, bool> parse_int(std::string_view sv);
size_t get_option(std::string_view prompt, size_t max_opts);
} // namespace prompt
