TOOLS_CFLAGS        := -Wstrict-prototypes -Wshadow -Wpointer-arith -Wcast-qual \
                   -Wcast-align -Wwrite-strings -Wnested-externs -Winline \
                   -W -Wundef -Wmissing-prototypes

all: Wrapper

Wrapper: wrapper.o Wrapper_ain.c gpio-utils.c gpio-utils.h
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) $(TOOLS_CFLAGS) -c $< -o $@

clean: rm wrapper.o wrapper
	
