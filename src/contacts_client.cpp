#include <algorithm>

#include <cassert>

#include "contacts_client.h"

namespace contacts {
namespace v1 {

using namespace contacts::internals;

ContactsClient::ContactsClient(const launch l) :
  server_(ContactsServer::connect()), policy_(l) { }

future<RESULT> ContactsClient::addContact(const Contact & c) {
  assert(server_);
  return async(policy_, [this, &c](void) -> auto {
    assert(server_);
    return server_->addContact(c) ? RESULT::SUCCESS : RESULT::FAILURE;
  });
}

future<vector<Contact>> ContactsClient::listContacts(void) {
  assert(server_);
  return async(policy_, [this](void) -> auto {
    assert(server_);
    return server_->listContacts();
  });
}

stream<Contact> ContactsClient::listenForUpdates(void) {
  assert(server_);
  return server_->listenForUpdates();
}

future<RESULT> ContactsClient::clear(void) {
  assert(server_);
  return async(policy_, [this](void) -> auto {
    assert(server_);
    return server_->clear() ? RESULT::SUCCESS : RESULT::FAILURE;
  });
}

} // end of namespace v1
} // end of namespace contacts
