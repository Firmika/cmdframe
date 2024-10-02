/**
 * @file cmdframe_manager.h
 * @brief cmdframe related classes
 *
 * @details
 * Related Class:
 *    CmdFrame: General purpose frame
 *    CmdMenu: Particular frame tailored for menu
 *    CmdFrameManager: Manager for frames
 * Related Namespace:
 *    cmdf: Frame related key words
 *
 * @author Firmika
 * @date 19 Apr 2024
 */
#ifndef __CMDFRAME_MANAGER_H__
#define __CMDFRAME_MANAGER_H__
#include "cmdframe.h"
#include <map>
#include <string>
#include <tuple>
#include <vector>

using frame_id_t = int;
#define FID_BACK -1
#define FID_EXIT -2

/**
 * @class CmdFrameManager
 * @brief Manager for multiple frames(including menus).
 */
class CmdFrameManager {
  public:
    CmdFrameManager() {}
    CmdFrameManager(std::vector<CmdFrame *> frames = {},
                    bool manage_mem = false, frame_id_t enter_frame_id = 0)
        : frames(frames), manage_mem(manage_mem),
          enter_frame_id(enter_frame_id) {
        frame_keys.resize(frames.size());
        links.resize(frames.size());
    }
    ~CmdFrameManager() {
        if (manage_mem)
            for (auto &&frame : frames)
                delete frame;
    }
    void Append(CmdFrame *frame);
    void Append(const std::string &name, CmdFrame *frame);
    void Append(std::vector<CmdFrame *> frames);
    void Append(std::vector<std::pair<std::string, CmdFrame *>> frames);
    void SetFrameKey(frame_id_t fid, const std::string &key);
    inline void SetEnterFrameID(int enter_frame_id) {
        this->enter_frame_id = enter_frame_id;
    }
    void SetEnterFrameID(const std::string &key);
    inline void Link(frame_id_t ffid, finf_id_t finfid, frame_id_t tfid) {
        links[ffid][finfid] = tfid;
    }
    void Link(std::string &&fkey, finf_id_t finfid, frame_id_t tfid);
    void Link(frame_id_t ffid, finf_id_t finfid, std::string &&tkey);
    void Link(std::string &&fkey, finf_id_t finfid, std::string &&tkey);
    template <typename FT, typename TT>
    void Link(std::vector<std::tuple<FT, finf_id_t, TT>> links) {
        for (auto &&link : links)
            Link((FT)std::get<0>(link), (finf_id_t)std::get<1>(link), (TT)std::get<2>(link));
    }
    /**
     * @brief Get number of frames managed by manager.
     * @return Number of frames managed by manager.
     */
    inline size_t Size() const { return frames.size(); }
    void Execute();

  private:
    std::vector<CmdFrame *> frames;
    std::map<std::string, frame_id_t> fkey_to_id;
    std::vector<std::string> frame_keys;
    std::vector<std::map<finf_id_t, frame_id_t>> links;
    frame_id_t enter_frame_id{0};
    bool manage_mem;
};

template void CmdFrameManager::Link<frame_id_t, frame_id_t>(
    std::vector<std::tuple<frame_id_t, finf_id_t, frame_id_t>> links);
template void CmdFrameManager::Link<std::string, frame_id_t>(
    std::vector<std::tuple<std::string, finf_id_t, frame_id_t>> links);
template void CmdFrameManager::Link<frame_id_t, std::string>(
    std::vector<std::tuple<frame_id_t, finf_id_t, std::string>> links);
template void CmdFrameManager::Link<std::string, std::string>(
    std::vector<std::tuple<std::string, finf_id_t, std::string>> links);
#endif
