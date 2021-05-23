#include <iostream>
#include <thread>
#include <queue>
#include <atomic>
#include <functional>
#include <list>
#include <condition_variable>
#include <unistd.h>

using namespace std;

int64_t fib(int64_t x) {
  if (x <= 2) {
    return 2;
  }

  return fib(x - 1) + fib(x - 2);
}

void foo() {
  cout << "working" << endl;
  cout << fib(45) << endl;
}

class ThreadPoolExecuter {
  public:
  ThreadPoolExecuter(size_t pool_size = 10, size_t _max_queue_size = 1000): _pool_size(pool_size), _max_queue_size(_max_queue_size) {
    for (int i = 0; i < pool_size; i++) {
      thread t1(&ThreadPoolExecuter::run, this);
      _threads.emplace_back(std::move(t1));
    }
  }

  bool submit(function<void()> func) {
    _mutex.lock();
    if (_queue.size() >= _max_queue_size) {
      cout << _queue.size() << endl;
      _mutex.unlock();
      return false;
    }
    _queue.push_back(func);
    _condition.notify_one();
    _mutex.unlock();
    return true;
  }

  void shutdown() {
    _shutdown = true;
    for (auto &t : _threads) {
      t.join();
    }
  }

  private:
  void run() {
    while (true) {
      _mutex.lock();
      std::function<void(void)> x;
      if (_queue.empty() == false) {
        x = _queue.front();
        _queue.pop_front();
        _mutex.unlock();
        x();
      } else {
        _mutex.unlock();
        if (_shutdown == true) {
          break;
        }
        unique_lock<mutex> lk(_condition_mutex);
        _condition.wait(lk);
      }
    }
  }

  list< function<void ()> > _queue;
  mutex _mutex;
  mutex _condition_mutex;
  condition_variable _condition;
  list <thread> _threads;
  size_t _pool_size = 0;
  size_t _max_queue_size = 0;
  bool _shutdown = false;
};


int main() {
  ThreadPoolExecuter workers(24);
  for (int i = 0; i < 100; i++) {
    if (workers.submit(bind(foo)) == false) {
      sleep(1);
    }
  }
  workers.shutdown();
  return 0;
}