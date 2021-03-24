//
//  color.cpp
//  OpenCV_Anim
//
//  Created by Alexander Graschenkov on 21.03.2021.
//

#include "color.hpp"

using namespace cv;

namespace ui {

cv::Scalar Color::scalar(float scale) const {
  return cv::Scalar(b*scale, g*scale, r*scale, a*scale);
}

// https://stackoverflow.com/a/6930407/820795
Color rgb2hsv(Color in)
{
  Color out;
  out.a = in.a;
  double min, max, delta;
  
  min = in.r < in.g ? in.r : in.g;
  min = min  < in.b ? min  : in.b;
  
  max = in.r > in.g ? in.r : in.g;
  max = max  > in.b ? max  : in.b;
  
  out.b = max;                                // v
  delta = max - min;
  if (delta < 0.00001)
  {
    out.g = 0;
    out.r = 0; // undefined, maybe nan?
    return out;
  }
  if( max > 0.0 ) { // NOTE: if Max is == 0, this divide would cause a crash
    out.g = (delta / max);                  // s
  } else {
    // if max is 0, then r = g = b = 0
    // s = 0, h is undefined
    out.g = 0.0;
    out.r = NAN;                            // its now undefined
    return out;
  }
  if( in.r >= max )                           // > is bogus, just keeps compilor happy
    out.r = ( in.g - in.b ) / delta;        // between yellow & magenta
  else
    if( in.g >= max )
      out.r = 2.0 + ( in.b - in.r ) / delta;  // between cyan & yellow
    else
      out.r = 4.0 + ( in.r - in.g ) / delta;  // between magenta & cyan
  
  out.r *= 60.0;                              // degrees
  
  if( out.r < 0.0 )
    out.r += 360.0;
  
  return out;
}


Color hsv2rgb(Color in)
{
  double hh, p, q, t, ff;
  long i;
  Color out;
  out.a = in.a;
  
  if(in.g <= 0.0) {       // < is bogus, just shuts up warnings
    out.r = in.b;
    out.g = in.b;
    out.b = in.b;
    return out;
  }
  hh = in.r;
  if(hh >= 360.0) hh = 0.0;
  hh /= 60.0;
  i = (long)hh;
  ff = hh - i;
  p = in.b * (1.0 - in.g);
  q = in.b * (1.0 - (in.g * ff));
  t = in.b * (1.0 - (in.g * (1.0 - ff)));
  
  switch(i) {
    case 0:
      out.r = in.b;
      out.g = t;
      out.b = p;
      break;
    case 1:
      out.r = q;
      out.g = in.b;
      out.b = p;
      break;
    case 2:
      out.r = p;
      out.g = in.b;
      out.b = t;
      break;
      
    case 3:
      out.r = p;
      out.g = q;
      out.b = in.b;
      break;
    case 4:
      out.r = t;
      out.g = p;
      out.b = in.b;
      break;
    case 5:
    default:
      out.r = in.b;
      out.g = p;
      out.b = q;
      break;
  }
  return out;
}

Color Color::interpolate(Color to, float percent, bool hsvSpace) const {
  Color fromHsv = hsvSpace ? rgb2hsv(*this) : *this;
  Color toHsv = hsvSpace ? rgb2hsv(to) : to;
  toHsv.r = (toHsv.r - fromHsv.r) * percent + fromHsv.r;
  toHsv.g = (toHsv.g - fromHsv.g) * percent + fromHsv.g;
  toHsv.b = (toHsv.b - fromHsv.b) * percent + fromHsv.b;
  
  to = hsvSpace ? hsv2rgb(toHsv) : toHsv;
  
  to.a = (to.a - this->a) * percent + this->a;
  return to;
}

Color Color::alpha(float alpha) {
  return Color(r,g,b,alpha);
}

} // namespace ui
