#include "rename.hpp"
#include <string.h>
#include <filesystem>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <regex>
#include <filesystem>
#include "../Application.hpp"
#include "../templates.hpp"
#include "../consts.h"

namespace commands {
    int RenameCommand(int argc, char *argv[]) {

        if (argc <= 3) {
            std::cerr << "Please enter a valid application name/service_id." << std::endl;
            return 1;
        }

        std::string newServiceName = argv[3];
        std::for_each(newServiceName.begin(), newServiceName.end(), [](char &c) {
            c = std::tolower(c);
            if (c == ' ')
                c = '.';
        });

        if (std::filesystem::exists("/etc/systemd/system/" + newServiceName + ".service")
            || std::filesystem::exists("/usr/lib/systemd/system/" + newServiceName + ".service")) {
            std::cerr << "The new application's name is already in use" << std::endl;
            return 1;
        }

        auto apps = Application::GetApps();

        Application *app = nullptr;

        for (int i = 0; i < apps.size(); i++) {
            if (apps[i].DisplayName == argv[2] || apps[i].ServiceName == argv[2]) {
                app = &(apps[i]);
            }
            if (apps[i].DisplayName == argv[3] || apps[i].ServiceName == argv[3] ||
                apps[i].ServiceName == newServiceName) {
                std::cerr << "The new application's name is already in use" << std::endl;
                return 1;
            }
        }

        if (app == nullptr) {
            std::cerr << "The entered application doesn't exist." << std::endl;
            return 1;
        }

        system(("systemctl disable --now " + (*app).ServiceName).c_str());

        system(("mv " + (*app).Directory + " /home/" + USERNAME + "/applications/" + newServiceName).c_str());

        system(("rm -f /etc/systemd/system/" + (*app).ServiceName + ".service").c_str());

        app->DisplayName = argv[3];
        app->ServiceName = newServiceName;
        app->Directory = "/home/" + USERNAME + "/applications/" + newServiceName;

        //Recreate service file
        std::string service = std::get<1>(templates::Templates.at((*app).Type));
        service = std::regex_replace(service, std::regex("\\{user\\}"), (*app).NeedsRoot ? "root" : USERNAME);
        service = std::regex_replace(service, std::regex("\\{workingDir\\}"), (*app).Directory);
        service = std::regex_replace(service, std::regex("\\{file\\}"), (*app).EntryFile);

        system(("touch /etc/systemd/system/" + (*app).ServiceName + ".service").c_str());

        std::ofstream fileService;
        fileService.open("/etc/systemd/system/" + (*app).ServiceName + ".service",
                         std::ofstream::out | std::ofstream::trunc);
        fileService << service;
        fileService.close();

        Application::SetApps(apps);

        system("systemctl daemon-reload");
        system(("systemctl enable --now " + (*app).ServiceName).c_str());


        return 0;
    }
}