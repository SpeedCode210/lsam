#include "stop.hpp"
#include <unistd.h>
#include <iostream>
#include "../Application.hpp"

namespace commands {
    int StopCommand(int argc, char *argv[]) {

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

        system(("systemctl stop " + (*app).ServiceName).c_str());

        return 0;
    }
}