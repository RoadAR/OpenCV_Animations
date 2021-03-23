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

namespace ui {

class Animator {
public:
  template <typename T>
  Animator(T *value, const T &to, float time): time_(time) {
    setup((float*)value, (float*)&to, sizeof(T) / sizeof(float));
  }
  
  Animator(Color *value, const Color &to, float time);
  
  void tick(float dt);
  bool isFinished() const { return finished_; }
  
  
  Animator &setName(const std::string &name);
  std::string getName() const { return name_; }
  
  Animator &setCurve(const Bezier &curve);
  
  Animator &setDelay(float delay);
  
  template <typename T>
  Animator &setFrom(const T &from) {
    float *p = (float*)&from;
    from_ = std::vector<float>(p, p+to_.size());
    return *this;
  }
  
private:
  bool finished_ = false;
  float *value_, time_, currTime_ = 0, delay_ = 0.f;
  Bezier curve_;
  std::vector<float> from_, to_, prevValue_;
  std::string name_;
  
  void setup(float *value, float *to, int length);
  void interpolate(float *value, float *from, float *to, int length, float percent);
  std::function<void(float* value, float* from, float* to, int lenght, float percent)> customInterpolate;
};

} // namespace ui
#endif /* animator_hpp */
