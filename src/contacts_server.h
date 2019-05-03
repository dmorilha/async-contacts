#pragma once

#include <future>
#include <memory>
#include <shared_mutex>

#include "include/contacts.h"
#include "include/stream.h"

#include "contacts_storage.h"

namespace contacts {
namespace internals {

using namespace std;

template <class T>
class shared_promise : public promise<T> {
public:
  shared_future<T> get_future(void) {
    if (!future_.valid()) {
      future_ = promise<T>::get_future();
    }
    return future_;
  }

private:
  shared_future<T> future_;
};

class ContactsServer {
public:
  static shared_ptr<ContactsServer> connect(void);

  bool addContact(const Contact &);
  vector<Contact> listContacts(void);
  stream<Contact> listenForUpdates(void);
  bool clear(void);

private:
  using SharedLock = shared_lock<shared_mutex>;
  using UniqueLock = unique_lock<shared_mutex>;
  using StreamEntry = stream<Contact>::entry;

  ContactsServer(void);

  ContactsStorage storage_;
  shared_mutex mutex_;
  shared_promise<StreamEntry> updatesStream_;
};

} // end of namespace internals
} // end of namespace contacts
