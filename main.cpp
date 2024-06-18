#include <cstdlib>
#include <fstream>
#include <iostream>
#include <ncurses.h>
#include <string>
#include <vector>

enum state {
    NORMAL,
    SELECTED,
    ACTIVE
};

void parser(std::vector<std::string> todos, std::vector<std::string> dones, std::string path){
    std::ifstream FILE(path);
    std::string line;
    while (std::getline(FILE, line)) {
        if (line.substr(0, 6) == "TODO: ") {
            std::string todo = line.substr(6);
            todos.push_back(todo);
        } else if (line.substr(0, 6) == "DONE: ") {
            std::string done = line.substr(6);
            dones.push_back(done);
        } else {
            endwin();
            std::cerr << "invalid input file" << std::endl;
            exit(1);
        }
    }
    FILE.close();

}

int main() {
    std::vector<std::string> todos;
    todos.push_back("program");
    todos.push_back("sleep");

    std::vector<std::string> dones;
    dones.push_back("sleep");
    dones.push_back("eat");
    dones.push_back("anime");

    bool isTodo = true, cont = true;
    int line = 0;
    
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    start_color();
    init_pair(NORMAL, COLOR_WHITE, COLOR_BLACK);
    init_pair(SELECTED, COLOR_BLACK, COLOR_WHITE);
    init_pair(ACTIVE, COLOR_GREEN, COLOR_BLACK);

    WINDOW* input_win = nullptr;

    while(cont) {
        erase();
        if(isTodo) {
            attron(COLOR_PAIR(ACTIVE));
            printw("[TODOS]");
            attroff(COLOR_PAIR(ACTIVE));
            printw(" DONES \n");
            for (int i = 0; i < todos.size(); i++) {
                if(i == line) {
                    attron(COLOR_PAIR(SELECTED));
                    printw("\t- [ ] %s\n", todos[i].c_str());
                    attroff(COLOR_PAIR(SELECTED));
                } else {
                    printw("\t- [ ] %s\n", todos[i].c_str());
                }
            }
        } else {
            printw(" TODOS ");
            attron(COLOR_PAIR(ACTIVE));
            printw("[DONES]\n");
            attroff(COLOR_PAIR(ACTIVE));
            for(int i = 0; i < dones.size(); i++) {
                if(i == line) {
                    attron(COLOR_PAIR(SELECTED));
                    printw("\t- [x] %s\n", dones[i].c_str());
                    attroff(COLOR_PAIR(SELECTED));
                } else {
                    printw("\t- [ ] %s\n", dones[i].c_str());
                }
            }
        }

        char pressed = getch();
        switch (pressed) {
            case 'q':
                cont = false;
                break;
            case '\t':
                isTodo = !isTodo;
                if (isTodo) {
                    if (line >= todos.size()) {
                        line = todos.size()-1;
                    }
                } else {
                    if (line >= dones.size()) {
                        line = dones.size()-1;
                    }
                }
                break;
            case 'k':
                if (line > 0) {
                    line--;
                }
                break;
            case 'j':
                if (isTodo) {
                    if (line < todos.size() - 1) {
                        line++;
                    }
                } else {
                    if (line < dones.size() - 1) {
                        line++;
                    }
                }
                break;
            case 'e':
                if (!input_win) {
                    int input_height = 3, input_width = 40;
                    int input_y = LINES/2 - input_height/2;
                    int input_x = COLS/2 - input_width/2;
                    input_win = newwin(input_height, input_width, input_y, input_x);
                    box(input_win, 0, 0);
                    mvwprintw(input_win, 1, 1, "Enter file path: ");
                    wrefresh(input_win);

                    char input_str[256];
                    echo();
                    wgetstr(input_win, input_str);
                    noecho();

                    std::string new_todo = input_str;

                    

                    parser(todos, dones, input_str);

                    delwin(input_win);
                    input_win = nullptr; 
                }
                break;
            case '\n':
                if (isTodo) {
                    if (line >= todos.size()){
                        break;
                    }
                    std::string str = todos[line];
                    dones.push_back(str);
                    todos.erase(todos.begin() + line);
                } else {
                    if (line >= dones.size()){
                        break;
                    }
                    std::string str = dones[line];
                    todos.push_back(str);
                    dones.erase(dones.begin() + line);
                }
                break;
            case 'a':
                if (!input_win) {
                    int input_height = 3, input_width = 40;
                    int input_y = LINES/2 - input_height/2;
                    int input_x = COLS/2 - input_width/2;
                    input_win = newwin(input_height, input_width, input_y, input_x);
                    box(input_win, 0, 0);
                    mvwprintw(input_win, 1, 1, "Enter new todo: ");
                    wrefresh(input_win);

                    char input_str[256];
                    echo();
                    wgetstr(input_win, input_str);
                    noecho();

                    std::string new_todo = input_str;
                    todos.push_back(new_todo);

                    delwin(input_win);
                    input_win = nullptr; 
                }
                break;
            default:
                break;
        }
    }


    endwin();
}

