/*****************************************************************************************************************
* Copyright (c) 2014 Khalid Ali Al-Kooheji                                                                       *
*                                                                                                                *
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and              *
* associated documentation files (the "Software"), to deal in the Software without restriction, including        *
* without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell        *
* copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the       *
* following conditions:                                                                                          *
*                                                                                                                *
* The above copyright notice and this permission notice shall be included in all copies or substantial           *
* portions of the Software.                                                                                      *
*                                                                                                                *
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT          *
* LIMITED TO THE WARRANTIES OF MERCHANTABILITY, * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.          *
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, * DAMAGES OR OTHER LIABILITY,      *
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE            *
* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                                                         *
*****************************************************************************************************************/
#pragma once

namespace ve {

enum ActionStatus {
  kActionStatusStopped = 0,
  kActionStatusRunning = 1,
  kActionStatusFinished = 2,
};

class Action {
 public:
  Action(float duration) : duration_(duration),status_(kActionStatusStopped) {}
  virtual void Update(float dt) = 0;
  ActionStatus status() const { return status_; }
  void Run() {
    time_ = 0;
    status_ = kActionStatusRunning;
  }
  void Stop() {
    status_ = kActionStatusStopped;
  }
 protected:
  void* target_;
  float duration_;
  float time_;
  ActionStatus status_;
};

class CustomAction : Action {
 public:
  typedef std::function<void(float)> UpdateFunc;
  CustomAction(const  UpdateFunc& on_update, float duration) : on_update_(on_update), Action(duration)  {
  
  }
  virtual void Update(float dt) {
    if (time_ <= duration_) {
      on_update_(dt);
      time_ += dt;
    } else {
      status_ = kActionStatusFinished;
    }
  }
 protected:
  std::function<void(float)> on_update_; 
};


class ActionManager {
 public:
  ActionManager() : context_(NULL)  {}
  virtual ~ActionManager() {}
  void Update(float dt);
protected:
  Context* context_;
  std::vector<Action*> actions_list_;
  
};

}

