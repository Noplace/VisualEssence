/*****************************************************************************************************************
* Copyright (c) 2012 Khalid Ali Al-Kooheji                                                                       *
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

class Timer 
{
public:
	// Initializes internal timer values.
	Timer()
	{
		if (!QueryPerformanceFrequency(&frequency_))
		{
		  return;
		}
		Reset();
	}
	
	// Reset the timer to initial values.
	void Reset()
	{
		Update();
		startTime_ = currentTime_;
		total_ = 0.0f;
		delta_ = 1.0f / 60.0f;
	}
	
	// Update the timer's internal values.
	void Update()
	{
		if (!QueryPerformanceCounter(&currentTime_))
		{
			return;
		}
		
		total_ = static_cast<float>(
			static_cast<double>(currentTime_.QuadPart - startTime_.QuadPart) /
			static_cast<double>(frequency_.QuadPart)
			);
		
		if (lastTime_.QuadPart == startTime_.QuadPart)
		{
			// If the timer was just reset, report a time delta equivalent to 60Hz frame time.
			delta_ = 1.0f / 60.0f;
		}
		else
		{
			delta_ = static_cast<float>(
				static_cast<double>(currentTime_.QuadPart - lastTime_.QuadPart) /
				static_cast<double>(frequency_.QuadPart)
				);
		}
		
		lastTime_ = currentTime_;
	}
	
  float total() { return total_; }
  float delta() { return delta_; }

private:
	LARGE_INTEGER frequency_;
	LARGE_INTEGER currentTime_;
	LARGE_INTEGER startTime_;
	LARGE_INTEGER lastTime_;
	float total_;
	float delta_;
};

}