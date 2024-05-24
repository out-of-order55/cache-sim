
NAME := $(shell basename $(PWD))
all: $(NAME)-64
SRCS   := $(shell find . -maxdepth 1 -name "*.cc")
DEPS   := $(shell find . -maxdepth 1 -name "*.h") $(SRCS)
CFLAGS += -O2 -std=c++11 -ggdb -pg
LDFLAGS+= -lm
.PHONY: all 

$(NAME)-64: $(DEPS) # 64bit binary
	g++ -m64 $(CFLAGS) $(SRCS) -o $@ $(LDFLAGS)

clean:
	rm -f $(NAME)-64
# include ../Makefile
