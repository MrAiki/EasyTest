CC 		    = gcc
CFLAGS 	  = -Wall -Wextra -O0 -g
OBJS	 		= test.o test_main.o
TEST_OBJS	= test_example.o 
TARGET    = test

all: $(TARGET)

run:
	make clean
	make all
	./test

clean:
	rm -f $(OBJS) $(TEST_OBJS) $(TARGET)

$(TARGET) : $(OBJS) $(TEST_OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(TEST_OBJS) -o $(TARGET)

.c.o:
	$(CC) $(CFLAGS) -c $<
