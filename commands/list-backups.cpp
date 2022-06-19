#include "list-backups.hpp"
#include "../consts.h"
#include "../Application.hpp"
#include "../cli-table-cpp/Utils.hpp"
#include "../cli-table-cpp/Table.hpp"
#include <iostream>
#include <string>
#include <filesystem>
#include <regex>

namespace commands {
    int ListBackupsCommand(int argc, char *argv[]) {

        if (argc < 2) {
            std::cerr << "Please enter valid arguments" << std::endl;
            return 1;
        }

        std::string path = "/home/" + USERNAME + "/backups";

        auto apps = Application::GetApps();

        CliTable::Options opt;
        CliTable::TableBody content;

        //All apps
        if (argc == 2) {

            content.push_back({"Backup", "Application", "Date", "Hour"});

            for (const auto &entry: std::filesystem::directory_iterator(path)) {
                std::string name = entry.path().string();
                name = name.substr(name.find_last_of('/') + 1, name.size());

                std::smatch nameMatchs;
                std::smatch dateMatchs;
                std::smatch hourMatchs;
                if (std::regex_search(name, nameMatchs, std::regex(".+?(?=-[\\d])"))
                    &&
                    std::regex_search(name, dateMatchs, std::regex("[\\d]{4}-[\\d]{2}-[\\d]{2}(?=-[\\d]{2}:[\\d]{2})"))
                    && std::regex_search(name, hourMatchs, std::regex("[\\d]{2}:[\\d]{2}(?=\\.tar)"))) {
                    for (int i = 0; i < apps.size(); i++) {
                        if (apps[i].ServiceName == nameMatchs[0]) {
                            content.push_back({name, apps[i].DisplayName, dateMatchs[0], hourMatchs[0]});
                            break;
                        }
                    }
                }

            }

        }
            //Specific app
        else {

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

            content.push_back({"Backup", "Date", "Hour"});

            for (const auto &entry: std::filesystem::directory_iterator(path)) {
                std::string name = entry.path().string();
                name = name.substr(name.find_last_of('/') + 1, name.size());

                std::smatch nameMatchs;
                std::smatch dateMatchs;
                std::smatch hourMatchs;
                if (std::regex_search(name, nameMatchs, std::regex(".+?(?=-[\\d])"))
                    &&
                    std::regex_search(name, dateMatchs, std::regex("[\\d]{4}-[\\d]{2}-[\\d]{2}(?=-[\\d]{2}:[\\d]{2})"))
                    && std::regex_search(name, hourMatchs, std::regex("[\\d]{2}:[\\d]{2}(?=\\.tar)"))) {
                    if (app->ServiceName == nameMatchs[0]) {
                        content.push_back({name, dateMatchs[0], hourMatchs[0]});
                    }
                }

            }

        }


        CliTable::Table table(opt, content);
        table.generate();

        return 0;

    }
}