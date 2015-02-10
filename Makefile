COMPILER_FLAGS = -W -Wall -Wextra -Werror -O3 -g
CFLAGS = $(COMPILER_FLAGS) -std=c11
CXXFLAGS = $(COMPILER_FLAGS) -std=c++11
timers: LDLIBS = -lm
lock-overhead: COMPILER_FLAGS += -pthread 
.PHONY: default run
default: timers lock-overhead
run: default
	./lock-overhead
	./timers
