##
# Array Circular
#
# @file
# @version 0.1

CC=g++
CFLAGS=-I. -Wall -Werror -Wextra -std=c++17
DEPS = circulararray.h circulararray_iterator.h queue.h stack.h tester.h
OBJ = main.o

BUILD_DIR = build

%.o: %.cpp $(DEPS)
	$(CC) -c -o $(BUILD_DIR)/$@ $< $(CFLAGS)

main: $(OBJ)
	$(CC) -o $(BUILD_DIR)/$@ $(BUILD_DIR)/$^ $(CFLAGS)

.PHONY: format
format:
	fd -e c -e cpp -e h -e hpp -x clang-format -i {}

# end
