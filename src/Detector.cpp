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
void Detector::process() {
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
    cv::Mat gray;

    // 1. Color Space 변환 : RGB -> Gray
    cv::cvtColor(origin, gray, cv::COLOR_BGR2GRAY);

#ifdef DEBUG
    // 디버깅을 위한 영상 출력
    // Release 모드에서는 실행되지 않는다.
    cv::imshow("Debug Windows", gray);
    if (cv::waitKey(30) >= 0) break;
#endif


    // 영상처리 후 transport_queue 에 프레임 저장
    this->transport_queue->EnqueueFrame(origin);
  }
#pragma clang diagnostic pop
}
