vpath %.c src

OBJ_PATH = build
SRC_PATH = src
BIN_PATH = bin
TARGET = server
CC = gcc
CFLAGS = -g -I./include
RM = rm -f
SRC= $(foreach x,$(SRC_PATH),$(wildcard $(addprefix $(x)/*,.c)))
OBJS = $(addprefix $(OBJ_PATH)/, $(addsuffix .o,$(notdir $(basename $(SRC)))))
$(BIN_PATH)/$(TARGET):$(OBJS)
	$(CC) $(OBJS) $(CFLAGS) -o $(BIN_PATH)/$(TARGET) -lpthread
$(OBJ_PATH)/%.o:%.c
	$(CC) $(CFLAGS) -c -o $@ $<
clean:
	$(RM) $(OBJS) $(BIN_PATH)/*
