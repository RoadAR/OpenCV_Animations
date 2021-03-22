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
  template <typename T>
  UIAnimator(T *value, const T &to, float time): time_(time) {
    setup((float*)value, nullptr, (float*)&to, sizeof(T) / sizeof(float));
  }
  
  UIAnimator(Color *value, const Color &to, float time);
  
  void tick(float dt);
  bool isFinished() const { return finished_; }
  
  
  UIAnimator &setName(const std::string &name);
  std::string getName() const { return name_; }
  
  UIAnimator &setCurve(const Bezier &curve);
  
  template <typename T>
  UIAnimator &setFrom(const T &from) {
    float *p = (float*)&from;
    from_ = std::vector<float>(p, p+from_.size());
  }
  
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
