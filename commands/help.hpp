#include<iostream>

namespace commands {
    int HelpCommand(int argc, char *argv[]) {
        std::cout
                << "List of commands :" << std::endl
                << "lsam help                     : Displays the list of commands" << std::endl
                << "lsam list                     : Displays the list of applications" << std::endl
                << "lsam info <app>               : Displays information about an application" << std::endl
                << "lsam logs <app>               : Shows the journal file of the app's service" << std::endl
                << "lsam create                   : Launches the app creation wizard" << std::endl
                << "lsam start <app>              : Starts the application" << std::endl
                << "lsam restart <app>            : Restarts the application" << std::endl
                << "lsam stop <app>               : Stops the application" << std::endl
                << "lsam kill <app>               : Kills the application's process" << std::endl
                << "lsam autostart on/off <app>   : Enables or disables application's launch at startup" << std::endl
                << "lsam root on/off <app>        : Enables or disables application's root privileges" << std::endl
                << "lsam mainfile <app> <file>    : Changes the application's main file" << std::endl
                << "lsam rename <app> <new-name>  : Changes the application's name" << std::endl
                << "lsam delete <app>             : Deletes an application" << std::endl
                << "lsam backup <app>             : Creates a backup of an application" << std::endl
                << "lsam backup-all               : Creates a backup of every application" << std::endl
                << "lsam list-backups             : Gets a list of all the apps's backups" << std::endl
                << "lsam list-backups <app>       : Gets a list of all the backups os a specific application" << std::endl
                << "lsam delete-backup <name>     : Deletes a backup of an app" << std::endl
                << "lsam restore-backup <name>    : Restores a backup of an app" << std::endl
                << "lsam export <app> <archive>?  : Exports an application in a tar.gz archive" << std::endl
                << "lsam import <archive>         : Imports a tar.gz archive of an lsam application" << std::endl
                ;
        return 0;
    }
}
