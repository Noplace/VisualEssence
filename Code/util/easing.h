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

/*
t : current time
d : duration
b : start 
c : end
*/

inline float linearTween(float t, float b, float c, float d) {
	return c*t/d + b;
}

inline float easeInQuad(float t, float b, float c, float d) {
  t /= d;
	return c*t*t + b;
}

inline float easeOutQuad(float t, float b, float c, float d) {
	t /= d;
	return -c * t*(t-2) + b;
}

inline float easeInOutQuad(float t, float b, float c, float d) {
	t /= d/2;
	if (t < 1) return c/2*t*t + b;
	t--;
	return -c/2 * (t*(t-2) - 1) + b;
}

inline float easeInCubic(float t, float b, float c, float d) {
	t /= d;
	return c*t*t*t + b;
}

inline float easeOutCubic(float t, float b, float c, float d) {
	t /= d;
	t--;
	return c*(t*t*t + 1) + b;
}

inline float easeInOutCubic(float t, float b, float c, float d) {
	t /= d/2;
	if (t < 1) return c/2*t*t*t + b;
	t -= 2;
	return c/2*(t*t*t + 2) + b;
}
	
inline float easeInQuart(float t, float b, float c, float d) {
	t /= d;
	return c*t*t*t*t + b;
}

inline float easeOutQuart(float t, float b, float c, float d) {
	t /= d;
	t--;
	return -c * (t*t*t*t - 1) + b;
}

inline float easeInOutQuart(float t, float b, float c, float d) {
	t /= d/2;
	if (t < 1) return c/2*t*t*t*t + b;
	t -= 2;
	return -c/2 * (t*t*t*t - 2) + b;
}

inline float easeInQuint(float t, float b, float c, float d) {
	t /= d;
	return c*t*t*t*t*t + b;
}

inline float easeOutQuint(float t, float b, float c, float d) {
	t /= d;
	t--;
	return c*(t*t*t*t*t + 1) + b;
}

inline float easeInOutQuint(float t, float b, float c, float d) {
	t /= d/2;
	if (t < 1) return c/2*t*t*t*t*t + b;
	t -= 2;
	return c/2*(t*t*t*t*t + 2) + b;
}
	
inline float easeInSine(float t, float b, float c, float d) {
	return -c * cosf(t/d * (dx::XM_PI/2)) + c + b;
}

inline float easeOutSine(float t, float b, float c, float d) {
	return c * sinf(t/d * (dx::XM_PI/2)) + b;
}

inline float easeInOutSine(float t, float b, float c, float d) {
	return -c/2 * (cosf(dx::XM_PI*t/d) - 1) + b;
}

inline float easeInExpo(float t, float b, float c, float d) {
	return c *  powf( 2, 10 * (t/d - 1) ) + b;
}

inline float easeOutExpo(float t, float b, float c, float d) {
	return c * ( -powf( 2, -10 * t/d ) + 1 ) + b;
}

inline float easeInOutExpo(float t, float b, float c, float d) {
	t /= d/2;
	if (t < 1) return c/2 * powf( 2, 10 * (t - 1) ) + b;
	t--;
	return c/2 * ( -powf( 2, -10 * t) + 2 ) + b;
}

inline float easeInCirc(float t, float b, float c, float d) {
	t /= d;
	return -c * (sqrtf(1 - t*t) - 1) + b;
}

inline float easeOutCirc(float t, float b, float c, float d) {
	t /= d;
	t--;
	return c * sqrtf(1 - t*t) + b;
}

	
inline float easeInOutCirc(float t, float b, float c, float d) {
	t /= d/2;
	if (t < 1) return -c/2 * (sqrtf(1 - t*t) - 1) + b;
	t -= 2;
	return c/2 * (sqrtf(1 - t*t) + 1) + b;
}

}

