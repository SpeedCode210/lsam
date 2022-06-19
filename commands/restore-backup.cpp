#include "restore-backup.hpp"
#include <unistd.h>
#include <iostream>
#include <filesystem>
#include <regex>
#include "../Application.hpp"
#include "backup.hpp"
#include "../consts.h"

namespace commands {
    int RestoreBackupCommand(int argc, char *argv[]) {

        if (argc <= 2) {
            std::cerr << "Please enter a valid backup name." << std::endl;
            return 1;
        }

        if (!std::filesystem::exists("/home/" + USERNAME + "/backups/" + argv[2])) {
            std::cerr << "The backup doesn't exist" << std::endl;
            return 1;
        }

        std::string name(argv[2]);

        std::smatch nameMatchs;
        if (!(std::regex_search(name, nameMatchs, std::regex(".+?(?=-[\\d])")))) {
            std::cerr << "The backup's application doesn't exist" << std::endl;
            return 1;
        }

        auto apps = Application::GetApps();

        Application *app = nullptr;

        int i;
        for (i = 0; i < apps.size(); i++) {
            if (apps[i].ServiceName == nameMatchs[0]) {
                app = &(apps[i]);
                break;
            }
        }

        if (app == nullptr) {
            std::cerr << "The entered application doesn't exist." << std::endl;
            return 1;
        }


        std::cout
                << "A backup of the application before restoring has been saved in "
                << commands::CreateBackup(*app)
                << std::endl;

        system(("rm -rf " + app->Directory).c_str());
        system(("tar -xf /home/" + USERNAME + "/backups/" + name + " -C /home/" + USERNAME + "/applications").c_str());

        return 0;
    }
}