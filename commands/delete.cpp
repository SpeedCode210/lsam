#include "delete.hpp"
#include <unistd.h>
#include <iostream>
#include "../Application.hpp"

namespace commands {
    int DeleteCommand(int argc, char *argv[]) {

        if (argc <= 2) {
            std::cerr << "Please enter a valid application name/service_id." << std::endl;
            return 1;
        }

        auto apps = Application::GetApps();

        Application *app = nullptr;

        int i;
        for (i = 0; i < apps.size(); i++) {
            if (apps[i].DisplayName == argv[2] || apps[i].ServiceName == argv[2]) {
                app = &(apps[i]);
                break;
            }
        }

        if (app == nullptr) {
            std::cerr << "The entered application doesn't exist." << std::endl;
            return 1;
        }

        std::cout << "Do you really want to delete the application ? [y/N] ";
        std::string r;
        std::cin >> r;

        if (r != "y" && r != "Y") {
            return 0;
        }

        system(("systemctl disable --now " + app->ServiceName).c_str());
        system(("rm -rf " + app->Directory).c_str());
        system(("rm -f /etc/systemd/system/" + app->ServiceName + ".service").c_str());

        apps.erase(apps.begin() + i);

        Application::SetApps(apps);

        std::cout << "The application has been deleted !" << std::endl;

        return 0;
    }
}
