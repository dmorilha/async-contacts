CXXFLAGS += -I.
CXXFLAGS += -std=c++17

LDFLAGS += -lpthread

PLATFORM = $(shell uname -s;)
CXXFLAGS += -D${PLATFORM}

ifeq ($(PLATFORM), Linux)
  SO_EXTENSION = so
endif
ifeq ($(PLATFORM), Darwin)
  SO_EXTENSION = dylib
endif

bin/test : test/test_contact.cpp bin/libcontacts.${SO_EXTENSION}
	mkdir -pv bin;
	${CXX} ${CXXFLAGS} ${LDFLAGS} -o $@ $^;

bin/libcontacts.${SO_EXTENSION}: src/*.cpp
	mkdir -pv bin;
	${CXX} ${CXXFLAGS} ${LDFLAGS} -fPIC -shared -o $@ $^;
