#include "mainfile.hpp"
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <regex>
#include "../Application.hpp"
#include "../templates.hpp"
#include "../consts.h"

namespace commands {
    int MainfileCommand(int argc, char *argv[]) {

        if (argc <= 2) {
            std::cerr << "Please enter a valid application name/service_id." << std::endl;
            return 1;
        }

        if (argc <= 3) {
            std::cerr << "Please enter a valid file name." << std::endl;
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

        if(app->EntryFile == "N/A"){
            std::cerr << "The entered application doesn't needs a main file." << std::endl;
            return 1;
        }

        //Recreate service file
        std::string service = std::get<1>(templates::Templates.at((*app).Type));
        service = std::regex_replace(service, std::regex("\\{user\\}"), (*app).NeedsRoot ? "root" : USERNAME);
        service = std::regex_replace(service, std::regex("\\{workingDir\\}"), (*app).Directory);
        service = std::regex_replace(service, std::regex("\\{file\\}"), argv[3]);

        std::ofstream fileService;
        fileService.open("/etc/systemd/system/" + (*app).ServiceName + ".service",
                         std::ofstream::out | std::ofstream::trunc);
        fileService << service;
        fileService.close();

        app->EntryFile = argv[3];
        Application::SetApps(apps);

        system("systemctl daemon-reload");
        system(("systemctl restart " + (*app).ServiceName).c_str());


        return 0;
    }
}