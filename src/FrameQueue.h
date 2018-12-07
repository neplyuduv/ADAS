#ifndef ADAS_FRAMEQUEUE_H
#define ADAS_FRAMEQUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include <opencv2/core/mat.hpp>

class FrameQueue {
 private:
  std::queue<cv::Mat> queue;
  mutable std::mutex mutex;
  std::condition_variable condition;

 public:
  void EnqueueFrame(cv::Mat frame);
  cv::Mat DequeueFrame();
};

#endif //ADAS_FRAMEQUEUE_H
