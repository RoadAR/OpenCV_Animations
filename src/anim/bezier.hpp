//
//  spring.hpp
//  OpenCV_Anim
//
//  Created by Alexander Graschenkov on 15.03.2021.
//

#pragma once

#include <stdio.h>
#include <opencv2/core.hpp>

namespace ui {

class Bezier {
public:
  Bezier(cv::Point2f p1, cv::Point2f p2);
  Bezier(float x1, float x2); // y1 = 0, y2 = 1
  Bezier();
  
  static Bezier linear()    { return Bezier(0, 1); };
  static Bezier easeIn()    { return Bezier(0.42, 1); };
  static Bezier easeOut()   { return Bezier(0, 0.58); };
  static Bezier easeInOut() { return Bezier(0.42, 0.58); };
  
  float operator()(float x);
  
private:
  cv::Point2f p1_, p2_;
  bool isLinear_ = false;
};

} // namespace ui
