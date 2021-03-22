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

class UIBase {
public:
  virtual void drawOn(cv::Mat &img, float dt = 1.0);
  
  template <class... _Args>
  UIAnimator &emplaceAnimator(_Args&&... args) {
    animators.emplace_back(std::forward<_Args>(args)...);
    return animators.back();
  }
  
  std::vector<UIAnimator> animators;
};


class UIRectangle: public UIBase {
public:
  float borderWidth = 0;
  Color borderColor = {}; // alpha support
  Color fillColor = {}; // alpha support
  cv::Rect2f rect;
  float cornerStrokePercent = 1;
  bool antialiasing = true;
  
  void drawOn(cv::Mat &img, float dt = 1.0) override;
  
private:
  void strokeOn(cv::Mat &img);
};


class UIText: public UIBase {
public:
  std::string text;
  cv::Point2f pos;
  float thikness = 1.0f;
  float scale = 1.0f;
  Color backgroundColor = {}; // alpha support
  Color textColor = {}; // alpha support
  bool antialiasing = true;
  
  void drawOn(cv::Mat &img, float dt = 1.0) override;
};

#endif /* obj_drawable_hpp */
