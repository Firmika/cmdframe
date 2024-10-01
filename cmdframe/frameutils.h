#ifndef __FRAMEUTILS_H__
#define __FRAMEUTILS_H__

#include <string>
#include <vector>

namespace frameutils {

std::string Trim(std::string str);
/**
 * @brief General purpose pause of execution.
 * @details Waiting user interaction.
 * @param prompt displayed when pausing.
 */
void Pause(const std::string &prompt = "Press any key to continue...");
/**
 * @brief Clear texts on terminal.
 */
void Clear();
/**
 * @brief Waiting user to input y(yes) or n(no) and return as boolean.
 * @param prompt displayed before user interaction.
 */
bool Confirm(const std::string &prompt = "");
/**
 * @brief List an option list waiting user interaction.
 * @param options option list which will be shown by number order.
 * @param prompt prompt for user input.
 * @param inputList extra user input std::string(lowercase) that can pass the
 * examination, and will be returned by a minus number indicating the index
 * (0, -1, -2,
 * ...). Default: the number of options.
 * @return the number of option the user input.
 */
int Option(const std::vector<std::string> &options,
           const std::string &prompt = "",
           const std::vector<std::string> &inputList = {});
int Getch();

} // namespace frameutils

#endif