#include "include/contacts.h"

#include "contacts_client.h"

namespace contacts {
namespace v1 {

Contacts::Contacts(void) { }

Contacts::~Contacts() { }

unique_ptr<Contacts> Contacts::create(void) {
  return make_unique<ContactsClient>();
}

unique_ptr<Contacts> Contacts::create(const launch l) {
  return make_unique<ContactsClient>(l);
}

} // end of namespace v1
} // end of namespace contacts
