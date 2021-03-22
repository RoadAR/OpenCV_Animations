//
//  canvas.hpp
//  OpenCV_Anim
//
//  Created by Alexander Graschenkov on 21.03.2021.
//

#ifndef canvas_hpp
#define canvas_hpp

#include <stdio.h>
#include <opencv2/core.hpp>
#include <map>
#include "obj_drawable.hpp"

class UICanvas {
public:
  UICanvas() = default;
  
  std::map<std::string, std::shared_ptr<UIBase>> objects;
  
  void drawOn(cv::Mat &img, float dt = 1.0);
};

#endif /* canvas_hpp */
