#include "autostart.hpp"
#include <string.h>
#include <unistd.h>
#include <iostream>
#include "../Application.hpp"


namespace commands {
    int AutostartCommand(int argc, char *argv[]) {

        if (argc <= 3) {
            std::cerr << "Please enter a valid application name/service_id." << std::endl;
            return 1;
        }

        if (strcmp(argv[2], "on") != 0 && strcmp(argv[2], "off") != 0) {
            std::cerr << "Invalid parameter entered ('" << argv[2] << "')." << std::endl;
            return 1;
        }

        auto apps = Application::GetApps();

        Application *app = nullptr;

        for (int i = 0; i < apps.size(); i++) {
            if (apps[i].DisplayName == argv[3] || apps[i].ServiceName == argv[3]) {
                app = &(apps[i]);
                break;
            }
        }

        if (app == nullptr) {
            std::cerr << "The entered application doesn't exist." << std::endl;
            return 1;
        }

        using namespace std;

        system(("systemctl "s + (strcmp(argv[2], "on") == 0 ? "enable" : "disable") + " " +
                (*app).ServiceName).c_str());

        return 0;
    }
}