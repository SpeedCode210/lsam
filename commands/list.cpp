#include "../cli-table-cpp/Table.hpp"
#include "../Application.hpp"

namespace commands {

    int ListCommand(int argc, char *argv[]) {

        CliTable::Options opt;
        CliTable::TableBody content;

        content.push_back({"Name", "Service", "Type", "Status", "Autostart", "Root", "Path"});

        auto apps = Application::GetApps();

        for (int i = 0; i < apps.size(); i++) {
            apps[i].CheckRunning();
            content.push_back(
                    {
                            apps[i].DisplayName,
                            apps[i].ServiceName,
                            apps[i].Type,
                            apps[i].IsRunning == "active"
                            ? "Running"
                            : apps[i].IsRunning == "inactive"
                              ? "Stopped"
                              : "Crashed",
                            apps[i].IsEnabled
                            ? "Yes"
                            : "No",
                            apps[i].NeedsRoot
                            ? "Yes"
                            : "No",
                            apps[i].Directory
                    }
            );
        }


        CliTable::Table table(opt, content);
        //Generating the final table and showing it

        table.generate();


        return 0;
    }
}