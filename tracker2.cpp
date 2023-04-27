#include <iostream>
#include <map>
#include <set>
#include <vector>

int main() {
    std::map<std::string, std::map<int, std::string>> data;

    int n;
    std::cin >> n;

    for (int i = 0; i < n; i++) {
        std::string uuid;
        std::string location;
        int ts;
        std::cin >> uuid >> location >> ts;
        data[uuid][ts] = location;
    }

    std::vector<std::string> answer;
    std::cin >> n;
    for (int i = 0; i < n; i++) {
        std::string req_type;
        int ts;
        std::cin >> req_type;
        if (req_type == "object") {
            std::string uuid;
            std::cin >> uuid >> ts;
            if (data.find(uuid) == data.end()) {answer.push_back("no data"); continue;}
            if (ts < (data[uuid].begin())->first) {
                answer.push_back("no data");
            } else {
                answer.push_back(std::prev(data[uuid].upper_bound(ts))->second);
            }
        } else {
            bool found_loc = false;
            std::string line;
            std::string name;
            std::cin >> name >> ts;
            for (auto element : data) {
                for (auto rhs : element.second) {
                    if (rhs.second == name) {
                        found_loc = true;
                        continue;
                    }
                }
                if (element.second.upper_bound(ts) != element.second.begin()) {
                if (std::prev(element.second.upper_bound(ts))->second == name) {
                    line += element.first + " ";
                }
                }
            }
            if (found_loc == false) {
                answer.push_back("no data");
            } else {
            answer.push_back(line);
            }
        }
    }
    for (auto element : answer) {
        std::cout << element << std::endl;
    }
}