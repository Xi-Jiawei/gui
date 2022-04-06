QT += widgets

HEADERS	    =	window.h \
                tumbler.h
SOURCES	    =	window.cpp \
                tumbler.cpp \
		main.cpp

# temp
MOC_DIR = ./.temp
OBJECTS_DIR	= ./.temp
RCC_DIR	= ./.temp

# install
target.path = ./.temp
INSTALLS += target

QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O0
