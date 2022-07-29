#include <iostream>
#include <fstream>
#include "json.hpp"
#include "whereami.hpp"


std::string chars_to_str(char *chars) {
    std::string str;

    for (int i = 0; i < strlen(chars); i++) {
        str.push_back(chars[i]);
    }

    return str;
}

std::string get_app_path() {
    std::string str_path;
    char *path;
    int length, dirname_length;
    length = wai_getExecutablePath(nullptr, 0, &dirname_length);
    if (length > 0) {
        path = (char *) malloc(length + 1);
        if (!path)
            abort();
        wai_getExecutablePath(path, length, &dirname_length);
        path[dirname_length] = '\0';
        str_path = chars_to_str(path);
        free(path);
    }
    return str_path;
}

void exec_macro(char *macro_name) {
    std::string macro_name_str = chars_to_str(macro_name);
    std::string file_path = get_app_path() + "/config.json";
    std::ifstream file(file_path);

    if (file.is_open()) {
        std::stringstream buffer;
        buffer << file.rdbuf();
        auto json = nlohmann::json::parse(buffer.str());
        auto json_array = json["macros"];

        for (auto &i: json_array) {
            if (i["macroName"] == macro_name_str) {

                std::string cmd;
                std::string desc;

                cmd.append(i["command"]);
                desc.append(i["description"]);

                printf("\nDescription: %s\n\n", desc.c_str());
                printf("Command: %s\n\n", cmd.c_str());

                system(cmd.c_str());
                exit(0);

            };
        }
        printf("%s is not a known macro.\n", macro_name);

    } else {
        printf("\nCarl could not find configuration file at: \n%s\n\n", file_path.c_str());
        std::cout << "Configuration file structure:\n";
        std::cout << "{\n"
                     "  \"macros\": [\n"
                     "    {\n"
                     "      \"macroName\": \"hello-cmd\",\n"
                     "      \"command\": \"cmd.exe echo \\\"Hello from cmd\\\"\",\n"
                     "      \"description\": \"Echo hello message from cmd\"\n"
                     "    },\n"
                     "    {\n"
                     "      \"macroName\": \"hello-ps\",\n"
                     "      \"command\": \"powershell.exe echo \\\"Hello from powershell\\\"\",\n"
                     "      \"description\": \"Echo hello message from powershell\"\n"
                     "    },\n"
                     "    {\n"
                     "      \"macroName\": \"hello-sh\",\n"
                     "      \"command\": \"bash echo \\\"Hello from bash\\\"\",\n"
                     "      \"description\": \"Echo hello message from bash\"\n"
                     "    }\n"
                     "  ]\n"
                     "}\n\n";
    }
}

int main(int argc, char *argv[]) {

    if (argc > 1) {
        exec_macro(argv[1]);
    } else {
        std::cout << "Usage: carl [macro name]";
    }
    return 0;
}
