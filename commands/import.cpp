#include "import.hpp"
#include "../json.hpp"
#include "../Application.hpp"
#include "../consts.h"
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <array>
#include <string>
#include <filesystem>


  namespace {
      std::string exec(const char *cmd) {
          std::array<char, 128> buffer;
          std::string result;
          std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
          if (!pipe) {
              throw std::runtime_error("popen() failed!");
          }
          while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
              result += buffer.data();
          }
          return result;
      }
  }

namespace commands{
    int ImportCommand(int argc, char* argv[]){

        if (argc < 3) {
            std::cerr << "Please enter a valid archive filename." << std::endl;
            return 1;
        }

        std::string file = argv[2];

        if(!file.ends_with(".tar.gz")){
            std::cerr << "Please enter a valid archive filename." << std::endl;
            return 1;
        }

        if(!std::filesystem::exists(file)){
            std::cerr << "Please enter a valid archive filename." << std::endl;
            return 1;
        }

        std::string tmp = std::filesystem::temp_directory_path().string() + "/lsam-temp-export-" + file.substr(0, file.size()-7);
        system(("mkdir -p " + tmp).c_str());

        system(("tar -xf " + file + " -C " + tmp).c_str());


        //Delete temporary files
        std::string command = "ls " + tmp;

        if(exec(command.c_str()).compare("app\napp.json\napp.service\n") != 0){
            std::cerr << "The given archive is not a valid lsam application archive." << std::endl;
            return 1;
        }

        std::ifstream appsFile;
        appsFile.open(tmp + "/app.json");
        std::string content = "{}\n";
        if(appsFile.is_open()){
            content = "";
            std::string line;
            while(getline(appsFile, line)){
                content += line + "\n";
            }
            appsFile.close();
        }
        auto jsonApp = nlohmann::json::parse(content);
        Application app;

        app.EntryFile = jsonApp["EntryFile"];
        app.Type = jsonApp["Type"];
        app.NeedsRoot = jsonApp["NeedsRoot"];

        //Choose app name and set service_name
        std::string name;
        std::string serviceName;
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
                std::vector<Application> apps = Application::GetApps();
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


        app.DisplayName = name;
        app.ServiceName = serviceName;
        app.Directory = "/home/" + USERNAME + "/applications/" + serviceName;

        system(("cp -r " + tmp + "/app " + app.Directory).c_str());

        system(("cp " + tmp + "/app.service /etc/systemd/system/" + serviceName + ".service").c_str());

        Application::AddApp(app);

        system(("systemctl enable " + serviceName).c_str());

        system(("rm -rf " + tmp).c_str());

        std::cout << "The application has been imported successfully !" << std::endl;

        return 0;
    }
}
