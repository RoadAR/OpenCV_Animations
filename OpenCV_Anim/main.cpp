//
//  main.cpp
//  OpenCV_Anim
//
//  Created by Alexander Graschenkov on 15.03.2021.
//

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "anim/obj_drawable.hpp"
#include "anim/canvas.hpp"

using namespace std;
using namespace cv;

int main(int argc, const char * argv[]) {
  // insert code here...
  std::cout << "Hello, World!\n";
  
  VideoCapture cap("/Users/alex/Desktop/VID_20170101_014346_0050.MP4");
  Mat frame;
  shared_ptr<UIRectangle> uiRect = make_shared<UIRectangle>();
  uiRect->borderColor = Color::red();
  uiRect->borderWidth = 3;
  uiRect->rect = Rect(100, 100, 200, 150);
  uiRect->cornerStrokePercent = 0;
  
  float duration = 60;
  uiRect->animators.emplace_back(&uiRect->rect, Rect2f(500, 200, 400, 200), duration, Bezier::easeInOut());//.setName("Rect Anim");
//  uiRect.emplaceAnimator(&uiRect.rect, Rect2f(500, 200, 400, 200), duration, Bezier::easeInOut()).setName("Rect Anim");
  // почемуто второй способ не работает
  
  uiRect->animators.emplace_back(&uiRect->cornerStrokePercent, 1.0f, 60.f, Bezier::easeInOut());//.setName("Stroke Anim");
  
  
  shared_ptr<UIText> uiText = make_shared<UIText>();
  uiText->text = "abandoned_object";
  uiText->backgroundColor = Color::red().alpha(0.5f);
  uiText->textColor = Color::white();
  uiText->pos = uiRect->rect.tl();
  uiText->animators.emplace_back(&uiText->pos, Point2f(500, 200), duration, Bezier::easeOut());
//  uiText->animators.emplace_back(&uiText->thikness, 5.0f, duration*2);
  uiText->animators.emplace_back(&uiText->backgroundColor, uiText->backgroundColor.alpha(1.f), duration);
  
  UICanvas canvas;
  canvas.objects["rect"] = uiRect;
  canvas.objects["text"] = uiText;
  
  bool pause = false;
  while (cap.read(frame)) {
    canvas.drawOn(frame);
    imshow("img", frame);
    auto k = waitKey(pause ? 0 : 10);
    if ('q' == k) {
      break;
    } else if (' ' == k) {
      pause = !pause;
    }
  }
  
  return 0;
}
