#include "set"

#include "nodeRoutine.hpp"
#include "socketRoutine.hpp"

int main() {
    std::set<int> Nodes;
    std::string programPath = getenv("PROGRAM_PATH");
    Node task(-1);
    Nodes.insert(-1);
    std::string command;
    while (std::cin >> command) {
        if (command == "create") {
            int idChild, idParent;
            std::cin >> idChild >> idParent;
            if (Nodes.find(idChild) != Nodes.end()) {
                std::cout << "Error: Already exists" << std::endl;
            } else if (Nodes.find(idParent) == Nodes.end()) {
                std::cout << "Error: Parent not found" << std::endl;
            }else if (idParent == task.id) { // from -1
                std::string ans = task.Create(idChild, programPath);
                std::cout << ans << std::endl;
                Nodes.insert(idChild);
            } else { // from other node
                std::string str = "create " + std::to_string(idChild);
                std::string ans = task.Send(str, idParent);
                std::cout << ans << std::endl;
                Nodes.insert(idChild);
            }   
        } else if (command == "ping") {
            int idChild;
            std::cin >> idChild;
            if (Nodes.find(idChild) == Nodes.end()) {
                std::cout << "Error: Not found" << std::endl;
            } else if (task.children.find(idChild) != task.children.end()) {
                std::string ans = task.Ping(idChild);
                std::cout << ans << std::endl;
            } else {
                std::string str = "ping " + std::to_string(idChild);
                std::string ans = task.Send(str, idChild);
                if (ans == "Error: Not found") {
                    ans = "Ok: 0";
                }
                std::cout << ans << std::endl;
            }
        }else if (command == "exec") {
            int id, number, count;
            std::cin >> id >> count;
            std::string msg = "exec " + std::to_string(count); 
            for (int i = 0; i < count; i++) {
                std::cin >> number;
                msg += " " + std::to_string(number);
            }
            if (Nodes.find(id) == Nodes.end()) {
                std::cout << "Error: Not found" << std::endl;
            } else {
                std::string ans = task.Send(msg, id);
                std::cout << ans << std::endl;
            }
        }else if(command == "kill") {
            int id;
            std::cin >> id;
            std::string msg = "kill";
            if (Nodes.find(id) == Nodes.end()) {
                std::cout << "Error: Not found" << std::endl;
            } else {
                std::string ans = task.Send(msg, id);
                if (ans != "Error: Not found") {
                    std::istringstream ids(ans);
                    int tmp;
                    while(ids >> tmp) {
                        Nodes.erase(tmp);
                    }
                    ans = "Ok";
                    if(task.children.find(id) != task.children.end()) {
                        Unbind(task.children[id].get(), task.childrenPort[id]);
                        task.children[id]->close();
                        task.children.erase(id);
                        task.childrenPort.erase(id);
                    }
                }
                std::cout << ans << std::endl;
            }
        } else if (command == "exit") {
            task.Kill();
            return 0;
        }
    }
}