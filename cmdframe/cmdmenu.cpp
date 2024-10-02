#include "cmdmenu.h"

#include <cassert>
#include <iostream>
#include <menu.h>
#include <ncurses.h>
#include <regex>

using std::cout;
using std::endl;
using std::function;
using std::string;

CmdMenu::CmdMenu(vector<std::string> &&item_descs, std::string &&title,
                 std::string &&desc, bool memorize, function<void()> on_enter,
                 function<void()> on_exit)
    : CmdFrame(std::bind(&CmdMenu::MenuMain, this)), title(title), desc(desc),
      on_enter(on_enter), on_exit(on_exit), memorize(memorize) {
    this->SetInitType(InitType::CURSE);
    for (auto &&desc : item_descs)
        this->menu_items.emplace_back(desc, nullptr);
}

CmdMenu::CmdMenu(vector<pair<std::string, function<int()>>> &&menu_event_pairs,
                 std::string &&title, std::string &&desc, bool memorize,
                 function<void()> on_enter, function<void()> on_exit)
    : CmdFrame(std::bind(&CmdMenu::MenuMain, this)), title(title), desc(desc),
      on_enter(on_enter), on_exit(on_exit), memorize(memorize) {
    this->SetInitType(InitType::CURSE);
    for (auto &&pair : menu_event_pairs)
        this->menu_items.emplace_back(
            MenuItem(std::move(pair.first), std::move(pair.second)));
}

void CmdMenu::Append(std::string &&desc) {
    this->menu_items.emplace_back(desc, nullptr);
}

void CmdMenu::Append(std::string &&desc, std::function<int()> event) {
    this->menu_items.emplace_back(desc, event);
}

finf_id_t CmdMenu::MenuMain() {
    if (this->on_enter)
        this->on_enter();
    // display menu & select
    if (!memorize)
        this->cur_index = 0;
    cur_index = this->DisplayAndSelect();
    assert(cur_index >= 0 && cur_index < this->menu_items.size());
    // handle event
    mevent_id_t mevent_id{MEVENT_CONTINUE};
    finf_id_t finf_id;
    if (this->menu_items[cur_index].event)
        mevent_id = this->menu_items[cur_index].event();
    if (mevent_id == MEVENT_CONTINUE)
        finf_id = FINF_FRAME(cur_index);
    else if (mevent_id == MEVENT_CANCEL)
        finf_id = FINF_SELF;
    else
        finf_id = FINF_FRAME(mevent_id);
    if (this->on_exit)
        this->on_exit();
    return finf_id;
}

item_id_t CmdMenu::DisplayAndSelect() {
    string rp_title = GetReplacedTitle();
    string rp_desc = GetReplacedDesc();
    // Calculate menu window dimensions
    int menu_height = menu_items.size() + 2 + 3; // menu items + title + desc
    int menu_width = 40;
    int menu_start_y = 0;
    int menu_start_x = 0;
    int n_choices = menu_items.size();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    // Create menu window
    WINDOW *menu_win =
        newwin(menu_height, menu_width, menu_start_y, menu_start_x);
    keypad(menu_win, TRUE);

    // Print title at the top
    if (!rp_title.empty())
        mvwprintw(menu_win, 0, 0, "%s", rp_title.c_str());

    // Create items for the menu
    ITEM **my_items;
    MENU *my_menu;
    my_items = (ITEM **)calloc(n_choices + 1, sizeof(ITEM *));
    for (int i = 0; i < n_choices; ++i) {
        my_items[i] = new_item(menu_items[i].desc.c_str(), "");
    }
    my_items[n_choices] = (ITEM *)NULL;

    // Create menu
    my_menu = new_menu(my_items);

    // Set menu window and menu association
    set_menu_win(my_menu, menu_win);
    set_menu_sub(my_menu, derwin(menu_win, n_choices, menu_width - 2, 2, 1));

    // Post the menu
    post_menu(my_menu);
    // Print description below the title
    if (!rp_desc.empty())
        mvwprintw(menu_win, n_choices + 2 + 1, 0, "%s", rp_desc.c_str());
    cur_index = memorize ? cur_index : 0;
    set_current_item(my_menu, my_items[cur_index]);
    wrefresh(menu_win);

    // Handle menu selection
    int choice_index = -1;
    int choice;
    for (;;) {
        choice = wgetch(menu_win);
        switch (choice) {
        case KEY_DOWN:
            menu_driver(my_menu, REQ_DOWN_ITEM);
            break;
        case KEY_UP:
            menu_driver(my_menu, REQ_UP_ITEM);
            break;
        case 10: // Enter key
            choice_index = item_index(current_item(my_menu));
            break;
        }
        if (choice == 10)
            break;
    }

    // Clean up and exit
    unpost_menu(my_menu);
    free_menu(my_menu);
    for (int i = 0; i < n_choices; ++i) {
        free_item(my_items[i]);
    }
    wrefresh(menu_win);
    delwin(menu_win);
    refresh();
    return choice_index;
}

string CmdMenu::ReplaceArgs(const string &ori,
                            const map<string, string> &args) {
    string ret = ori;
    for (const auto &item : args) {
        std::regex p("\\{\\{" + item.first + "\\}\\}");
        ret = std::regex_replace(ret, p, item.second);
    }
    return ret;
}

void CmdMenu::SetTitle(const string &title, const map<string, string> &args) {
    this->title = title;
    this->args.insert(args.begin(), args.end());
}

void CmdMenu::SetArg(const string &arg_name, const string &val) {
    this->args[arg_name] = val;
}

void CmdMenu::SetArg(const string &arg_name, const char *val) {
    this->args[arg_name] = val;
}

void CmdMenu::SetArg(const string &arg_name, int val) {
    this->args[arg_name] = std::to_string(val);
}

void CmdMenu::SetDesc(const string &desc, const map<string, string> &args) {
    this->desc = desc;
    this->args.insert(args.begin(), args.end());
}

size_t CmdMenu::Size() const { return this->menu_items.size(); }
