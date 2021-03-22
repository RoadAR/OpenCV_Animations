//
//  spring.hpp
//  OpenCV_Anim
//
//  Created by Alexander Graschenkov on 15.03.2021.
//

#pragma once

#include <stdio.h>
#include <opencv2/core.hpp>


class Bezier {
public:
  Bezier(cv::Point2f p1, cv::Point2f p2);
  
  static Bezier linear()    { return Bezier({0,0}, {1,1}); };
  static Bezier easeIn()    { return Bezier({0.42,0}, {1,1}); };
  static Bezier easeOut() { return Bezier({0,0}, {0.58,1}); };
  static Bezier easeInOut() { return Bezier({0.42,0}, {0.58,1}); };
  
  float operator()(float x);
  
private:
  cv::Point2f p1_, p2_;
  bool isLinear_ = false;
};
