CC = gcc
OBJS = build/*.o
CFLAGS = -I./include
RM = rm -f
server:$(OBJS)
	$(CC) $(OBJS) $(CFLAGS) -o bin/server -lpthread
$(OBJS):src/*.c
	$(CC) $(CFLAGS) -c -o $@ $<
clean:
	$(RM) $(OBJS)
