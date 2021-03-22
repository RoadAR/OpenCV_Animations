//
//  animator.hpp
//  OpenCV_Anim
//
//  Created by Alexander Graschenkov on 21.03.2021.
//

#ifndef animator_hpp
#define animator_hpp

#include <stdio.h>
#include "bezier.hpp"
#include "color.hpp"



class UIAnimator {
public:
  UIAnimator(float *value, float to, float time, Bezier curve = Bezier::linear(), float *from = nullptr);
  
  UIAnimator(float *value, float *to, int length, float time, Bezier curve = Bezier::linear(), float *from = nullptr);
  
  UIAnimator(Color *value, Color to, float time, Bezier curve = Bezier::linear(), Color *from = nullptr);
  
  UIAnimator(cv::Rect2f *value, cv::Rect2f to, float time, Bezier curve = Bezier::linear(), cv::Rect2f *from = nullptr);
  
  UIAnimator(cv::Point2f *value, cv::Point2f to, float time, Bezier curve = Bezier::linear(), cv::Point2f *from = nullptr);
  
  void tick(float dt);
  bool isFinished() const { return finished_; }
  
  
  UIAnimator &setName(const std::string &name);
  std::string getName() const { return name_; }
  
private:
  bool finished_ = false;
  float *value_, time_, currTime_ = 0;
  Bezier curve_;
  std::vector<float> from_, to_, prevValue_;
  std::string name_;
  
  void setup(float *value, float *from, float *to, int length);
  void interpolate(float *value, float *from, float *to, int length, float percent);
  std::function<void(float* value, float* from, float* to, int lenght, float percent)> customInterpolate;
};

#endif /* animator_hpp */
