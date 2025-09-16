#include <chrono>
#include <iostream>
#include <random>
#include <string>
#include <thread>
#include <vector>

#include <cassert>

#define CATCH_CONFIG_MAIN
#include "vendor/catch2/catch.hpp"

#include "vendor/nlohmann/json.hpp"

#include "include/contacts.h"

using namespace std;
using namespace contacts::v1;

/**
 * symbol is exported as "C" naming for one reason or another
 */
const string myContacts = "[{\"first\" : \"Alexander\",\"last\" : \"Bell\",\"phone\" : \"+16170000001\"},"
    "{\"first\" : \"Thomas\",\"last\" : \"Watson\",\"phone\" : \"+16170000002\"},"
    "{\"first\" : \"Elisha\",\"last\" : \"Gray\",\"phone\" : \"+18476003599\"},"
    "{\"first\" : \"Antonio\",\"last\" : \"Meucci\",\"phone\" : \"+17188763245\"},"
    "{\"first\" : \"Guglielmo\",\"last\" : \"Marconi\",\"phone\" : \"+39051203222\"},"
    "{\"first\" : \"Samuel\",\"last\" : \"Morse\",\"phone\" : \"+16172419876\"},"
    "{\"first\" : \"Tim\",\"last\" : \"Berners-Lee\",\"phone\" : \"+44204549898\"},"
    "{\"first\" : \"John\",\"last\" : \"Baird\",\"phone\" : \"+4408458591006\"},"
    "{\"first\" : \"Thomas\",\"last\" : \"Edison\",\"phone\" : \"+19086575678\"}]";

ostream & operator << (ostream & o, const Contact & c) {
  o << c.firstName << " " << c.lastName << " - " << c.phoneNumber;
  return o;
}

/**
 * parses json string into a vector of Contacts
 */
vector<Contact> parseJson(const string & s) {
  using json = nlohmann::json;
  vector<Contact> contacts;
  try {
    const auto array = json::parse(s);
    for (const auto & item : array) {
      contacts.emplace_back(Contact{
        .firstName = item["first"],
        .lastName = item["last"],
        .phoneNumber = item["phone"],
      });
    }
  } catch (exception &) {
    std::cerr << "error while parsing the json" << endl;
  }
  return contacts;
}

void addContacts(const vector<Contact> & v, const shared_ptr<Contacts> & c) {
  assert(c);
  for (const auto & item : v) {
    c->addContact(item);
  }
}

TEST_CASE("add async contact") {
  const Contact contact {
    .firstName = "Daniel",
    .lastName = "Morilha",
    .phoneNumber = "+1 408 480 7050",
  };

  SECTION("add contact") {
    unique_ptr<Contacts> client = Contacts::create(launch::async);
    const auto result = client->addContact(contact).get();
    REQUIRE(RESULT::SUCCESS == result);
  }

  SECTION("listing contacts") {
    unique_ptr<Contacts> client = Contacts::create(launch::async);
    const auto contacts = client->listContacts().get();
    REQUIRE(1 == contacts.size());
    REQUIRE(contact == contacts[0]);
  }

  SECTION("cleaning contacts") {
    unique_ptr<Contacts> client = Contacts::create(launch::async);
    const auto result = client->clear().get();
    REQUIRE(RESULT::SUCCESS == result);
  }
}

TEST_CASE("add deferred contact") {
  const Contact contact {
    .firstName = "Daniel",
    .lastName = "Morilha",
    .phoneNumber = "+1 408 480 7050",
  };

  unique_ptr<Contacts> client = Contacts::create(launch::deferred);

  SECTION("add contact") {
    const auto result = client->addContact(contact).get();
    REQUIRE(RESULT::SUCCESS == result);
  }

  SECTION("listing contacts") {
    const auto contacts = client->listContacts().get();
    REQUIRE(1 == contacts.size());
    REQUIRE(contact == contacts[0]);
  }

  SECTION("cleaning contacts") {
    const auto result = client->clear().get();
    REQUIRE(RESULT::SUCCESS == result);
  }
}

TEST_CASE("listening for updates") {
  const auto contacts = parseJson(myContacts);

  //random device, I absolutely <3 C++
  random_device rd;
  mt19937 mt(rd());

  // TODO: the use of a Catch2's section here breaks the stream.
  // SECTION adding contacts asynchronously from a client
  auto waiter = async(launch::async, [=, &mt](void) {
    uniform_int_distribution<> dist(100, 501);
    unique_ptr<Contacts> client = Contacts::create(launch::async);
    for (const auto & item : contacts) {
      std::this_thread::sleep_for(chrono::milliseconds(dist(mt)));
      const auto result = client->addContact(item).get();
      REQUIRE(RESULT::SUCCESS == result);
    }
  });

  SECTION("listening for updates from another client") {
    uniform_int_distribution<> dist(501, 800);
    unique_ptr<Contacts> client = Contacts::create(launch::async);
    auto stream = client->listenForUpdates();
    for (const auto & item : contacts) {
      switch (stream.wait_for(chrono::milliseconds(dist(mt)))) {
      case future_status::ready:
        {
          const auto contact = stream.get();
          REQUIRE(contact == item);
        }
        break;

      default:
        REQUIRE(!"Unreachable");
        break;
      }
    }
    REQUIRE("reached another end");
  }

  SECTION("listening for updates from yet another client") {
    unique_ptr<Contacts> client = Contacts::create(launch::deferred);
    auto stream = client->listenForUpdates();
    for (const auto & item : contacts) {
      switch (stream.wait_for(800ms)) {
      case future_status::ready:
        {
          const auto contact = stream.get();
          REQUIRE(contact == item);
        }
        break;

      default:
        REQUIRE(!"Unreachable");
        break;
      }
    }
    REQUIRE("reached yet another end");
  }

  waiter.wait();
  REQUIRE("reached one end");

  SECTION("cleaning contacts") {
    unique_ptr<Contacts> client = Contacts::create(launch::async);
    const auto result = client->clear().get();
    REQUIRE(RESULT::SUCCESS == result);
  }
}
