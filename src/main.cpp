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
using namespace ui;

void addDetection(ui::Canvas &canvas) {
  Rect2f detRect(100, 300, 300, 90);
  Color detColor(0.03, 0.78, 0.94);
  Color candColor(0.83, 0.03, 0.73);
  shared_ptr<Rectangle> uiRect = make_shared<Rectangle>();
  uiRect->rect = detRect;
  uiRect->borderColor = candColor;
  uiRect->borderWidth = 3;
  uiRect->borderOutside = 0;
  uiRect->emplaceAnimator(&uiRect->cornerStrokePercent, 0.3f, 30)
  .setFrom(0.f)
  .setName("stroke_start")
  .setCurve(Bezier::easeInOut());
  
  uiRect->emplaceAnimator(&uiRect->cornerStrokePercent, 1.0f, 30).setDelay(150).setName("stroke_end").setCurve(Bezier::easeInOut());
  uiRect->emplaceAnimator(&uiRect->borderColor, detColor, 30).setDelay(150).setName("color");
  
  shared_ptr<Text> uiText = make_shared<Text>();
  uiText->text = "Abandoned Object";
  uiText->backgroundColor = candColor;
  uiText->textColor = Color::white();
  uiText->pos = uiRect->rect.tl();
  uiText->textFill = 0;
  uiText->emplaceAnimator(&uiText->backgroundColor, detColor, 30).setName("color").setDelay(150);
  uiText->emplaceAnimator(&uiText->textFill, 1.0f, 30).setName("text fill").setDelay(150);
  
  canvas.objects["rect_2"] = uiRect;
  canvas.objects["text_2"] = uiText;
}

int main(int argc, const char * argv[]) {
  // insert code here...
  std::cout << "Hello, World!\n";
  
  VideoCapture cap("/Users/alex/Desktop/VID_20170101_014346_0050.MP4");
  Mat frame;
  shared_ptr<Rectangle> uiRect = make_shared<Rectangle>();
  uiRect->borderColor = Color::red();
  uiRect->borderWidth = 3;
  uiRect->rect = Rect(100, 100, 200, 150);
  uiRect->cornerStrokePercent = 0;
  
  float duration = 60;
  uiRect->emplaceAnimator(&uiRect->rect, Rect2f(500, 200, 400, 200), duration).setCurve(Bezier::easeInOut()).setName("Rect Anim");
  // почемуто второй способ не работает
  
  uiRect->emplaceAnimator(&uiRect->cornerStrokePercent, 1.0f, duration).setCurve(Bezier::easeInOut());//.setName("Stroke Anim");
  
  
  shared_ptr<Text> uiText = make_shared<Text>();
  uiText->text = "abandoned_object";
  uiText->backgroundColor = Color::red().alpha(0.5f);
  uiText->textColor = Color::white();
  uiText->pos = uiRect->rect.tl();
  uiText->emplaceAnimator(&uiText->pos, Point2f(500, 200), duration).setCurve(Bezier::easeOut()).setName("position");
  uiText->emplaceAnimator(&uiText->backgroundColor, uiText->backgroundColor.alpha(1.f), duration).setName("alpha");
  uiText->emplaceAnimator(&uiText->textFill, 1.0f, duration).setFrom(0).setName("text fill");
  
  Canvas canvas;
  canvas.objects["rect"] = uiRect;
  canvas.objects["text"] = uiText;
  
  bool pause = false;
  int frameIdx = 0;
  string savePath = "/Users/alex/Downloads/anim_test.mp4";
  VideoWriter writer;
  while (cap.read(frame)) {
    if (savePath.length() && !writer.isOpened()) {
      int fourcc = cv::VideoWriter::fourcc('a', 'v', 'c', '1');
      writer.open(savePath, fourcc, 30, frame.size());
    }
    frameIdx++;
    if (frameIdx == 120) {
      addDetection(canvas);
    } else if (frameIdx == 500) {
      break;
    }
    canvas.drawOn(frame);
    if (writer.isOpened()) {
      writer.write(frame);
    }
    imshow("img", frame);
    auto k = waitKey(pause ? 0 : 10);
    if ('q' == k) {
      break;
    } else if (' ' == k) {
      pause = !pause;
    }
  }
  
  writer.release();
  return 0;
}
