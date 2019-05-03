#pragma once

#include <memory>

#include "include/contacts.h"

#include "contacts_server.h"

namespace contacts {
namespace v1 {

using namespace contacts::internals;

class ContactsClient : public Contacts {
public:
  ContactsClient(const launch l = launch::deferred);
  future<RESULT> addContact(const Contact &);
  future<vector<Contact>> listContacts(void);
  stream<Contact> listenForUpdates(void);
  future<RESULT> clear(void);

private:
  shared_ptr<ContactsServer> server_;
  launch policy_;
};

} // end of namespace v1
} // end of namespace contacts
