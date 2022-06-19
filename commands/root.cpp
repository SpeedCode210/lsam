#include "root.hpp"
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <regex>
#include "../Application.hpp"
#include "../templates.hpp"
#include "../consts.h"

namespace commands {
    int RootCommand(int argc, char *argv[]) {

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

        //Recreate service file
        std::string service = std::get<1>(templates::Templates.at((*app).Type));
        service = std::regex_replace(service, std::regex("\\{user\\}"), strcmp(argv[2], "on") == 0 ? "root" : USERNAME);
        service = std::regex_replace(service, std::regex("\\{workingDir\\}"), (*app).Directory);
        service = std::regex_replace(service, std::regex("\\{file\\}"), (*app).EntryFile);

        std::ofstream fileService;
        fileService.open("/etc/systemd/system/" + (*app).ServiceName + ".service",
                         std::ofstream::out | std::ofstream::trunc);
        fileService << service;
        fileService.close();


        app->NeedsRoot = strcmp(argv[2], "on") == 0;
        Application::SetApps(apps);

        system("systemctl daemon-reload");
        system(("systemctl restart " + (*app).ServiceName).c_str());


        return 0;
    }
}