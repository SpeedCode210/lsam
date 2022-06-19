#include "export.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include "../Application.hpp"
#include "../json.hpp"

namespace commands {
    int ExportCommand(int argc, char *argv[]) {
        if (argc < 3) {
            std::cerr << "Please specify a valid application to export" << std::endl;
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

        std::string exportFile = std::filesystem::current_path().string() + "/" + app->ServiceName + ".tar.gz";

        if (argc > 3) {
            exportFile = argv[3];
        }

        std::string tmp = std::filesystem::temp_directory_path().string() + "/lsam-temp-export-" + app->ServiceName;
        system(("mkdir -p " + tmp).c_str());


        system(("cp -r " + app->Directory + " " + tmp + "/app").c_str());

        system(("cp /etc/systemd/system/" + app->ServiceName + ".service " + tmp + "/app.service").c_str());

        using nlohmann::json;

        std::ofstream jsonFile;
        jsonFile.open(tmp + "/app.json", std::ofstream::out | std::ofstream::trunc);
        if (jsonFile.is_open()) {
            json appsJson({
                                  {"DisplayName", app->DisplayName},
                                  {"ServiceName", app->ServiceName},
                                  {"EntryFile",   app->EntryFile},
                                  {"Type",        app->Type},
                                  {"Directory",   app->Directory},
                                  {"NeedsRoot",   app->NeedsRoot},
                          });

            jsonFile << appsJson.dump(4) << std::endl;

            jsonFile.close();
        }

        system(("tar -czf " + exportFile + " --directory=" + tmp + " .").c_str());

        //Delete temporary files
        system(("rm -rf " + tmp).c_str());

        std::cout << "The application has been exported in " << exportFile << std::endl;

        return 0;
    }
}