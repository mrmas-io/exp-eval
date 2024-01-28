CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic

SRCS = main.c
TARGET = main

ifeq ($(findstring Windows_NT,$(OS)),Windows_NT)
	TARGET_EXTENSION = .exe
	RM = del /Q
else
	TARGET_EXTENSION =
	RM = rm -rf
endif

$(TARGET)$(TARGET_EXTENSION): $(SRCS)
	@$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

run: $(TARGET)$(TARGET_EXTENSION)
	@./$(TARGET)$(TARGET_EXTENSION)
clean:
	@$(RM) $(TARGET)$(TARGET_EXTENSION)