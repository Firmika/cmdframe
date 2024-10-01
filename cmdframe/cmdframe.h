#ifndef __CMDFRAME_H__
#define __CMDFRAME_H__

#include <functional>

using finf_id_t = int;
#define FINF_BACK 0
#define FINF_SELF -1
#define FINF_EXIT -2
#define FINF_FRAME(n) ((n) + 1)

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
     * @param return_key The next Frame to enter after execution.
     * possible values: NKEY_BACK, NKEY_SELF, NKEY_EXIT, NKEY_FRAME(n)
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
    inline void SetInitType(InitType init_type) { this->init_type = init_type; }
    /**
     * @brief Start execution of a frame.
     * @details Execute hander and return by means of the `next_key`.
     */
    finf_id_t Execute() const;

  public:
    template <typename Function, typename... Args>
    /**
     * @brief Create a frame with a function.
     * @note func must be a function that returns int
     * To bind a member functin, use `std::bind`.
     * To bind a argument as reference, use `std::ref`.
     */
    CmdFrame(Function &&func, Args &&...args) {
        static_assert(std::is_invocable_r<int, Function, Args...>::value,
                      "Function must return int");
        frame_main = std::bind(std::forward<Function>(func),
                               std::forward<Args>(args)...);
    }
    /**
     * @brief Create a frame with a function.
     * @note func must be a function that returns void
     * To bind a member functin, use `std::bind`.
     * To bind a argument as reference, use `std::ref`.
     * @param return_key The next Frame to enter after execution.
     * possible values: NKEY_BACK, NKEY_SELF, NKEY_EXIT, NKEY_FRAME(n)
     */
    template <typename Function, typename... Args>
    CmdFrame(int return_key, Function &&func, Args &&...args) {
        static_assert(std::is_invocable_r<void, Function, Args...>::value,
                      "Function must return void");
        frame_main = [return_key, func, args...]() {
            func(args...);
            return return_key;
        };
    }
    // CmdFrame(const CmdFrame &other) = delete;
    virtual ~CmdFrame() {}

  private:
    InitType init_type{InitType::DEFAULT};
    std::function<int()> frame_main;
};
#endif