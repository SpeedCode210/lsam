cmake_minimum_required(VERSION 3.22)
project(lsam)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")


add_executable(lsam main.cpp commands/help.hpp Application.cpp Application.hpp Application.cpp Application.hpp commands/list.cpp commands/list.hpp cli-table-cpp/Table.cpp cli-table-cpp/Table.hpp cli-table-cpp/Utils.cpp cli-table-cpp/Utils.hpp json.hpp commands/create.hpp commands/create.cpp templates.hpp consts.h commands/info.hpp commands/info.cpp commands/logs.hpp commands/logs.cpp commands/start.cpp commands/start.hpp commands/stop.hpp commands/stop.cpp commands/kill.cpp commands/kill.hpp commands/restart.cpp commands/restart.hpp commands/autostart.hpp commands/autostart.cpp commands/root.hpp commands/root.cpp commands/mainfile.hpp commands/mainfile.cpp commands/rename.hpp commands/rename.cpp commands/init.hpp commands/delete.hpp commands/delete.cpp commands/backup.hpp commands/backup.cpp commands/backup-all.hpp commands/backup-all.cpp commands/list-backups.hpp commands/list-backups.cpp commands/delete-backup.hpp commands/delete-backup.cpp commands/restore-backup.hpp commands/restore-backup.cpp commands/export.hpp commands/export.cpp commands/import.hpp commands/import.cpp)

