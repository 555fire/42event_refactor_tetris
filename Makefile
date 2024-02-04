include variable.mk

# $(NAME): $(OBJS)
$(NAME): $(OBJDIR) $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@
#	$(CC) $(CFLAGS) $(LDFLAGS) $(INCPATH) $(OBJS) -o $@

# $(OBJDIR)%.o: $(SRCDIR)%.c $(OBJDIR) $(DEPDIR)
$(OBJDIR)%.o: $(SRCDIR)%.c
	$(CC) $(CFLAGS) $(INCPATH) -c $< -o $@

$(OBJDIR):
	$(MKDIR) objs

# $(DEPDIR):
# 	$(MKDIR) deps

all: $(NAME)

clean:
	$(RM) $(OBJS)
	$(RM) $(DEPS)
	$(RM) $(OBJDIR)
#	$(RM) $(DEPDIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
