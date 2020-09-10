#include <iostream>
#include <fstream>
#include <string>
#include "str"

namespace std {
    str html_request(string url) {
        str request = "getpage \"";
        request += url;
        request += "\" temp.html";

        system(request.c_str());
        str res;

        std::fstream file;
        file.open("temp.html", std::ios::in);
        if(file.is_open()) {
            str tp;
            while(std::getline(file, tp)) {
                res += tp;
            }
            file.close();
        }

        return res;
    }
}