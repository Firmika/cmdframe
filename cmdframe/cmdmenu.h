#ifndef __CMDMENU_H__
#define __CMDMENU_H__
#include "cmdframe.h"
#include <map>
#include <string>
#include <tuple>
#include <vector>
using std::map;
using std::pair;
using std::tuple;
using std::vector;

using item_id_t = int;
using mevent_id_t = int;
#define MEVENT_CONTINUE 0
#define MEVENT_CANCEL -1
#define MEVENT_REDIRECT(n) ((n) + 1)

/**
 * @class CmdMenu
 * @brief Special menu-typed CmdFrame.
 * @details Enable user defined menu items
 *        and easy usable selection system.
 */
class CmdMenu : public CmdFrame {
  public:
    CmdMenu(vector<std::string> &&item_descs = {}, std::string &&title = "",
            std::string &&desc = "", bool memorize = false,
            std::function<void()> on_enter = nullptr,
            std::function<void()> on_exit = nullptr);
    /**
     * @brief Up-Level: set menu items and events.
     * @note return value of event should be next key redirecting to.
     * `MEVENT_CONTINUE`: Default output interface
     * `MEVENT_CANCEL`: Stay in current menu
     * `MEVENT_REDIRECT(n)`: Redirect to other output interface
     */
    CmdMenu(vector<pair<std::string, std::function<int()>>> &&menu_event_pairs,
            std::string &&title = "", std::string &&desc = "",
            bool memorize = false, std::function<void()> on_enter = nullptr,
            std::function<void()> on_exit = nullptr);
    virtual ~CmdMenu() {}

    void Append(std::string &&desc);
    void Append(std::string &&desc, std::function<int()> event);
    template <typename Function, typename... Args>
    inline void Append(std::string &&desc, Function &&func, Args &&...args) {
        static_assert(std::is_invocable_r<void, Function, Args...>::value,
                      "Function must return void");
        Append(std::forward<std::string>(desc),
               std::bind(std::forward<Function>(func),
                         std::forward<Args>(args)...));
    }

    void SetTitle(const std::string &title,
                  const map<std::string, std::string> &args = {});
    void SetDesc(const std::string &desc,
                 const map<std::string, std::string> &args = {});
    void SetArg(const std::string &arg_name, const std::string &val);
    void SetArg(const std::string &arg_name, const char *val);
    void SetArg(const std::string &arg_name, int val);
    /**
     * @brief Bind a function as menu on enter.
     * @note func must be a function that returns void
     * To bind a member functin, use `std::bind`.
     * To bind a argument as reference, use `std::ref`.
     */
    template <typename Function, typename... Args>
    void BindOnEnter(Function &&func, Args &&...args) {
        static_assert(std::is_invocable_r<void, Function, Args...>::value,
                      "Function must return void");
        this->on_enter = std::bind(std::forward<Function>(func),
                                   std::forward<Args>(args)...);
    }
    /**
     * @brief Bind a function as menu on exit.
     * @note func must be a function that returns void
     * To bind a member functin, use `std::bind`.
     * To bind a argument as reference, use `std::ref`.
     */
    template <typename Function, typename... Args>
    void BindOnExit(Function &&func, Args &&...args) {
        static_assert(std::is_invocable_r<void, Function, Args...>::value,
                      "Function must return void");
        this->on_exit = std::bind(std::forward<Function>(func),
                                  std::forward<Args>(args)...);
    }
    /**
     * @return The size of menu items.
     */
    size_t Size() const;

  protected:
    inline std::string GetItemDesc(int index) const {
        return menu_items[index].desc;
    }
    inline std::string GetReplacedTitle() const {
        return ReplaceArgs(title, args);
    }
    inline std::string GetReplacedDesc() const {
        return ReplaceArgs(desc, args);
    }
    /**
     * @note To modify the style of the menu, just override this method.
     */
    virtual item_id_t DisplayAndSelect();

  private:
    finf_id_t MenuMain();
    static std::string ReplaceArgs(const std::string &ori,
                                   const map<std::string, std::string> &args);

  private:
    /**
     * @struct MenuItem
     * @brief Definition of a menu item
     *        with item desc, event.
     */
    struct MenuItem {
        std::string desc;
        std::function<int()> event;
        MenuItem() {}
        MenuItem(const std::string &desc, const std::function<int()> &event)
            : desc(desc), event(event) {}
    };
    using ArgList = map<std::string, std::string>;

  private:
    vector<MenuItem> menu_items;
    std::string title, desc;
    ArgList args;
    std::function<void()> on_enter, on_exit;

  protected:
    bool memorize;
    int cur_index = 0;
};
#endif