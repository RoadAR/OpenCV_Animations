//
//  animator.cpp
//  OpenCV_Anim
//
//  Created by Alexander Graschenkov on 21.03.2021.
//

#include "animator.hpp"
#include <opencv2/imgproc.hpp>

using namespace cv;



UIAnimator::UIAnimator(float *value, float to, float time, Bezier curve, float *from): time_(time), curve_(curve) {
  setup(value, from, &to, 1);
}

UIAnimator::UIAnimator(float *value, float *to, int length, float time, Bezier curve, float *from): time_(time), curve_(curve) {
  setup(value, from, to, length);
}

UIAnimator::UIAnimator(Color *value, Color to, float time, Bezier curve, Color *from): time_(time), curve_(curve) {
  setup((float*)value, (float*)from, (float*)&to, 4);
  customInterpolate = [](float *value, float *from, float *to, int length, float percent){
    *(Color*)value = ((Color*)from)->interpolate(*((Color*)to), percent);
  };
}

UIAnimator::UIAnimator(Rect2f *value, Rect2f to, float time, Bezier curve, Rect2f *from): time_(time), curve_(curve) {
  setup((float*)value, (float*)from, (float*)&to, 4);
}


UIAnimator::UIAnimator(cv::Point2f *value, cv::Point2f to, float time, Bezier curve, cv::Point2f *from): time_(time), curve_(curve) {
  setup((float*)value, (float*)from, (float*)&to, 2);
}

void UIAnimator::setup(float *value, float *from, float *to, int length) {
  value_ = value;
  from = from ?: value; // if from value is missing, use current value
  from_ = std::vector<float>(from, from+length);
  to_ = std::vector<float>(to, to+length);
  prevValue_ = std::vector<float>(value, value+length);
  
  if (to_ == prevValue_) {
    finished_ = true;
  }
}

void UIAnimator::tick(float dt) {
  if (finished_) return;
  
  // check is other code was changed this value
  for (int i = 0; i < prevValue_.size(); i++) {
    if (prevValue_[i] != value_[i]) {
      finished_ = true;
      return;
    }
  }
  
  // get percent of animation
  currTime_ += dt;
  float percent = currTime_ / time_;
  if (percent >= 1) {
    percent = 1;
    finished_ = true;
  }
  percent = curve_(percent);
  
  // perform animation
  if (customInterpolate) {
    customInterpolate(value_, &from_[0], &to_[0], (int)from_.size(), percent);
  } else {
    interpolate(value_, &from_[0], &to_[0], (int)from_.size(), percent);
  }
  prevValue_ = std::vector<float>(value_, value_+prevValue_.size());
}

void UIAnimator::interpolate(float *value, float *from, float *to, int length, float percent) {
  for (int i = 0; i < length; i++) {
    value[i] = (to[i] - from[i]) * percent + from[i];
  }
}

UIAnimator &UIAnimator::setName(const std::string &name) {
  name_ = name;
  return *this;
}
