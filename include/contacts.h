#pragma once

#include <future>
#include <string>
#include <vector>

#include "include/stream.h"

namespace contacts {
namespace v1 {

/**
 * structure to hold Contact's information.
 */
struct Contact {
  string firstName;
  string lastName;
  //TODO: it could be better validated with https://github.com/googlei18n/libphonenumber
  string phoneNumber;

  bool operator == (const Contact &) const;
};

/**
 * asynchronous API result enumeration
 */
enum class RESULT {
  SUCCESS,
  FAILURE,
  TIMEOUT,
};

/**
 * abstract class to interact with contacts "Cloud" API
 */
class Contacts {
protected:
  Contacts(void);

public:
  /**
   * creates an instance calling the empty constructor.
   */
  static unique_ptr<Contacts> create(void);

  /**
   * creates an instance setting the std::launch option
   * to control std::async policy making it convenient for the exercise.
   *
   * reference: https://en.cppreference.com/w/cpp/thread/launch
   */
  static unique_ptr<Contacts> create(const launch l);

  /**
   * virtual destructor
   */
  virtual ~Contacts();

  /**
   * addContact takes a reference to a contact,
   * and returns a future whether the operation succeeded.
   */
  virtual future<RESULT> addContact(const Contact &) = 0;

  /**
   * listContacts returns a future of a vector of contacts.
   */
  virtual future<vector<Contact>> listContacts(void) = 0;

  /**
   * listenForUpdates returns a stream of latest added contacts.
   */
  virtual stream<Contact> listenForUpdates(void) = 0;

  /**
   * clear removes all contacts from the server.
   */
  virtual future<RESULT> clear(void) = 0;
};

} // end of namespace v1
} // end of namespace contacts
