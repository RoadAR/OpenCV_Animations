//
//  obj_drawable.hpp
//  OpenCV_Anim
//
//  Created by Alexander Graschenkov on 21.03.2021.
//

#ifndef obj_drawable_hpp
#define obj_drawable_hpp

#include <stdio.h>
#include <opencv2/core.hpp>
#include <unordered_map>
#include "animator.hpp"

namespace ui {

class Base {
public:
  void tickAndDraw(cv::Mat &img, float dt = 1.0);
  virtual void drawOn(cv::Mat &img) const = 0;
  
  template <class... _Args>
  Animator &emplaceAnimator(_Args&&... args) {
    animators.emplace_back(std::forward<_Args>(args)...);
    return animators.back();
  }
  
  std::vector<Animator> animators;
  
  /// helpful when you finish animation and autoremove from hierarchy automaticly
  void setAutoRemoveAfter(float time);
  float getAutoRemoveTime() const;
  void setAutoRemove();
  bool getNeedToBeRemoved() const;
  void resetAutoRemove();
  
private:
  float autoRemoveTime_ = -1;
  bool needToBeRemoved_ = false;
};


class Rectangle: public Base {
public:
  float borderWidth = 0;
  Color borderColor = {}; // alpha support
  Color fillColor = {}; // alpha support
  cv::Rect2f rect;
  float cornerStrokePercent = 1;
  /// 0 - inside, 0.5 - middle, 1 - outside
  float borderOutside = 0;
  bool antialiasing = true;
  
  void drawOn(cv::Mat &img) const override;
  
private:
  void strokeOn(cv::Mat &img) const;
};


struct Inset {
  Inset(){};
  Inset(float left, float top, float right, float bot):
    left(left), top(top), right(right), bot(bot) {}
  Inset(float x, float y):
    left(x), top(y), right(x), bot(y) {}
  
  float left = 0, top = 0, right = 0, bot = 0;
};

class Text: public Base {
public:
  std::string text;
  cv::Point2f pos;
  float thikness = 1.0f;
  float scale = 1.0f;
  float textFill = 1.0f;
  cv::Point2f anchor = cv::Point2f(0, 1); // bottom left
  Color backgroundColor = {}; // alpha support
  Color textColor = {}; // alpha support
  Inset margin = {};
  bool antialiasing = true;
  
  void drawOn(cv::Mat &img) const override;
};

}
#endif /* obj_drawable_hpp */
