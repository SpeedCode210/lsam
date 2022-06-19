# LSAM - Linux Systemd Application Manager

![LSAM Logo](logo.png)

## An application manager for Linux servers
It will allow you to manage NodeJS, Python, Java, .NET, Go...and more applications under Linux distributions using systemd.

## Requirements
- Any Linux distribution based on systemd service management system.

We provide packages for most popular Linux distributions

## Features
- Application creation wizard
- Application power commands
- Backup and restore utilities
- Import/Export system

## Commands list

| Command                        | Description                                              |
|--------------------------------|----------------------------------------------------------|
| lsam help                      | Displays the list of commands                            |
| lsam list                      | Displays the list of applications                        |
| lsam info \<app>               | Displays information about an application                |
| lsam logs \<app>               | Shows the journal file of the app's service              |
| lsam create                    | Launches the app creation wizard                         |
| lsam start \<app>              | Starts the application                                   |
| lsam restart \<app>            | Restarts the application                                 |
| lsam stop \<app>               | Stops the application                                    |
| lsam kill \<app>               | Kills the application's process                          |
| lsam autostart on/off \<app>   | Enables or disables application's launch at startup      |
| lsam root on/off \<app>        | Enables or disables application's root privileges        |
| lsam mainfile \<app> \<file>   | Changes the application's main file                      |
| lsam rename \<app> \<new-name> | Changes the application's name                           |
| lsam delete \<app>             | Deletes an application                                   |
| lsam backup \<app>             | Creates a backup of an application                       |
| lsam backup-all                | Creates a backup of every application                    |
| lsam list-backups              | Gets a list of all the apps's backups                    |
| lsam list-backups \<app>       | Gets a list of all the backups os a specific application |
| lsam delete-backup \<name>     | Deletes a backup of an app                               |
| lsam restore-backup \<name>    | Restores a backup of an app                              |
| lsam export \<app> \<archive>? | Exports an application in a tar.gz archive               |
| lsam import \<archive>         | Imports a tar.gz archive of an lsam application           |

**NB :** \<arg> is a necessary argument whereas \<arg>? is not.

## Used open-source libraries

- [nlohmann/json](https://github.com/nlohmann/json) for manipulating json files.
- [sdmg15/cli-table-cpp](https://github.com/sdmg15/cli-table-cpp) for displaying tables.