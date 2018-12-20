#include <queue>
#include "FrameQueue.h"
#include "VideoIO.h"

// Todo: 종료 기능 구현후에 아래 pragma구문 삭제하기
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
VideoIO::VideoIO(
    const std::shared_ptr<FrameQueue> &capture_queue,
    const std::shared_ptr<FrameQueue> &transport_queue) {

  this->capture_queue = capture_queue;
  this->transport_queue = transport_queue;

  this->capture = std::make_unique<cv::VideoCapture>(this->KInUrl, cv::CAP_GSTREAMER);
  this->writer =
      std::make_unique<cv::VideoWriter>(
          this->KOutUrl,
          cv::CAP_GSTREAMER,
          0,
          30,
          cv::Size(640, 480),
          true
      );

  // cv::VideoCapture 또는 cv::VideoWriter 를 사용할 수 없으면 프로그램 강제 종료
  if (!this->capture->isOpened() || !this->writer->isOpened()) {
    std::cerr << "VideoCapture or VideoWriter not opened" << std::endl;
    exit(-1);
  }
}

/*
 * - 웹캠에서 프레임을 읽어오는 메소드
 * - 쓰레드에서 실행되며, 읽어온 프레임은 capture_queue 에 저장된다.
 *  Todo: 특정 조건에서 쓰레디 실행을 종료시키는 기능 추가하기
 */
void VideoIO::CaptureFrame() {
  cv::Mat frame;

  while (true) {
#ifdef DEBUG
    auto tmp = cv::imread("../../src/image.jpg", cv::IMREAD_COLOR);
    cv::resize(tmp, frame, cv::Size(640, 480), 0, 0);
#else
    *(this->capture) >> frame;
#endif

    this->capture_queue->EnqueueFrame(frame);
  } 
}

/*
 * transport_queue 에 저장된 프레임을 원격 클라이언트에 전송
 * 별도의 쓰레드에서 시행된다.
 */
void VideoIO::TransportFrame() {
  while (true) {
    auto frame = this->transport_queue->DequeueFrame();

    *(this->writer) << frame;
  }
}

#pragma clang diagnostic pop