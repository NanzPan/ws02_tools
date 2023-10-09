//
// Copyright (c) 2022 ECOVACS
//
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT
//

#pragma once

#include <string>
#include <functional>

using Handler = std::function<bool(std::function<int(uint8_t *, int)>)>;
using CtlRunHandler = std::function<bool()>;

class IWorker {
public:
    virtual ~IWorker() = default;

    virtual bool Launch(Handler handler, bool detached, int sync_interval, int sync_timeout) = 0;
    virtual bool Write(const std::string &cmd) = 0;
    virtual bool Write(unsigned char* cmd, size_t len) = 0;
    virtual bool GetCtlRun() = 0;
    virtual bool SetCtlRunHandler(CtlRunHandler handler) = 0;
};