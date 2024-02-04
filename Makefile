include variable.mk

$(NAME): $(OBJDIR) $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@

$(OBJDIR)%.o: $(SRCDIR)%.c
	$(CC) $(CFLAGS) $(INCPATH) -c $< -o $@

$(OBJDIR):
	$(MKDIR) objs

all: $(NAME)

clean:
	$(RM) $(OBJS)
	$(RM) $(DEPS)
	$(RM) $(OBJDIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
