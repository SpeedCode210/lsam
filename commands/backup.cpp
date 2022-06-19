#include "backup.hpp"
#include "../Application.hpp"
#include "../consts.h"
#include <string>
#include <iostream>
#include <unistd.h>
#include <ctime>
#include <filesystem>

namespace commands {
    int BackupCommand(int argc, char *argv[]) {

        if (argc <= 2) {
            std::cerr << "Please enter a valid application name/service_id." << std::endl;
            return 1;
        }

        auto apps = Application::GetApps();

        Application *app = nullptr;

        for (int i = 0; i < apps.size(); i++) {
            if (apps[i].DisplayName == argv[2] || apps[i].ServiceName == argv[2]) {
                app = &(apps[i]);
                break;
            }
        }

        if (app == nullptr) {
            std::cerr << "The entered application doesn't exist." << std::endl;
            return 1;
        }


        std::cout << "The application '" << app->DisplayName << "' has been backed in the file :" << std::endl
                  << CreateBackup(*app) << std::endl;

        return 0;
    }

    std::string CreateBackup(Application app) {
        //Get Date and time
        std::time_t t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::tm *timeinfo;
        char datetime[80];
        std::time(&t);
        timeinfo = std::localtime(&t);
        std::strftime(datetime, 80, "%Y-%m-%d-%H:%M", timeinfo);

        if (!std::filesystem::exists("/home/" + USERNAME + "/backups"))
            system(("mkdir /home/" + USERNAME + "/backups").c_str());

        std::string filePath = "/home/" + USERNAME + "/backups/" + app.ServiceName + "-" + datetime + ".tar.gz";

        system(("tar -czf " + filePath + " --directory=" + app.Directory.substr(0, app.Directory.find_last_of('/')) +
                " " + app.Directory.substr(app.Directory.find_last_of('/') + 1, app.Directory.size())).c_str());

        return filePath;
    }
}