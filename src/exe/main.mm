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
//=============================================================================
int main(void) {
  char ftFile[256] = "../model/face2.tracker";
  char conFile[256] = "../model/face.con";
  char triFile[256] = "../model/face.tri";

  FACETRACKER::Tracker model(ftFile);
  cv::Mat tri = FACETRACKER::IO::LoadTri(triFile);
  cv::Mat con = FACETRACKER::IO::LoadCon(conFile);

  // initialize camera and display window
  cv::Mat im;
  CvCapture *camera = cvCreateCameraCapture(CV_CAP_ANY);
  if (!camera) return -1;
  cvNamedWindow("Face Tracker", 1);
  std::cout << "Hot keys: " << std::endl << "\t ESC - quit" << std::endl
            << "\t d   - Redetect" << std::endl;

  // loop until quit (i.e user presses ESC)
  while (1) {
    // grab image, resize and flip
    IplImage *I = cvQueryFrame(camera);
    if (!I) continue;
    im = I;

    // show image and check for user input
    FACETRACKER::FaceTracker::ApplyFaceRecognition(im, model, con, tri);
    imshow("Face Tracker", im);
    int c = cvWaitKey(10);
    if (c == 27)
      break;
    else if (char(c) == 'd')
      model.FrameReset();
  }
  return 0;
}
//=============================================================================
