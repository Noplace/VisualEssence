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
  Action() : status_(kActionStatusStopped) {}
  virtual ~Action() {}
  virtual void Update(float dt) = 0;
  ActionStatus status() const { return status_; }
  void set_status(ActionStatus status) { status_ = status; }
  float delay() const { return delay_; }
  void set_delay(float delay) { delay_ = delay; }
 protected:
  void* target_;
  ActionStatus status_;
  float delay_;
};

class SequenceAction : public Action {
 public:
  SequenceAction(std::initializer_list<Action*> actions) : Action() {
    for (auto i: actions)
      actions_.push_back(i);
    
  }
  
  void Update(float dt) {
    auto cur = actions_.back();
    cur->Update(dt);
    if (cur->status() == kActionStatusFinished) {
      actions_.pop_back();
      if (actions_.size() == 0)
        status_ = kActionStatusFinished;
    }
  }
 protected:
  std::vector<Action*> actions_;
};

class ParallelAction : public Action {
 public:
  ParallelAction(std::initializer_list<Action*> actions) : Action() {
    for (auto i: actions)
      actions_.push_back(i);
    
  }
  
  void Update(float dt) {
    bool done = true;
    for (auto i: actions_) {
      i->Update(dt);
      if (i->status() != kActionStatusFinished)
        done = false;
    }
    if (done == true)
      status_ = kActionStatusFinished;


  }
 protected:
  std::vector<Action*> actions_;
};


class TimeBoundAction : public Action {
 public:
  TimeBoundAction(float duration) : duration_(duration),Action() {}
  virtual ~TimeBoundAction() {}
  void Update(float dt) {
    if (time_ <= duration_) {
      OnUpdate(dt);
      time_ += dt;
    } else {
      status_ = kActionStatusFinished;
    }
  }
  virtual void OnUpdate(float dt) = 0;
 protected:
  float duration_;
  float time_;
};

class DelayAction : public TimeBoundAction {
 public:
  typedef std::function<void(float)> UpdateFunc;
  DelayAction(float duration) : TimeBoundAction(duration)  {
  
  }
  virtual void OnUpdate(float dt) {

  }
 protected:

};


class CustomTimeBoundAction : public TimeBoundAction {
 public:
  typedef std::function<void(float)> UpdateFunc;
  CustomTimeBoundAction(const  UpdateFunc& on_update, float duration) : on_update_(on_update), TimeBoundAction(duration)  {
  
  }
  virtual void OnUpdate(float dt) {
    on_update_(dt);
  }
 protected:
  UpdateFunc on_update_; 
};


class ActionManager : public Component {
 public:
  ActionManager() : context_(NULL)  {}
  virtual ~ActionManager() {}
  void Run(Action* action);
  void RunAfter(Action* action, float delay);
  void Update(float dt);
  
 protected:
  Context* context_;
  std::vector<Action*> pending_actions_list_;
  std::vector<Action*> running_actions_list_;
  float time_;
};

}

