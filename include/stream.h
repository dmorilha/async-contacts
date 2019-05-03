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

  /**
   * value is wrapped up in an instance of the entry class
   * which chains the next future.
   */
  struct entry;
  struct entry {
    shared_future<entry> next;
    T value;
  };

  template <class ... A>
  stream(A ... a) : current_(forward<A>(a)...) { }

  /**
   * overrides std::future::get(void) method to follow the chain
   */
  T get(void) {
    const entry e = current_.get();
    current_ = e.next;
    return e.value;
  }

  void wait(void) const {
    current_.wait();
  }

  /**
   * why even bother? perfect forwarding to std::future::wait_for method.
   */
  template <class ... A>
  auto wait_for(A ... a) const {
    return current_.wait_for(forward<A>(a)...);
  }

private:
  shared_future<entry> current_;
};

} // end of namespace v1
} // end of namespace contacts
