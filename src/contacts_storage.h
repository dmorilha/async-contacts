#pragma once

#include <vector>

#include "include/contacts.h"

namespace contacts {
namespace internals {

using namespace contacts::v1;

class ContactsStorage {
  using Container = vector<Contact>;
  Container container_;

public:
  void add(const Contact &);
  Container::const_iterator begin(void) const;
  Container::const_iterator end(void) const;
  void clear(void);
};

} // end of namespace internals
} // end of namespace contacts
