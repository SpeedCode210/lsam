#include "delete-backup.hpp"
#include <unistd.h>
#include <iostream>
#include <filesystem>
#include "../consts.h"

namespace commands {
    int DeleteBackupCommand(int argc, char *argv[]) {

        if (argc <= 2) {
            std::cerr << "Please enter a valid backup name." << std::endl;
            return 1;
        }

        if (!std::filesystem::exists("/home/" + USERNAME + "/backups/" + argv[2])) {
            std::cerr << "The backup doesn't exist" << std::endl;
            return 1;
        }

        std::cout << "Do you really want to delete the backup ? [y/N] ";
        std::string r;
        std::cin >> r;

        if (r != "y" && r != "Y") {
            return 0;
        }

        system(("rm -rf /home/" + USERNAME + "/backups/" + argv[2]).c_str());

        return 0;

    }
}