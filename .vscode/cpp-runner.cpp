#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <chrono>

inline bool is_exists(const std::string& filename) {
    struct stat buffer;
    return (stat(filename.c_str(), &buffer) == 0);
}

inline int execute(const char* command) {
    int ret = system(command);
    if (WIFEXITED(ret))
        return WEXITSTATUS(ret);
    return -1;
}

int build(const std::string& filename, const std::string& filename_woext, const std::string& args) {
    std::string fileWithoutExt = filename;
    
    struct stat source, binary;
    stat(filename.c_str(), &source);
    bool exists = (stat(filename_woext.c_str(), &binary) == 0);
    if (exists && source.st_mtime <= binary.st_ctime) {
        return 0;
    }
    std::string command = "g++ " + args + ' ' + filename;
    return execute(command.c_str());
}

int run(const std::string& filename, const std::string& args) {
    std::string command = filename + ' ' + args;
    return execute(command.c_str());
}

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "Usage: cpp-runner <filename> '<building-arguments>' '<running-arguments>'" << std::endl;
        return 0;
    }
    std::string filename = static_cast<std::string>(argv[1]);
    if (!is_exists(filename)) {
    	std::cerr << "File " << filename << " does not exists" << std::endl;
    	return 1;
    }

    std::string build_args = static_cast<std::string>(argv[2]);
    std::string running_args = static_cast<std::string>(argv[3]);
    std::string filename_without_ext = filename;
    while (filename_without_ext.back() != '.') {
        filename_without_ext.pop_back();
    }
    filename_without_ext.pop_back();

    int buildExitCode = build(filename, filename_without_ext, build_args);
    if (buildExitCode != 0) {
    	std::cerr << "Building failed with code " << buildExitCode << "(0x" << std::hex << buildExitCode << ")" << std::endl;
    	return 1;
    }
    auto startTime = std::chrono::high_resolution_clock::now();
    int runExitCode = run(filename_without_ext, running_args);
    auto endTime = std::chrono::high_resolution_clock::now();
    std::cerr << std::endl;
    std::cerr << std::endl << "Process returned " << runExitCode << "(0x" << std::hex << runExitCode << "). "
        << "Execution time: " << std::dec << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() / 1000.0
        << " s" << std::endl;

    return 0;
}
