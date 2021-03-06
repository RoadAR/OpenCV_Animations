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

void Base::tickAndDraw(cv::Mat &img, float dt) {
  if (autoRemoveTime_ > 0) {
    autoRemoveTime_ = MAX(0, autoRemoveTime_ - dt);
    if (autoRemoveTime_ == 0) {
      needToBeRemoved_ = true;
    }
  }
  if (needToBeRemoved_) {
    return;
  }
  
  
  // tick
  for (int i = ((int)animators.size())-1; i >= 0; i--) {
    animators[i].tick(dt);
    if (animators[i].isFinished()) {
      animators.erase(animators.begin() + i);
    }
  }
  
  drawOn(img);
}

void Base::setAutoRemoveAfter(float time) {
  autoRemoveTime_ = time;
}

float Base::getAutoRemoveTime() const {
  return autoRemoveTime_;
}

void Base::setAutoRemove() {
  needToBeRemoved_ = true;
}

bool Base::getNeedToBeRemoved() const {
  return needToBeRemoved_;
}

void Base::resetAutoRemove() {
  autoRemoveTime_ = -1;
  needToBeRemoved_ = true;
}

// MARK: - Rectangle
void Rectangle::drawOn(cv::Mat &img) const {
  if (fillColor.a > 0) {
    Rect r = (Rect)rect & Rect({}, img.size());
    float alpha = MIN(1, fillColor.a);
    addWeighted(img(r), 1-alpha, fillColor.scalar(), alpha, 0, img(r));
  }
  
  if (borderWidth > 0 && borderColor.a > 0 && cornerStrokePercent > 0) {
    strokeOn(img);
  }
}

void Rectangle::strokeOn(cv::Mat &img) const {
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
  Rect snapshotRect(floor(r.x - borderWidth/2)-2,
                    floor(r.y - borderWidth/2)-2,
                    ceil(r.width + borderWidth)+4,
                    ceil(r.height + borderWidth)+4);
  snapshotRect &= Rect({}, img.size());
  Mat snapshot = borderColor.a < 1 ? img(snapshotRect).clone() : Mat();
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

void Text::drawOn(cv::Mat &img) const {
  if (text.empty()) {
    return;
  }
  
  cv::Size textSize = cv::getTextSize(text, FONT_HERSHEY_DUPLEX, scale, thikness, 0);
  
  int expand = scale * 4;
  Rect r(0, 0, textSize.width+expand*2, textSize.height+expand*2);
  r.width += margin.left + margin.right;
  r.height += margin.top + margin.bot;
  r.x = pos.x - anchor.x * r.width;
  r.y = pos.y - anchor.y * r.height;
  r.width *= textFill;
  
  Point2f drawPoint = Point2f(r.x + expand + margin.left, r.br().y - expand - margin.bot);
  Rect rCliped = r & Rect({}, img.size());
  
  float alpha = backgroundColor.a;
  if (alpha > 0) {
    addWeighted(img(rCliped), 1-alpha, backgroundColor.scalar(), alpha, 0, img(rCliped));
  }
  
  Mat snapshot = textColor.a < 1 ? img(rCliped).clone() : Mat();
  string drawText = text;
  if (textFill < 1) {
    while (drawText.length()) {
      cv::Size textSize = cv::getTextSize(drawText, FONT_HERSHEY_DUPLEX, scale, thikness, 0);
      if (textSize.width + expand*2 <= r.width) {
        // text fit to rect
        break;
      } else {
        // text doesn't fit inside rect, so make length smaller
        drawText.pop_back();
      }
    }
  }
  putText(img, drawText, drawPoint, FONT_HERSHEY_DUPLEX, scale, textColor.scalar(), thikness, antialiasing ? LINE_AA : LINE_8);
  
  // border with alpha
  if (!snapshot.empty()) {
    float alpha = textColor.a;
    addWeighted(img(rCliped), alpha, snapshot, 1-alpha, 0, img(rCliped));
  }
}

} // namespace ui
