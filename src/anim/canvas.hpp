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

namespace ui {

class Canvas {
public:
  Canvas() = default;
  
  std::map<std::string, std::shared_ptr<Base>> objects;
  
  void tickAndDraw(cv::Mat &img, float dt = 1.0);
};

} // namespace ui

#endif /* canvas_hpp */
