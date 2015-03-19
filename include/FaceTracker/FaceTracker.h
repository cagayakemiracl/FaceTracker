// Copyright (c) 2015 cagayakemiracl All Rights Reserved.
// $Mail: <cagayakemiracl@gmail.com>

#ifndef FACETRACKER_H_
#define FACETRACKER_H_

#include <opencv/highgui.h>
#include <string>
#include "./Tracker.h"

namespace FACETRACKER {
class FaceTracker {
 public:
  static void ApplyFaceRecognition(cv::Mat &im, Tracker &model,
                                   cv::Mat &con, cv::Mat &tri);

 private:
  static void Draw(cv::Mat &image, cv::Mat &shape, cv::Mat &con, cv::Mat &tri,
                   cv::Mat &visi);
};
}
#endif  // FACETRACKER_H_
