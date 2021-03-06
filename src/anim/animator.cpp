//
//  animator.cpp
//  OpenCV_Anim
//
//  Created by Alexander Graschenkov on 21.03.2021.
//

#include "animator.hpp"
#include <opencv2/imgproc.hpp>

using namespace cv;


namespace ui {

Animator::Animator(Color *value, const Color &to, float time): time_(time) {
  setup((float*)value, (float*)&to, 4);
  customInterpolate = [](float *value, float *from, float *to, int length, float percent){
    *(Color*)value = ((Color*)from)->interpolate(*((Color*)to), percent, false);
  };
}


void Animator::setup(float *value, float *to, int length) {
  value_ = value;
  to_ = std::vector<float>(to, to+length);
  prevValue_ = std::vector<float>(value, value+length);
  
//  if (to_ == prevValue_) {
//    finished_ = true;
//  }
}

void Animator::tick(float dt) {
  if (finished_) {
    return;
  }
  if (delay_ > 0) {
    delay_ -= dt;
    prevValue_ = std::vector<float>(value_, value_+prevValue_.size());
  }
  if (delay_ > 0) {
    return;
  }
  if (from_.empty()) {
    from_ = std::vector<float>(value_, value_+to_.size());
  }
  
  
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
  if (prevValue_ == to_) {
    finished_ = true;
  }
}

void Animator::interpolate(float *value, float *from, float *to, int length, float percent) {
  for (int i = 0; i < length; i++) {
    value[i] = (to[i] - from[i]) * percent + from[i];
  }
}

Animator &Animator::setName(const std::string &name) {
  name_ = name;
  return *this;
}

Animator &Animator::setCurve(const Bezier &curve) {
  curve_ = curve;
  return *this;
}

Animator &Animator::setDelay(float delay) {
  delay_ = delay;
  return *this;
}

} // namespace ui
