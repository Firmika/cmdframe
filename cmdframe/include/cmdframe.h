#ifndef __CMDFRAME_H__
#define __CMDFRAME_H__

#include <functional>
#include <string>
#include <vector>

using std::function;
using std::string;

#define NKEY_BACK 0
#define NKEY_SELF -1
#define NKEY_EXIT -2
#define NKEY_FRAME(n) ((n) + 1)

/**
 * @class CmdFrame
 * @brief Basic element of command line frame.
 */
class CmdFrame {
  public:
    enum class InitType { DEFAULT, CLEAR, CURSE };

  public:
    /**
     * @brief Bind a function as a frame main.
     * @note func must be a function that returns int.
     * To bind a member functin, use `std::bind`.
     * To bind a argument as reference, use `std::ref`.
     */
    template <typename Function, typename... Args>
    void Bind(Function &&func, Args &&...args) {
        static_assert(std::is_invocable_r<int, Function, Args...>::value,
                      "Function must return int");
        frame_main = std::bind(std::forward<Function>(func),
                               std::forward<Args>(args)...);
    }
    /**
     * @brief Bind a function as a frame main.
     * @note func must be a function that returns void
     * To bind a member functin, use `std::bind`.
     * To bind a argument as reference, use `std::ref`.
     */
    template <typename Function, typename... Args>
    void Bind(int return_key, Function &&func, Args &&...args) {
        static_assert(std::is_invocable_r<void, Function, Args...>::value,
                      "Function must return void");
        frame_main = [return_key, func, args...]() {
            func(args...);
            return return_key;
        };
    }
    /**
     * @brief Set init type of the frame.
     */
    void SetInitType(InitType init_type) { this->init_type = init_type; }
    /**
     * @brief Start execution of a frame.
     * @details Execute hander and return by means of the `next_key`.
     */
    int Execute() const;

  public:
    /**
     * @brief Default constructor.
     * @details Create an empty frame(do nothing). `init_type` is DEFAULT.
     */
    CmdFrame();
    explicit CmdFrame(InitType init_type) : init_type(init_type) {}
    template <typename Function, typename... Args>
    CmdFrame(Function &&func, Args &&...args) {
        static_assert(std::is_invocable_r<int, Function, Args...>::value,
                      "Function must return int");
        frame_main = std::bind(std::forward<Function>(func),
                               std::forward<Args>(args)...);
    }
    template <typename Function, typename... Args>
    CmdFrame(int return_key, Function &&func, Args &&...args) {
        static_assert(std::is_invocable_r<void, Function, Args...>::value,
                      "Function must return void");
        frame_main = [return_key, func, args...]() {
            func(args...);
            return return_key;
        };
    }
    CmdFrame(const CmdFrame &other) = delete;
    virtual ~CmdFrame();

  private:
    InitType init_type{InitType::DEFAULT};
    function<int()> frame_main;
};
#endif