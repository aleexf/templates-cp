#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <chrono>

inline bool isExists(const std::string& filename) {
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
    struct stat source, binary;
    stat(filename.c_str(), &source);
    bool exists = (stat(filename_woext.c_str(), &binary) == 0);
    if (exists && source.st_mtime <= binary.st_ctime) {
        return 0;
    }
    std::cerr << " === Building... === " << std::endl;
    std::string command = "g++ " + args + ' ' + filename;
    return execute(command.c_str());
}

int run(const std::string& filename, const std::string& args) {
    std::string command = filename + ' ' + args;
    return execute(command.c_str());
}

std::string cutExtension(const std::string& filename) {
    std::string without_ext = filename; 
    while (!without_ext.empty() && without_ext.back() != '.') {
        without_ext.pop_back();
    }
    if (!without_ext.empty()) {
        without_ext.pop_back();
    }
    return (!without_ext.empty() ? without_ext : filename);
}

std::string makeExecutable(const std::string& file) {
    if (file[0] == '/' || (file.size() > 1 && file[0] == '.' && file[1] == '/')) {
        return file;
    }
    return "./" + file;
}

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "Usage: cpp-runner <filename> '<building-arguments>' '<running-arguments>'" << std::endl;
        return 0;
    }

    std::string filename = static_cast<std::string>(argv[1]);
    if (!isExists(filename)) {
    	std::cerr << "File " << filename << " does not exists" << std::endl;
    	return 1;
    }

    std::string build_args = static_cast<std::string>(argv[2]);
    std::string running_args = static_cast<std::string>(argv[3]);
    
    std::string without_ext = cutExtension(filename);

    int build_exit_code = build(filename, without_ext, build_args);
    if (build_exit_code != 0) {
    	std::cerr << "Building failed with code " << build_exit_code << "(0x" << std::hex << build_exit_code
    	    << ")" << std::endl;
    	return 1;
    } else {
        std::cerr << " === Build completed successfully === " << std::endl;
    }

    auto start_time = std::chrono::high_resolution_clock::now();
    int run_exit_code = run(makeExecutable(without_ext), running_args);
    auto end_time = std::chrono::high_resolution_clock::now();
    
    std::cerr << std::endl;
    std::cerr << std::endl << "Process returned " << run_exit_code << "(0x" << std::hex << run_exit_code << "). "
        << "Execution time: " << std::dec 
        << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() / 1000.0
        << " s" << std::endl;

    return 0;
}
