TOOLS_CFLAGS        := -Wstrict-prototypes -Wshadow -Wpointer-arith -Wcast-qual \
                   -Wcast-align -Wwrite-strings -Wnested-externs -Winline \
                   -W -Wundef -Wmissing-prototypes

all: snake

snake: snake.o
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) $(TOOLS_CFLAGS) -c $< -o $@

clean:
	rm snake.o
