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
  while (true) {
    // capture_queue 에서 프레임 읽어오기
    auto origin = this->capture_queue->DequeueFrame();

    // 영상처리



    // 영상처리 후 transport_queue 에 프레임 저장
    this->transport_queue->EnqueueFrame(origin);
  }
#pragma clang diagnostic pop
}
