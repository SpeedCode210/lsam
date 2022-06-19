#include <iostream>
#include <vector>
#include <algorithm>
#include <filesystem>
#include <regex>
#include <fstream>
#include <unistd.h>
#include "../templates.hpp"
#include "../consts.h"
#include "../Application.hpp"

namespace commands {
    int CreateCommand(int argc, char *argv[]) {

        std::cout << "N.B. Please install before launching the necessary runtimes/compilers for your app." << std::endl;

        //Choose app name and set service_name
        std::string name = "";
        std::string serviceName = "";
        bool isValid = false;
        while (!isValid) {
            std::cout << "Please enter application name : ";
            std::getline(std::cin, name);

            serviceName = name;
            std::for_each(serviceName.begin(), serviceName.end(), [](char &c) {
                c = std::tolower(c);
                if (c == ' ')
                    c = '.';
            });

            //Check if app exists
            if (!name.empty()) {
                isValid = true;
                std::vector <Application> apps = Application::GetApps();
                for (auto &app: apps) {
                    if (app.DisplayName == name || app.ServiceName == serviceName) {
                        isValid = false;
                        break;
                    }
                }
                isValid = isValid && !std::filesystem::exists("/etc/systemd/system/" + serviceName + ".service")
                          && !std::filesystem::exists("/usr/lib/systemd/system/" + serviceName + ".service");

                if (!isValid)
                    std::cerr << "This application already exists !" << std::endl;


            }
        }

        std::string workingDir = "/home/" + USERNAME + "/applications/" + serviceName;

        //Choose application type
        std::string type;
        while (true) {
            int appType = -1;
            std::vector <std::string> types;
            std::cout << "Please choose the type of the app : \n";
            int i = 0;
            for (auto const &x: templates::Templates) {
                std::cout << i + 1 << ") " << x.first << std::endl;
                types.push_back(x.first);
                i++;
            }

            std::string r;
            std::getline(std::cin, r);
            try {
                appType = std::stoi(r);
            } catch (std::invalid_argument) {
                appType = -1;
            }

            if (appType < 1 || appType > templates::Templates.size())
                continue;
            type = types[appType - 1];
            break;

        }

        //If app needs Root
        std::string user = "";
        while (user.empty()) {
            std::cout << "Does your application need root permissions ? [y/N]";
            std::string r;
            std::getline(std::cin, r);
            if (r == "y" || r == "Y")
                user = "root";
            else if (r == "" || r == "n" || r == "N")
                user = USERNAME;
        }

        //Select file to run
        std::string file = std::get<0>(templates::Templates.at(type));
        if (file != "N/A") {
            std::cout << "Please enter the file (command if custom command selected) to run with needed args. [" << file << "] ";
            std::string r;
            std::getline(std::cin, r);
            if (!r.empty())
                file = r;
        }

        //Create app object
        Application app;
        app.ServiceName = serviceName;
        app.DisplayName = name;
        app.EntryFile = file;
        app.NeedsRoot = user == "root";
        app.Directory = workingDir;
        app.Type = type;

        //Create necessary folder
        std::filesystem::create_directories(workingDir);
        system(("chown -R " + USERNAME + " " + workingDir).c_str());

        //Create service file
        std::string service = std::get<1>(templates::Templates.at(type));
        service = std::regex_replace(service, std::regex("\\{user\\}"), user);
        service = std::regex_replace(service, std::regex("\\{workingDir\\}"), workingDir);
        service = std::regex_replace(service, std::regex("\\{file\\}"), file);

        system(("touch /etc/systemd/system/" + serviceName + ".service").c_str());
        std::fstream fileService;
        fileService.open("/etc/systemd/system/" + serviceName + ".service");
        fileService << service;
        fileService.close();

        Application::AddApp(app);

        system(("systemctl enable " + serviceName).c_str());

        std::cout << "The application has been created, please go to the folder of the app with this command :\n"
                  << "'cd " + workingDir + "' to put in it the application's files\n"
                  << "Then start it using the command : 'lsam start " + serviceName + "'\n";

        return 0;
    }
}