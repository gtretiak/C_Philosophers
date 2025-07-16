NAME = philo
HEADER = philo.h
CFLAGS = -Wall -Wextra -Werror
SPECFLAG = -pthread
SPECFLAG2 = -fsanitize=thread # to detect data races, incorrect locking, use-after-destroy bugs and thread leaks
SRCS = main.c args.c core_flow.c dinner.c eating.c free_and_errors.c \
       get_and_set.c init_and_set.c lone_philo.c output.c reflecting.c \
       supervision.c time.c utils.c waiter.c
DIR = ./
OBJS = $(addprefix $(DIR), $(SRCS:.c=.o))
TMP = .tmp

all: $(NAME)

$(DIR)%.o: $(DIR)%.c $(HEADER)
	@if [ ! -f $(TMP) ]; then \
		echo "Compiling source files into object files...";\
		touch $(TMP); \
	fi
	@cc $(CFLAGS) -g -I. -c $< -o $@

$(NAME): $(OBJS)
	@echo "Linking object files into the final executable...";
	@cc $(OBJS) -o $@ $(SPECFLAG) $(SPECFLAG2)
	@echo "Done.\n\n\n";
	@echo "Welcome to the \"🇬 🇷 🇪 🇪 🇨 🇪 "\
	"\" cafe!\n"\
	"Run the program with arguments:\n"\
	" 1. ./philo\n 2." \
	"number_of_philosophers\n 3. time_to_die\n 4. time_to_eat\n 5. "\
	"time_to_sleep\n+6. [optional] number_of_times_each_philosopher_must_eat"
	@rm -rf $(TMP);

.PHONY: clean fclean re all

clean:
	@rm -f $(OBJS)

fclean: clean
	@rm -f $(NAME)

re: fclean all
