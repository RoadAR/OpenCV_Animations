//
//  canvas.cpp
//  OpenCV_Anim
//
//  Created by Alexander Graschenkov on 21.03.2021.
//

#include "canvas.hpp"

namespace ui {

void Canvas::drawOn(cv::Mat &img, float dt) {
  for (auto &k : objects) {
    k.second->drawOn(img, dt);
  }
}

} // namespace ui
