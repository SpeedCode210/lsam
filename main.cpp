#include <iostream>
#include <string>
#include <unordered_map>
#include <functional>
#include <filesystem>
#include <unistd.h>
#include "consts.h"
// Import commands
#include "commands/init.hpp"


int main(int argc, char* argv[])
{

    if (system("which systemctl > /dev/null 2>&1")) {
        std::cerr << "Sorry, this system is not compatible with lsam (without systemd)." << std::endl;
        return 1;
    }

    if(argc <= 1)
    {
        std::cerr << "Please enter a valid command, run 'lsam help' for the list of commands." << std::endl;
        return 1;
    }

    if (getuid())
    {
        std::cerr << "Please run lsam with root privileges." << std::endl;
        return 1;
    }

    if(!std::filesystem::exists("/home/" + USERNAME))
    {
        system(("useradd " + USERNAME).c_str());
        std::filesystem::create_directory("/home/" + USERNAME);
    }

    if(!std::filesystem::exists("/home/" + USERNAME + "/applications"))
    {
        std::filesystem::create_directory("/home/" + USERNAME + "/applications");
    }

    if (!getuid())
    {
        system(("chown -R " + USERNAME + " /home/" + USERNAME).c_str());
    }


    std::string command = argv[1];


    std::unordered_map<std::string, std::function<int(int, char* [])>> commands;

    commands["version"] = commands::VersionCommand;
    commands["help"] = commands::HelpCommand;
    commands["list"] = commands::ListCommand;
    commands["create"] = commands::CreateCommand;
    commands["info"] = commands::InfoCommand;
    commands["logs"] = commands::LogsCommand;
    commands["start"] = commands::StartCommand;
    commands["stop"] = commands::StopCommand;
    commands["restart"] = commands::RestartCommand;
    commands["kill"] = commands::KillCommand;
    commands["autostart"] = commands::AutostartCommand;
    commands["root"] = commands::RootCommand;
    commands["mainfile"] = commands::MainfileCommand;
    commands["rename"] = commands::RenameCommand;
    commands["delete"] = commands::DeleteCommand;
    commands["backup"] = commands::BackupCommand;
    commands["backup-all"] = commands::BackupAllCommand;
    commands["list-backups"] = commands::ListBackupsCommand;
    commands["delete-backup"] = commands::DeleteBackupCommand;
    commands["restore-backup"] = commands::RestoreBackupCommand;
    commands["export"] = commands::ExportCommand;
    commands["import"] = commands::ImportCommand;

    auto callback = commands[{argv[1]}];

    if(callback)
        return callback(argc, argv);

    std::cerr << "Please enter a valid command, run 'lsam help' for the list of commands.\n";
    return 1;

}



