#ifndef ADAS_DETECTOR_H
#define ADAS_DETECTOR_H

#include <queue>
#include <memory>

#include "types.h"
#include "FrameQueue.h"

class Detector {
 private:
  frame_queue_p capture_queue;
  frame_queue_p transport_queue;

  cv::Mat Threshold(const cv::Mat *target, int idx);
  void FuzzyBinarization(const cv::Mat *channels, const cv::Mat *gray, cv::Mat *result);

 public:
  Detector(const frame_queue_p &capture_queue, const frame_queue_p &transport_queue);
  /*
 * VideoIO.capture_queue 에서 프레임을 하나씩 읽어와서 영상 처리를 진행하는 메소드
 */
  void Process();
};

#endif //ADAS_DETECTOR_H
