# Cloud Contacts (version 1)

## Classes
  * `ContactsClient` abstracts the client library
  * `ContactsServer` abstracts a pseudo server
  * `ContactsStorage` abstracts the storage layer

## Asynchronous Interfaces
I decided to use C++ `std::future` over observer due its native support into the language. Besides, most recently major languages have embraced this paradigm over others.

Inspired by other languages, I had to create my own simple `stream` class to abstract an asynchronous series of events. While many methods are missing, it helped me with defining a clear interface for the method `listenForUpdates(void)`

I also created a `shared_promise` class which overrides the method `get_future(void)` returning the same `std::shared_future` which can be copied multiple times.

It is worth noting that Observers could be added as a different interface on top of futures.

## Storage
Since network was purposefully left out, a simple `std::vector` serves as the basis for our storage.

For retrieving the values a copy is made using the algorithm `std::copy`. The copy is necessary to guarantee no changes will be made to the original values.

Interactions are protected by a `std::shared_mutex` which allows the "multiple readers / single writer" paradigm. The famous C++ RAII technique is used to guarantee the right and minimal usage of the locks.

## C++ modern features
Several modern features from C++ were used. They break down into:

Compiler Features:
  * Initialization lists.
  * Variadic templating.
  * Moving semantics and perfect forwarding.
  * Name aliasing.

Standard Library Features:
  * Thread and synchronization supporting libraries.
  * Smart pointers.
  * Random number generator.

## Build
When possible, I like to keep things simple, a `Makefile` fits the purpose in here.

## Compatibility
**C++17** standard is used here. It was tested and successfully ran on:

* Arch Linux with GCC 8.2.1 (preferred)
* MacOSX 10.13.6 with LLVM 10.0.0

## External Dependencies
* Catch2 test framework: https://github.com/catchorg/Catch2
* JSON for Modern C++: https://github.com/nlohmann/json



