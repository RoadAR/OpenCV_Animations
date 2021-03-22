//
//  spring.cpp
//  OpenCV_Anim
//
//  Created by Alexander Graschenkov on 15.03.2021.
//

#include "bezier.hpp"

// from Telegram iOS app

namespace BezierSpace {

inline float a(float a1, float a2) {
  return 1.0 - 3.0 * a2 + 3.0 * a1;
}

inline float b(float a1, float a2) {
  return 3.0 * a2 - 6.0 * a1;
}

inline float c(float a1) {
  return 3.0 * a1;
}

float calculateBezier(float t, float a1, float a2) {
  return ((a(a1, a2)*t + b(a1, a2))*t + c(a1)) * t;
}

float calcSlope(float t, float a1, float a2) {
  return 3.0 * a(a1, a2) * t * t + 2.0 * b(a1, a2) * t + c(a1);
}

float getTForX(float x, float x1, float x2) {
  float t = x;
  for (int i = 0; i < 4; i++) {
    float currentSlope = calcSlope(t, x1, x2);
    if (currentSlope == 0.0) {
      return t;
    } else {
      float currentX = calculateBezier(t, x1, x2) - x;
      t -= currentX / currentSlope;
    }
  }
  
  return t;
}

float bezierPoint(cv::Point2f p1, cv::Point2f p2, float x) {
  float value = calculateBezier(getTForX(x, p1.x, p2.x), p1.y, p2.y);
  if (value >= 0.997) {
    value = 1.0;
  }
  return value;
}

} // namespace BezierSpace


Bezier::Bezier(cv::Point2f p1, cv::Point2f p2): p1_(p1), p2_(p2) {
  isLinear_ = (p1_ == cv::Point2f(0,0)) && (p2_ == cv::Point2f(1, 1));
}

float Bezier::operator()(float x) {
  if (isLinear_) {
    return x;
  } else {
    return BezierSpace::bezierPoint(p1_, p2_, x);
  }
}
