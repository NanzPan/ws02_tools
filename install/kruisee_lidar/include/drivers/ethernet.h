//
// Copyright (c) 2021 ECOVACS
//
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT
//

#pragma once

#include <memory>
#include <atomic>
#include <string>
#include <map>
#include <mutex>

#include <time.h>
#include <sys/time.h>

#include "drivers/iworker.h"

typedef struct _LidarInfo
{   
    uint64_t pre_recv_time;
    uint64_t cur_recv_time;
    uint64_t sync_end_time;
    uint64_t send_cmd_time;

    int sync_state;
    int sync_retry;

    unsigned int sync_interval;
    unsigned int sync_timeout;

    Handler hfunc;
    CtlRunHandler crhfunc;
    
    uint64_t last_pub_time;
}LidarInfo;


class Ethernet : public IWorker {
public:
    static std::unique_ptr<Ethernet> Create(uint16_t listen_port, const std::string &ip, uint16_t port);
    ~Ethernet();

    bool Launch(Handler handler, bool detached, int sync_interval, int sync_timeout) override;
    bool Write(const std::string &cmd) override;
    bool Write(unsigned char* cmd, size_t len) override;
    bool Write(std::string ip, char* cmd, size_t len);
    bool GetCtlRun() override;
    bool SetCtlRunHandler(CtlRunHandler handler) override;

private:
    Ethernet();
    Ethernet(std::string ip);
    bool Init(uint16_t listen_port);
    void Handle(int fd);

private:
    std::string ip_;
    uint16_t port_;

    Handler handler_;
    CtlRunHandler ctl_run_handler_;
    
    std::atomic<bool> quit_;
    std::atomic<bool> try_to_quit_;

    static int fd_;

    static std::mutex mtx_;
    static std::map<std::string, LidarInfo> handlers_;    

};


