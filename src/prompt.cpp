#include "prompt.h"
#include <iostream>

namespace prompt {
std::string get_name(std::string_view prompt) {
  std::string in;
  std::cout << prompt;
  std::getline(std::cin, in);

  return in;
}
std::string get_name(std::string_view prompt, std::regex name_constraint) {

  std::string in;
  do {

    std::cout << prompt;
    std::getline(std::cin, in);

    if (std::regex_match(in.begin(), in.end(), name_constraint)) {
      std::cout << "That's not a directory name.\n";
      continue;
    }

    return in;

  } while (true);
}
std::pair<int, bool> parse_int(std::string_view sv) {
  std::stringstream ss;
  int out;

  if (!(ss << sv) || !(ss >> out)) {
    return {out, false};
  }

  return {out, true};
}
size_t get_option(std::string_view prompt, size_t max_opts) {

  std::string in;
  do {

    std::cout << prompt;
    std::getline(std::cin, in);

    auto [o, status] = parse_int(in);
    size_t opt = static_cast<size_t>(o);

    if (!status) {
      std::cout << "Failed to parse your input. Try again.\n";
      continue;
    }

    if (opt > max_opts || opt < 1) {
      std::cout << "Your choice is impossible. Try again.\n";
      continue;
    }

    return opt;

  } while (true);
}
} // namespace prompt
