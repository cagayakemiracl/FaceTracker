///////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2010, Jason Mora Saragih, all rights reserved.
//
// This file is part of FaceTracker.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//     * The software is provided under the terms of this licence stricly for
//       academic, non-commercial, not-for-profit purposes.
//     * Redistributions of source code must retain the above copyright notice,
//       this list of conditions (licence) and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions (licence) and the following disclaimer
//       in the documentation and/or other materials provided with the
//       distribution.
//     * The name of the author may not be used to endorse or promote products
//       derived from this software without specific prior written permission.
//     * As this software depends on other libraries, the user must adhere to
//       and keep in place any licencing terms of those libraries.
//     * Any publications arising from the use of this software, including but
//       not limited to academic journal and conference publications, technical
//       reports and manuals, must cite the following work:
//
//       J. M. Saragih, S. Lucey, and J. F. Cohn. Face Alignment through
//       Subspace Constrained Mean-Shifts. International Conference of Computer
//       Vision (ICCV), September, 2009.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR "AS IS" AND ANY EXPRESS OR IMPLIED
// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
// EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
// THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
///////////////////////////////////////////////////////////////////////////////
#include <FaceTracker/FaceTracker.h>
#include <opencv/highgui.h>
#include <iostream>
using namespace FACETRACKER;
//=============================================================================
void FaceTracker::Draw(cv::Mat &image, cv::Mat &shape, cv::Mat &con,
                       cv::Mat &tri, cv::Mat &visi) {
  int i, n = shape.rows / 2;
  cv::Point p1, p2;
  cv::Scalar c;

  // draw triangulation
  c = CV_RGB(0, 0, 0);
  for (i = 0; i < tri.rows; i++) {
    if (visi.at<int>(tri.at<int>(i, 0), 0) == 0 ||
        visi.at<int>(tri.at<int>(i, 1), 0) == 0 ||
        visi.at<int>(tri.at<int>(i, 2), 0) == 0)
      continue;
    p1 = cv::Point(shape.at<double>(tri.at<int>(i, 0), 0),
                   shape.at<double>(tri.at<int>(i, 0) + n, 0));
    p2 = cv::Point(shape.at<double>(tri.at<int>(i, 1), 0),
                   shape.at<double>(tri.at<int>(i, 1) + n, 0));
    cv::line(image, p1, p2, c);
    p1 = cv::Point(shape.at<double>(tri.at<int>(i, 0), 0),
                   shape.at<double>(tri.at<int>(i, 0) + n, 0));
    p2 = cv::Point(shape.at<double>(tri.at<int>(i, 2), 0),
                   shape.at<double>(tri.at<int>(i, 2) + n, 0));
    cv::line(image, p1, p2, c);
    p1 = cv::Point(shape.at<double>(tri.at<int>(i, 2), 0),
                   shape.at<double>(tri.at<int>(i, 2) + n, 0));
    p2 = cv::Point(shape.at<double>(tri.at<int>(i, 1), 0),
                   shape.at<double>(tri.at<int>(i, 1) + n, 0));
    cv::line(image, p1, p2, c);
  }
  // draw connections
  c = CV_RGB(0, 0, 255);
  for (i = 0; i < con.cols; i++) {
    if (visi.at<int>(con.at<int>(0, i), 0) == 0 ||
        visi.at<int>(con.at<int>(1, i), 0) == 0)
      continue;
    p1 = cv::Point(shape.at<double>(con.at<int>(0, i), 0),
                   shape.at<double>(con.at<int>(0, i) + n, 0));
    p2 = cv::Point(shape.at<double>(con.at<int>(1, i), 0),
                   shape.at<double>(con.at<int>(1, i) + n, 0));
    cv::line(image, p1, p2, c, 1);
  }
  // draw points
  for (i = 0; i < n; i++) {
    if (visi.at<int>(i, 0) == 0) continue;
    p1 = cv::Point(shape.at<double>(i, 0), shape.at<double>(i + n, 0));
    c = CV_RGB(255, 0, 0);
    /*
    std::string str = std::to_string(i);
    cv::putText(image, str, p1, CV_FONT_HERSHEY_SIMPLEX, 0.5,
                c);
    */
    cv::circle(image, p1, 2, c);
  }
  return;
}

void FaceTracker::ApplyFaceRecognition(cv::Mat &im, Tracker &model,
                                       cv::Mat &con, cv::Mat &tri) {
  bool fcheck = false;
  int fpd = -1;

  // set other tracking parameters
  std::vector<int> wSize1(1);
  wSize1[0] = 7;
  std::vector<int> wSize2(3);
  wSize2[0] = 11;
  wSize2[1] = 9;
  wSize2[2] = 7;
  int nIter = 5;
  double clamp = 3, fTol = 0.01;

  static double fps = 0;
  char sss[256];
  std::string text;

  cv::Mat gray;
  static int64 t1, t0 = cvGetTickCount();
  static int fnum = 0;

  static bool failed = true;

  cv::flip(im, im, 1);
  cv::cvtColor(im, gray, CV_BGR2GRAY);
  // track this image
  std::vector<int> wSize;
  if (failed)
    wSize = wSize2;
  else
    wSize = wSize1;
  if (model.Track(gray, wSize, fpd, nIter, clamp, fTol, fcheck) == 0) {
    int idx = model._clm.GetViewIdx();
    failed = false;
    Draw(im, model._shape, con, tri, model._clm._visi[idx]);
  } else {
    cv::Mat R(im, cvRect(0, 0, 150, 50));
    R = cv::Scalar(0, 0, 255);
    model.FrameReset();
    failed = true;
  }
  // draw framerate on display image
  if (fnum >= 9) {
    t1 = cvGetTickCount();
    fps = 10.0 / ((double(t1 - t0) / cvGetTickFrequency()) / 1e+6);
    t0 = t1;
    fnum = 0;
  } else
    fnum += 1;

  sprintf(sss, "%d frames/sec", (int)round(fps));
  text = sss;
  cv::putText(im, text, cv::Point(10, 20), CV_FONT_HERSHEY_SIMPLEX, 0.5,
              CV_RGB(255, 255, 255));
}
