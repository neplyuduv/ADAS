#ifndef ADAS_CAMERA_H
#define ADAS_CAMERA_H

#include <string>
#include <queue>
#include <memory>
#include <opencv2/opencv.hpp>

#include "types.h"
#include "FrameQueue.h"

class VideoIO {
 private:
  const std::string KInUrl =
      "v4l2src ! videoconvert ! video/x-raw,format=BGR,width=640,height=480,framerate=30/1 ! appsink";
  const std::string KOutUrl =
      "appsrc ! videoconvert ! video/x-raw,format=YUY2,width=640,height=480,framerate=30/1 ! jpegenc ! rtpjpegpay ! udpsink host=127.0.0.1 port=5000";
  frame_queue_p capture_queue;
  frame_queue_p transport_queue;

  std::unique_ptr<cv::VideoCapture> capture;
  std::unique_ptr<cv::VideoWriter> writer;

 public:
  VideoIO(const std::shared_ptr<FrameQueue> &capture_queue, const std::shared_ptr<FrameQueue> &transport_queue);
  // 웹캠에서 프레임을 획득 후 capture_queue 에 저장하는 메소드
  void CaptureFrame();
  // 영상 처리된 프레임을 원격 클라이언트에 전송하는 메소드
  void TransportFrame();
};

#endif //ADAS_CAMERA_H
