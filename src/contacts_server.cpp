#include "contacts_server.h"

namespace contacts {
namespace internals {

/**
 * addresses possible race condition: https://stackoverflow.com/a/11711991
 */
shared_ptr<ContactsServer> ContactsServer::connect(void) {
  static shared_ptr<ContactsServer> instance(new ContactsServer);
  return instance;
}

ContactsServer::ContactsServer(void) { }

bool ContactsServer::addContact(const Contact & c) {
  shared_promise<StreamEntry> next;
  {
    UniqueLock lock(mutex_);
    try {
      storage_.add(c);
    } catch (exception &) {
      return false;
    }
    updatesStream_.set_value({next.get_future(), c});
    swap(updatesStream_, next);
  }
  return true;
}

vector<Contact> ContactsServer::listContacts(void) {
  vector<Contact> c;
  {
    SharedLock lock(mutex_);
    copy(storage_.begin(), storage_.end(), back_inserter(c));
  }
  return c;
}

stream<Contact> ContactsServer::listenForUpdates(void) {
  SharedLock lock(mutex_);
  return stream<Contact>(updatesStream_.get_future());
}

bool ContactsServer::clear(void) {
  UniqueLock lock(mutex_);
  try {
    storage_.clear();
  } catch (exception &) {
    return false;
  }
  return true;
}

} // end of namespace internals
} // end of namespace contacts
