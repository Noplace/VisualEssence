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
#include "../ve.h"

namespace ve {

void ActionManager::Run(Action* action) {
  if (action->status() == kActionStatusRunning) return;
  action->set_status(kActionStatusRunning);
  running_actions_list_.push_back(action);
}

void ActionManager::RunAfter(Action* action, float delay) {
  if (action->status() == kActionStatusRunning) return;
  action->set_delay(time_+delay);
  pending_actions_list_.push_back(action);
}

void ActionManager::Update(float dt) {

  pending_actions_list_.erase(
      std::remove_if(
        pending_actions_list_.begin(), 
        pending_actions_list_.end(), 
        [this](Action* action) { 
          if (time_ >= action->delay()) {
            Run(action);
            return true;
          } else {
            return false;
          }
        }),
      pending_actions_list_.end());


  for(auto i: running_actions_list_) {
    i->Update(dt);
  }

  time_ += dt;

  //remove actions that have finished
  running_actions_list_.erase(
      std::remove_if(
        running_actions_list_.begin(), 
        running_actions_list_.end(), 
        [](const Action* action) { 
          return  action->status() == kActionStatusFinished;
      }),
      running_actions_list_.end());

}



}