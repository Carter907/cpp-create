#include <concepts>
#include <filesystem>
#include <format>
#include <iostream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

// static on global variables makes their linkage internal
static const std::string PROJECTS_DIR{"/home/carters/Code/cpp"};
static const std::string TEMPLATES_DIR{"/home/carters/Code/cpp/cpp-templs/"};

int main() {

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

  auto i{0u};
  // choose a template
  std::cout << "choose a template:\n";
  for (const fs::path &cpptmpl : templs) {
    i++;
    std::cout << std::format("{0:}-> {1:}\n", i, cpptmpl.string());
  }

  // option syntax

  // copy template to projects + projname
  // std::cout << "project name: ";
  // std::string projname{};
  // std::getline(std::cin, projname);
  //
  // fs::copy_file(TEMPLATES_DIR + file_name, PROJECTS_DIR + projname);
  //
  return 0;
}
