#include <cmath>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include "Detector.h"

Detector::Detector(const frame_queue_p &capture_queue, const frame_queue_p &transport_queue) {
  this->capture_queue = capture_queue;
  this->transport_queue = transport_queue;
}

/*
* capture_queue 에서 프레임을 하나씩 읽어와서 영상 처리를 진행
* 영상 처리 완료 후 transport_queue 에 저장
* 별도의 쓰레드에서 실행된다.
*/
void Detector::Process() {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
#ifdef DEBUG
  // 디버깅을 위한 영상 출력
  // Release 모드에서는 실행되지 않는다.
  cv::startWindowThread();
#endif
  while (true) {
    // capture_queue 에서 프레임 읽어오기
    auto origin = this->capture_queue->DequeueFrame();

    // 영상처리
    cv::Mat gray_frame;

    // 1. Color Space 변환 : RGB -> Gray
    cv::cvtColor(origin, gray_frame, cv::COLOR_BGR2GRAY);

    // 2. Gray Scale 이미지 이진화
    auto adaptive_gaussian_binary = this->Threshold(&gray_frame, 0);
    auto otsu_binary = this->Threshold(&gray_frame, 1);
    cv::Mat channels[3];
    cv::split(origin, channels);
    auto fuzzy_binary = gray_frame.clone();
    this->FuzzyBinarization(channels, &gray_frame, &fuzzy_binary);

#ifdef DEBUG
    // 디버깅을 위한 영상 출력
    // Release 모드에서는 실행되지 않는다.
    cv::Mat debug_image;
    cv::Mat arr[] = {adaptive_gaussian_binary, otsu_binary, fuzzy_binary};
    cv::hconcat(arr, 3, debug_image);
    cv::imshow("1. Adaptive Gaussian, 2. OTSU, 3. Fuzzy", debug_image);
    if (cv::waitKey(30) >= 0) break;
#endif


    // 영상처리 후 transport_queue 에 프레임 저장
    this->transport_queue->EnqueueFrame(origin);
  }
#pragma clang diagnostic pop
}

cv::Mat Detector::Threshold(const cv::Mat *target, int idx) {
  cv::Mat result;

  switch (idx) {
    case 0:
      cv::adaptiveThreshold(*target,
                            result,
                            255,
                            cv::ADAPTIVE_THRESH_GAUSSIAN_C,
                            cv::THRESH_BINARY,
                            11,
                            10);
      break;
    case 1:cv::threshold(*target, result, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
      break;
    default:break;
  }

  return result;
}
void Detector::FuzzyBinarization(const cv::Mat *channels, const cv::Mat *gray, cv::Mat *result) {
  // 원본(RGB 이미지)에서 평균 밝기 값 구하기
  // 평균값 계산
  cv::Mat1d tmp_mean = (channels[0] + channels[1] + channels[2]) / 3.0;
  double xm = cv::mean(tmp_mean).val[0];

  // Gray Scale 이미지에서 최대, 최소 픽셀 값 구하기
  double xh = 0;    // 가장 밝은 픽셀 값
  double xl = 255;  // 가장 어두운 픽셀 값
  cv::minMaxLoc(*gray, &xl, &xh);

  // 평균 값(xm)에서 xl, xh 까지의 거리 구하기
  double dmax = std::abs(xh - xm);
  double dmin = std::abs(xm - xl);

  // 밝기 조정률 구하기
  double gamma = 0;
  if (xm > 128)
    xm = 255 - xm;

  if (dmin > xm)
    gamma = xm;
  else
    gamma = dmin;

  if (dmax > xm)
    gamma = xm;
  else
    gamma = dmax;

  // 밝기 조정률을 이용하여 최대 밝기 값(imax), 최소 밝기 값(imin)과 중간 밝기 값(imid) 구하기
  auto imax = xm + gamma;
  auto imin = xm - gamma;
  auto imid = (imax + imin) / 2;

  // 그레이 스케일 이미지에 멤버쉽 함수 적용
  for (int i = 0; i < result->rows; ++i) {
    for (int j = 0; j < result->cols; ++j) {
      double p = result->at<uchar>(i, j);
//      std::cout << p << std::endl;
      if (p <= imin || p >= imax)
        p = 0;
      else if (p < imid)
        p = (p - imin) / (imid - imin);
      else if (p == imid)
        p = 1;
      else if (p > imid)
        p = (imax - p) / (imax - imid);

      result->at<uchar>(i, j) = static_cast<uchar>(p > 0.34 ? 255 : 0);
    }
  }
}