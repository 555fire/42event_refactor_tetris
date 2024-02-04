NAME := tetris

CC := cc
CFLAGS := -MMD -MP

LDFLAGS := -lncurses

SRCDIR := srcs/
SRCS := $(SRCDIR)tetris.c

OBJDIR := objs/
OBJS := $(SRCS:$(SRCDIR)%.c=$(OBJDIR)%.o)

INCDIR := incs
INCPATH := -I./$(INCDIR)

DEPDIR := objs/
DEPS := $(OBJS:$(OBJDIR)%.o=$(DEPDIR)%.d)

RM := rm -rf
MKDIR := mkdir -p
