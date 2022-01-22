#ifndef TBOX_EVENT_COMMON_LOOP_H_20170713
#define TBOX_EVENT_COMMON_LOOP_H_20170713

#include <deque>
#include <mutex>
#include <thread>

#include "loop.h"

#ifdef ENABLE_STAT
#include <chrono>
using std::chrono::steady_clock;
#endif

namespace tbox {
namespace event {

class CommonLoop : public Loop {
  public:
    CommonLoop();
    ~CommonLoop() override;

  public:
    bool isInLoopThread() override;
    bool isRunning() const override;
    void runInLoop(const Func &func) override;
    void runNext(const Func &func) override;

    Stat getStat() const override;
    void resetStat() override;

  public:
#ifdef ENABLE_STAT
    void recordTimeCost(uint64_t cost_us);
#endif  //ENABLE_STAT
    void handleNextFunc();

  protected:
    void runThisBeforeLoop();
    void runThisAfterLoop();

    void onGotRunInLoopFunc(short);

    void cleanupDeferredTasks();

    void commitRequest();
    void finishRequest();

  private:
    mutable std::mutex lock_;

    std::thread::id loop_thread_id_;

    bool has_unhandle_req_;
    int read_fd_, write_fd_;
    FdEvent *sp_read_event_;

    std::deque<Func> run_in_loop_func_queue_;
    std::deque<Func> run_next_func_queue_;

#ifdef ENABLE_STAT
    steady_clock::time_point stat_start_;
    uint64_t time_cost_us_ = 0;
    uint32_t event_count_ = 0;
    uint32_t max_cost_us_ = 0;
#endif  //ENABLE_STAT

    int cb_level_;
};

}
}

#endif //TBOX_EVENT_COMMON_LOOP_H_20170713
