#include "util_env.h"

#if defined(__linux__)
#include <array>
#endif
#include <filesystem>
#include <unistd.h>
#include <limits.h>
#if defined(__APPLE__) || defined(__FreeBSD__)
#include <sys/sysctl.h>
#if defined(__APPLE__)
#define KERN_PROC_PATHNAME KERN_PROCARGS2
#endif
#endif

namespace dxvk::env {

  std::string getExePath() {
    #if defined(__linux__)
    td::array<char, PATH_MAX> exePath = {};


    size_t count = readlink("/proc/self/exe", exePath.data(), exePath.size());

    return std::string(exePath.begin(), exePath.begin() + count);
    #else // FreeBSD & macOS
          // [armaturo] this is stolen from dotsujin/dxvk, dxvk/src/util/util_env.cpp
    int mib[4] = {CTL_KERN, KERN_PROC, KERN_PROC_PATHNAME, getpid()};
    char exePath[PATH_MAX] = {};
    size_t size = PATH_MAX;

    if (sysctl(mib, 4, exePath, &size, NULL, 0) != 0) {
        // throw error here?
        return "";
    }

    return std::string(exePath);
    #endif
  }
  
  
  void setThreadName(const std::string& name) {
  }


  bool createDirectory(const std::string& path) {
    return std::filesystem::create_directories(path);
  }

}
