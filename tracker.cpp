#include <iostream>
#include <map>
#include <vector>


int main() {
    std::map<std::string, std::map<int, std::string>> data;            
    
    int N;
    std::cin >> N;
    std::string id, loc;
    int time;
    for (auto i = 0; i < N; i++) {
        std::cin >> id >> loc >> time;
        data[id][time] = loc;
    }

    std::vector<std::string> output;
    std::cin >> N;
    int ts;
    std::string request_type, uuid, name;
    for (auto i = 0; i < N; i++) {
        std::cin >> request_type;
        if (request_type == "object") {
            std::cin >> uuid >> ts;
            auto object_it = data.find(uuid);
            if (object_it == data.end()) { 
                output.push_back("no data");
                continue;
            }
            auto& locs = object_it->second;
            auto bound = locs.upper_bound(ts);
            if (bound == locs.begin()) {
                output.push_back("no data");
            } else {
                output.push_back(std::prev(bound)->second);
            }
        } else if (request_type == "location") {
            std::vector<std::string> objects_on_base;
            std::cin >> name >> ts;
            for (auto object : data) {
                auto& locs = object.second;
                auto bound = locs.upper_bound(ts);
                if (bound != locs.begin()) {
                    if (std::prev(bound)->second == name) {
                        objects_on_base.push_back(object.first);
                    }
                }
            }
            std::string to_push;
            for (auto element : objects_on_base) {
                to_push += element + " ";
            }
            output.push_back(to_push);
        }
    }
    for (auto element : output) {
        std::cout << element << std::endl;
    }
}