#pragma once

#include <string>
#include <fstream>

namespace tod::sugar {

    bool slurp_bin(std::string& out, std::string const& file_path)
    {
        try {
            std::ifstream file(file_path, std::ios_base::binary);
            out.assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
            out.append("\0");
            return true;
        }
        catch (...) {
            std::cout << "ERROR: Could not read " << file_path << std::endl;
        }

        return false;
    }

}