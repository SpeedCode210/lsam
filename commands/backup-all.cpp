#include "backup-all.hpp"
#include "../Application.hpp"
#include "../consts.h"
#include <string>
#include <iostream>
#include <unistd.h>
#include <ctime>
#include <filesystem>

namespace commands {
    int BackupAllCommand(int argc, char *argv[]) {

        auto apps = Application::GetApps();


        for (int i = 0; i < apps.size(); i++) {
            auto app = apps[i];

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

            system(("tar -czf " + filePath + " --directory=" +
                    app.Directory.substr(0, app.Directory.find_last_of('/')) + " " +
                    app.Directory.substr(app.Directory.find_last_of('/') + 1, app.Directory.size())).c_str());

            std::cout << "The application '" << app.DisplayName << "' has been backed in the file :" << std::endl
                      << filePath << std::endl;
        }

        return 0;
    }
}