#include <filesystem>
#include <format>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

// use C libraries for environment variables and execution
#include <cstdlib>

namespace fs = std::filesystem;

// static on global variables makes their linkage internal

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

std::string get_name(std::string_view prompt) {
  std::string in;
  std::cout << prompt;
  std::getline(std::cin, in);

  return in;
}

int main() {

  fs::path PROJECTS_DIR{};
  fs::path TEMPLATES_DIR{};

  if (const char *proj_dir_env = std::getenv("PROJECTS_DIR")) {
    PROJECTS_DIR = proj_dir_env;
  } else {
    std::cerr << "Please specify your project directory as the PROJECTS_DIR "
                 "environment variable\n";
    return 1;
  }
  if (const char *templs_dir_env = std::getenv("TEMPLATES_DIR")) {
    TEMPLATES_DIR = templs_dir_env;
  } else {
    std::cerr << "Please specify your template directory as the TEMPLATES_DIR "
                 "environment variable\n";
    return 1;
  }
  const fs::path curr{fs::current_path()};
  if (curr != PROJECTS_DIR) {
    PROJECTS_DIR = curr;
    PROJECTS_DIR += '/';
  }

  std::cout << "Project directory: " << PROJECTS_DIR << '\n';
  std::cout << "Templates directory: " << TEMPLATES_DIR << '\n';

  std::vector<fs::path> templs{}; // default list initialized
  if (fs::exists(TEMPLATES_DIR) && fs::is_directory(TEMPLATES_DIR)) {
    for (const auto &e : fs::directory_iterator{TEMPLATES_DIR}) {
      fs::path file_name{e.path().filename()};
      bool is_hidden{file_name.string().starts_with('.')};
      if (e.is_directory() && !is_hidden) {
        templs.push_back(file_name);
      }
    }
  }
  // choose a template
  std::cout << "C++ templates:\n";
  for (auto i{0u}; i < templs.size(); i++) {
    std::cout << std::format("{}-> {}\n", i + 1, templs[i].string());
  }
  // choose template
  int opt = get_option("option: ", templs.size());

  fs::path templ_name{templs[opt - 1]};
  fs::path chosen_templ{TEMPLATES_DIR};
  chosen_templ += templ_name;

  fs::path name = get_name("name: ");
  fs::path project_dir{PROJECTS_DIR};
  project_dir += name;

  // recursive copy
  fs::copy(chosen_templ, project_dir, fs::copy_options::recursive);
  // for (const auto &dir : fs::recursive_directory_iterator{chosen_templ}) {
  //   fs::path mirrored_dir{project_dir};
  //   bool sub_dir{false};
  //   for (const auto &ele : dir.path()) {
  //     if (sub_dir) {
  //
  //       mirrored_dir += '/';
  //       mirrored_dir += ele;
  //     }
  //     if (ele == templ_name) {
  //       sub_dir = true;
  //     }
  //   }
  //
  //   fs::copy(dir, mirrored_dir);
  // }

  return 0;
}
