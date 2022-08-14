#include <iostream>

namespace commands {
    int VersionCommand(int argc, char *argv[]){
        std::cout << "LSAM version 1.0.2" << std::endl;
        return 0;
    };
}
