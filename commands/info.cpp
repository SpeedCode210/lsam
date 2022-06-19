#include "info.hpp"
#include <string>
#include <iostream>
#include "../cli-table-cpp/Table.hpp"
#include "../Application.hpp"


namespace commands {
    int InfoCommand(int argc, char *argv[]) {

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

        (*app).CheckRunning();

        CliTable::Options opt;
        CliTable::TableBody content;

        content.push_back({"Application", (*app).DisplayName});
        content.push_back({"Service", (*app).ServiceName + ".service"});
        content.push_back({"Main file", (*app).EntryFile});
        content.push_back({"Type", (*app).Type});
        content.push_back({"Directory", (*app).Directory});
        content.push_back({"Autostart", (*app).IsEnabled ? "Yes" : "No"});
        content.push_back({"Root", (*app).NeedsRoot ? "Yes" : "No"});
        content.push_back({"Status",
                           (*app).IsRunning == "active"
                           ? "Running"
                           : (*app).IsRunning == "inactive"
                             ? "Stopped"
                             : "Crashed"});

        CliTable::Table table(opt, content);
        table.generate();


        return 0;
    }
}