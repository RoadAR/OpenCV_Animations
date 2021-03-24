//
//  canvas.cpp
//  OpenCV_Anim
//
//  Created by Alexander Graschenkov on 21.03.2021.
//

#include "canvas.hpp"

namespace ui {

void Canvas::tickAndDraw(cv::Mat &img, float dt) {
  for (auto it = objects.cbegin(); it != objects.cend(); it++) {
    it->second->tickAndDraw(img, dt);
    if (it->second->getNeedToBeRemoved()) {
      objects.erase(it);
    }
  }
}

} // namespace ui
