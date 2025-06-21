NAME = preliminary
HEADER = preliminary.h
CFLAGS = -Wall -Wextra -Werror
SPECFLAGS = -pthread -fsanitize=thread
SRCS = main.c times.c threads_id.c threads.c mutexes.c threads_in_loop.c \
       threads_return.c threads_take.c threads_separate_work.c
DIR = ./
OBJS = $(addprefix $(DIR), $(SRCS:.c=.o))
TMP = .tmp

all: $(NAME)

$(DIR)%.o: $(DIR)%.c $(HEADER)
	@if [ ! -f $(TMP) ]; then \
		echo "Compiling source files into object files...";\
		touch $(TMP); \
	fi
	@cc $(CFLAGS) -g -I$(HEADER) -c $< -o $@

$(NAME): $(OBJS)
	@echo "Linking object files into the final executable...";
	@cc $(OBJS) -o $@ $(SPECFLAGS)
	@echo "Done.";
	@rm -rf $(TMP);

.PHONY: clean fclean re all

clean:
	@rm -f $(OBJS)

fclean: clean
	@rm -f $(NAME)

re: fclean all
