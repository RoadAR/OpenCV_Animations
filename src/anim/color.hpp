//
//  color.hpp
//  OpenCV_Anim
//
//  Created by Alexander Graschenkov on 21.03.2021.
//

#ifndef color_hpp
#define color_hpp

#include <stdio.h>
#include <opencv2/core.hpp>

namespace ui {

struct Color {
  float r = 0, g = 0, b = 0, a = 0;
  Color() {};
  Color(float r, float g, float b, float a = 1): r(r), g(g), b(b), a(a) {};
  
  cv::Scalar scalar(float scale = 255.f);
  Color interpolate(Color to, float percent, bool hsvSpace = true);
  Color alpha(float alpha);
  
  static Color white()  { return Color(1,1,1,1); }
  static Color black()  { return Color(0,0,0,1); }
  static Color red()    { return Color(1,0,0,1); }
  static Color green()  { return Color(0,1,0,1); }
  static Color blue()   { return Color(0,0,1,1); }
  static Color yellow() { return Color(1,1,0,1); }
};

} // namespace ui

#endif /* color_hpp */
