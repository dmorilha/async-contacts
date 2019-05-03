#include "include/contacts.h"

namespace contacts {
namespace v1 {

/**
 * oversimplified comparison
 */
bool Contact::operator == (const Contact & c) const {
  return c.firstName == firstName &&
    c.lastName == lastName &&
    c.phoneNumber == phoneNumber;
}

} // end of namespace v1
} // end of namespace contacts
