#pragma once

namespace contacts {
namespace v1 {

using namespace std;

/**
 * homemade simplest implementation of a stream of events
 * inspired by https://www.dartlang.org/tutorials/language/streams
 *
 * WARNING: this implementation might contain unforeseen bugs.
 */
template <class T> class stream {
public:
  ~stream() {
    if (nullptr != current_) {
      delete current_;
      current_ = nullptr;
    }
  }

  /**
   * value is wrapped up in an instance of the entry class
   * which chains the next future.
   */
  struct entry;
  struct entry {
    shared_future<entry> * next = nullptr;
    T value;
  };

  template <class ... A>
  stream(A ... a) : current_(std::forward<A>(a)...) { }

  /**
   * overrides std::future::get(void) method to follow the chain
   */
  T get(void) {
    entry e;
    if (nullptr != current_) {
      try {
        e = current_->get();
      } catch (...) {
        delete current_;
        current_ = nullptr;
        throw;
      }
      delete current_;
      current_ = nullptr;
      current_ = e.next;
    }
    return e.value;
  }

  void wait(void) const {
    if (nullptr != current_) {
      try {
        current_->wait();
      } catch (...) {
        delete current_;
        current_ = nullptr;
        throw;
      }
    }
  }

  /**
   * why even bother? perfect forwarding to std::future::wait_for method.
   */
  template <class ... A>
  std::future_status wait_for(A ... a) {
    std::future_status result;
    if (nullptr != current_) {
      try {
      result = current_->wait_for(std::forward<A>(a)...);
      } catch (...) {
        delete current_;
        current_ = nullptr;
        throw;
      }
    }
    return result;
  }

private:
  shared_future<entry> * current_ = nullptr;
};

} // end of namespace v1
} // end of namespace contacts
