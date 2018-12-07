#include <opencv2/core/mat.hpp>
#include "FrameQueue.h"

void FrameQueue::EnqueueFrame(const cv::Mat frame) {
  std::lock_guard<std::mutex> lock(this->mutex);

  this->queue.push(frame);

  this->condition.notify_one();
}
cv::Mat FrameQueue::DequeueFrame() {
  std::unique_lock<std::mutex> lock(this->mutex);
  // 큐에 원소가 적어도 한 개 이상일 때까지 기다린다.
  this->condition.wait(lock, [&] { return !queue.empty(); });

  auto frame = this->queue.front();
  this->queue.pop();

  return frame;
}
