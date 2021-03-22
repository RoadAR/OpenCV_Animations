//
//  obj_drawable.cpp
//  OpenCV_Anim
//
//  Created by Alexander Graschenkov on 21.03.2021.
//

#include "obj_drawable.hpp"
#include <opencv2/imgproc.hpp>

using namespace cv;
using namespace std;

namespace ui {

void Base::drawOn(cv::Mat &img, float dt) {
  for (int i = ((int)animators.size())-1; i >= 0; i--) {
    animators[i].tick(dt);
    if (animators[i].isFinished()) {
      animators.erase(animators.begin() + i);
    }
  }
}

// MARK: - Rectangle
void Rectangle::drawOn(cv::Mat &img, float dt) {
  Base::drawOn(img, dt);
  
  if (fillColor.a > 0) {
    Rect r = (Rect)rect & Rect({}, img.size());
    float alpha = MIN(1, fillColor.a);
    addWeighted(img(r), 1-alpha, fillColor.scalar(), alpha, 0, img(r));
  }
  
  if (borderWidth > 0 && borderColor.a > 0 && cornerStrokePercent > 0) {
    strokeOn(img);
  }
}

void Rectangle::strokeOn(cv::Mat &img) {
  vector<vector<Point2f>> lines;
  
  float inset = borderWidth * (borderOutside - 0.5);
  Rect2f r(rect.x - inset,
           rect.y - inset,
           rect.width + inset*2.f,
           rect.height + inset*2.f);
  Point2f tl = r.tl();
  Point2f br = r.br();
  Point2f tr(br.x, tl.y);
  Point2f bl(tl.x, br.y);
  
  Point2f dx = tr - tl;
  Point2f dy = bl - tl;
  
  // define lines to draw
  if (cornerStrokePercent >= 1) {
    // normal rectangle
    vector<Point2f> line;
    line.push_back(tl); line.push_back(tr);
    line.push_back(br); line.push_back(bl);
    line.push_back(tl);
    lines.push_back(line);
  } else {
    float p = cornerStrokePercent * 0.5f;
    vector<Point2f> line;
    line.push_back(tl+dy*p); line.push_back(tl); line.push_back(tl+dx*p);
    lines.push_back(line);
    
    line.clear();
    line.push_back(tr+dy*p); line.push_back(tr); line.push_back(tr-dx*p);
    lines.push_back(line);
    
    line.clear();
    line.push_back(br-dy*p); line.push_back(br); line.push_back(br-dx*p);
    lines.push_back(line);
    
    line.clear();
    line.push_back(bl-dy*p); line.push_back(bl); line.push_back(bl+dx*p);
    lines.push_back(line);
  }
  
  // draw lines
  Scalar color = borderColor.scalar();
  Rect snapshotRect(floor(r.x - borderWidth/2),
                    floor(r.y - borderWidth/2),
                    ceil(r.width + borderWidth),
                    ceil(r.height + borderWidth));
  snapshotRect &= Rect({}, img.size());
  Mat snapshot = borderColor.a < 1 ? img(snapshotRect) : Mat();
  for (const auto &l : lines) {
    for (int i = 1; i < l.size(); i++) {
      cv::line(img, l[i-1], l[i], color, borderWidth, antialiasing ? LINE_AA : LINE_8);
    }
  }
  
  // border with alpha
  if (!snapshot.empty()) {
    float alpha = borderColor.a;
    addWeighted(img(snapshotRect), alpha, snapshot, 1-alpha, 0, img(snapshotRect));
  }
}

// MARK: - UIText

void Text::drawOn(cv::Mat &img, float dt) {
  Base::drawOn(img, dt);
  if (text.empty()) {
    return;
  }
  
  cv::Size textSize = cv::getTextSize(text, FONT_HERSHEY_DUPLEX, scale, thikness, 0);
  
  int expand = scale * 4;
  Rect r(0, 0, textSize.width+expand*2, textSize.height+expand*2);
  r.x = pos.x - anchor.x * r.width;
  r.y = pos.y - anchor.y * r.height;
  
  Point2f drawPoint = Point2f(r.x + expand, r.br().y - expand);
  Rect rCliped = r & Rect({}, img.size());
  
  float alpha = backgroundColor.a;
  if (alpha > 0) {
    addWeighted(img(rCliped), 1-alpha, backgroundColor.scalar(), alpha, 0, img(rCliped));
  }
  
  Mat snapshot = textColor.a < 1 ? img(rCliped) : Mat();
  putText(img, text, drawPoint, FONT_HERSHEY_DUPLEX, scale, textColor.scalar(), thikness, antialiasing ? LINE_AA : LINE_8);
}

} // namespace ui
