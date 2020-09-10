#include <iostream>
#include <windows.h>
#include "parser.h"
#include <vector>
#include "htmlclient.h"

#pragma warning(disable : 4018)

using namespace std;

class Program {
private:
    struct SearchResult {
        string url;
        string title;
        string content;
    };

    static std::vector<SearchResult> search(string request) {
        std::vector<SearchResult> res;

        string url = "https://en.wikipedia.org/w/index.php?sort=relevance&search=";
        url += request + "&fulltext=1&advancedSearch-current={}&ns0=1";
        htmlparser::htmlblock b(html_request(url));

        b = b.
            get_block("body").
            get_block("div", 0, "id", "content").
            get_block("div", 0, "id", "bodyContent").
            get_block("div", 0, "id", "mw-content-text");

        if(b.get_block("div", 0, "class", "searchresults").get_body() == "") {
            b = b.get_block("div", 0, "class", "searchresults mw-searchresults-has-iw");
        }
        else {
            b = b.get_block("div", 0, "class", "searchresults");
        }

        b = b.get_block("ul");
        htmlparser::htmlblock elements(b.get_body());

        for(int i = 0; i < 5; i++) {
            SearchResult s;

            str temp = elements.
                get_block("li", i).
                get_block("div").
                get_body();

            htmlparser::htmlblock t(temp);

            s.url = t.get_prop_value("a", "href");
            s.title = t.get_prop_value("a", "title");

            if(s.title != "") res.push_back(s);
        }
        return res;
    }
public:
    static void start() {
        while(true) {
            cout << "Введiть пошуковий запит\n";

            string search;
            cin >> search;
            std::vector<SearchResult> res = Program::search(search);
            
            if(res.size() == 0) {
                cout << "Пошук не дав результатiв\n";
                continue;
            }

            for(int i = 0; i < res.size(); i++)
                cout << i + 1 << " - " << res[i].title << endl;

            if(res.size() != 0) 
                cout << "Введiть номер сторiнки -> ";
            

            int ans;
            cin >> ans;
            ans--;

            if(ans > res.size() || ans < 0) {
                cout << "Такого варiанту немаэ\n";
                continue;
            }
            string url = "https://en.wikipedia.org";

            url += res[ans].url;

            cout << url << endl;
        }
    }
};

int main() {
    // Пiдключення локалiзацii
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
    Program::start();
}