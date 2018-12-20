#include <iostream>
#include <memory>
#include <thread>

#include "types.h"
#include "VideoIO.h"
#include "Detector.h"

int main() {
  // VideoIO, Detector 에서 사용할 FrameQueue 생성
  frame_queue_p capture_queue = std::make_shared<FrameQueue>();
  frame_queue_p transport_queue = std::make_shared<FrameQueue>();

  VideoIO videoIO(capture_queue, transport_queue);
  Detector detector(capture_queue, transport_queue);

  std::thread capture_thread([&] {
    videoIO.CaptureFrame();
  });
  std::thread img_proc_thread([&] {
    detector.Process();
  });
  std::thread transport_thread([&] {
    videoIO.TransportFrame();
  });

  img_proc_thread.join();
  capture_thread.join();
  transport_thread.join();

  return 0;
}