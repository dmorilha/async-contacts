#include "contacts_storage.h"

namespace contacts {
namespace internals {

void ContactsStorage::add(const Contact & c) {
  container_.push_back(c);
};

ContactsStorage::Container::const_iterator
ContactsStorage::begin(void) const {
  return container_.begin();
}

ContactsStorage::Container::const_iterator
ContactsStorage::end(void) const {
  return container_.end();
}

void ContactsStorage::clear(void) {
  return container_.clear();
}


} // end of namespace internals
} // end of namespace contacts
