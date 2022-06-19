#pragma once

#include <string>
#include <vector>


struct Application {
public:
    //Properties
    std::string  DisplayName;
    std::string  ServiceName;
    std::string  Directory;
    std::string  EntryFile;
    std::string  Type;
    std::string  IsRunning;
    bool         IsEnabled;
    bool         NeedsRoot;
    //Functions
    void         CheckRunning();

    static std::vector<Application> GetApps();
    static void AddApp(Application app);
    static void SetApps(std::vector<Application> apps);

private:
    std::string exec(const char* cmd);

};

