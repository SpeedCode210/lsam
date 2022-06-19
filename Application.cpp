#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <array>
#include "json.hpp"
#include "consts.h"

#include "Application.hpp"

std::vector<Application> Application::GetApps() {
    std::ifstream appsFile;
    appsFile.open("/home/" + USERNAME + "/applications/.apps");
    std::string content = "[]\n";
    if(appsFile.is_open()){
        content = "";
        std::string line;
        while(getline(appsFile, line)){
            content += line + "\n";
        }
        appsFile.close();
    }

    std::vector<Application> result {};

    auto apps = nlohmann::json::parse(content);
    if(apps.is_array()) {
        for(int i = 0; i < apps.size(); i++){
            Application app;
            app.DisplayName = apps[i]["DisplayName"];
            app.ServiceName = apps[i]["ServiceName"];
            try{
                app.EntryFile = apps[i]["EntryFile"];
            }
            //Compatibility with esm1.x
            catch(nlohmann::detail::type_error){
                app.EntryFile = "";
            };
            app.Type = apps[i]["Type"];
            //Compatibility with esm1.x
            if(app.Type == "DotNet"){
                app.Type = ".NET";
            }
            else if(app.Type == "GoLang"){
                app.Type = "Golang";
            }
            app.Directory = apps[i]["Directory"];
            app.NeedsRoot = apps[i]["NeedsRoot"];
            result.push_back(app);
        }
    }

    return result;
}

void Application::AddApp(Application app) {
    using nlohmann::json;

    auto apps = GetApps();
    apps.push_back(app);
    std::ofstream appsFile;
    appsFile.open("/home/" + USERNAME + "/applications/.apps", std::ofstream::out | std::ofstream::trunc);
    if(appsFile.is_open()){
        json appsJson = json::array();
        for(int i = 0; i < apps.size(); i++){
            appsJson.push_back({
                {"DisplayName", apps[i].DisplayName},
                {"ServiceName", apps[i].ServiceName},
                {"EntryFile", apps[i].EntryFile},
                {"Type", apps[i].Type},
                {"Directory", apps[i].Directory},
                {"NeedsRoot", apps[i].NeedsRoot},
                });
        }

        appsFile << appsJson.dump(4) << std::endl;

        appsFile.close();
    }
}


void Application::SetApps(std::vector<Application> apps) {
    using nlohmann::json;

    std::ofstream appsFile;
    appsFile.open("/home/" + USERNAME + "/applications/.apps", std::ofstream::out | std::ofstream::trunc);
    std::string content = "[]\n";
    if(appsFile.is_open()){
        json appsJson = json::array();
        for(int i = 0; i < apps.size(); i++){
            appsJson.push_back({
                                       {"DisplayName", apps[i].DisplayName},
                                       {"ServiceName", apps[i].ServiceName},
                                       {"EntryFile", apps[i].EntryFile},
                                       {"Type", apps[i].Type},
                                       {"Directory", apps[i].Directory},
                                       {"NeedsRoot", apps[i].NeedsRoot},
                               });
        }

        appsFile << appsJson.dump(4) << std::endl;

        appsFile.close();
    }
}

std::string Application::exec(const char* cmd) {
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

void Application::CheckRunning() {
    auto command = "systemctl status " + ServiceName + " && echo";
    std::string content = exec(command.c_str());
    std::regex activeRegex("Active\\:[\\s][\\S]*", std::regex_constants::ECMAScript);

    std::cmatch matchs;

    if (std::regex_search(content.c_str(), matchs, activeRegex)){
        std::string match = matchs[0];
        match = match.substr(match.find(" ") + 1, match.size());
        IsRunning = match;
        IsEnabled = content.find("enabled") != std::string::npos;
    }

}
