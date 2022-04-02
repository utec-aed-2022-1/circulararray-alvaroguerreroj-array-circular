##
# Array Circular
#
# @file
# @version 0.1

CC=g++
CFLAGS=-I. -Wall -Werror -Wextra -std=c++17
DEPS = circulararray.h circulararray_iterator.h queue.h stack.h tester.h
OBJ = main.o

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

# end
