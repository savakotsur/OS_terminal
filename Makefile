CPPFLAGS = -Wall -Werror -Wextra -std=c++17

all:
	g++ $(CPPFLAGS) terminal.cpp

clean:
	rm -rf *.out