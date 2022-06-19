#pragma once

#include <string>
#include <unordered_map>
#include <tuple>


namespace templates{
    const std::unordered_map<std::string, std::tuple<std::string, std::string>> Templates {
            {
                    "Custom command",

                    {
                            "echo \"Hello world !\"",

                            "[Service]\n"
                            "User={user}\n"
                            "Type=simple\n"
                            "RemainAfterExit=no\n"
                            "Restart=on-failure\n"
                            "ExecStart=bash -c 'cd {workingDir} && {file}'\n"
                            "\n"
                            "[Install]\n"
                            "WantedBy=multi-user.target\n"
                    }
            },
            {
                    "Rust (Cargo)",

                    {
                            "N/A",

                            "[Service]\n"
                            "User={user}\n"
                            "Type=simple\n"
                            "RemainAfterExit=no\n"
                            "Restart=on-failure\n"
                            "ExecStart=bash -c 'cd {workingDir} && cargo run'\n"
                            "\n"
                            "[Install]\n"
                            "WantedBy=multi-user.target\n"
                    }
            },
            {
                    "Lua",

                    {
                            "main.lua",

                            "[Service]\n"
                            "User={user}\n"
                            "Type=simple\n"
                            "RemainAfterExit=no\n"
                            "Restart=on-failure\n"
                            "ExecStart=bash -c 'cd {workingDir} && lua {file}'\n"
                            "\n"
                            "[Install]\n"
                            "WantedBy=multi-user.target\n"
                    }
            },
            {
                    "Java",

                    {
                            "main.jar",

                            "[Service]\n"
                            "User={user}\n"
                            "Type=simple\n"
                            "RemainAfterExit=no\n"
                            "Restart=on-failure\n"
                            "ExecStart=bash -c 'cd {workingDir} && java -jar {file}'\n"
                            "\n"
                            "[Install]\n"
                            "WantedBy=multi-user.target\n"
                    }
            },
            {
                    "Golang",

                    {
                            "main.go",

                            "[Service]\n"
                            "User={user}\n"
                            "Type=simple\n"
                            "RemainAfterExit=no\n"
                            "Restart=on-failure\n"
                            "ExecStart=bash -c 'cd {workingDir} && go run {file}'\n"
                            "\n"
                            "[Install]\n"
                            "WantedBy=multi-user.target\n"
                    }
            },
            {
                    "NodeJS",

                    {
                            "index.js",

                            "[Service]\n"
                            "User={user}\n"
                            "Type=simple\n"
                            "RemainAfterExit=no\n"
                            "Restart=on-failure\n"
                            "ExecStart=bash -c 'cd {workingDir} && (test -f ./package.json && npm i) ; node {file}'\n"
                            "\n"
                            "[Install]\n"
                            "WantedBy=multi-user.target\n"
                    }
            },
            {
                    "Python",

                    {
                            "main.py",

                            "[Service]\n"
                            "User={user}\n"
                            "Type=simple\n"
                            "RemainAfterExit=no\n"
                            "Restart=on-failure\n"
                            "ExecStart=bash -c 'cd {workingDir} && python {file}'\n"
                            "\n"
                            "[Install]\n"
                            "WantedBy=multi-user.target\n"
                    }
            },
            {
                    ".NET",

                    {
                            "Program.dll",

                            "[Service]\n"
                            "User={user}\n"
                            "Type=simple\n"
                            "RemainAfterExit=no\n"
                            "Restart=on-failure\n"
                            "ExecStart=bash -c 'cd {workingDir} && dotnet {file}'\n"
                            "\n"
                            "[Install]\n"
                            "WantedBy=multi-user.target\n"
                    }
            },
    };
}
