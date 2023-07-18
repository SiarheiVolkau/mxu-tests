CROSS_COMPILE ?= mipsel-linux-
CFLAGS ?= -O2 -mmxu=r2 -Wall -Wextra -pedantic -std=gnu11
SOURCES = main.c \
	helpers.c \
	load_store_tests.c \
	slt_condmove_tests.c \
	bitwise_shifts_tests.c \
	multiplication_tests.c \
	arithmetic_tests.c

HEADERS = helpers.h

testapp: $(SOURCES) $(HEADERS) Makefile
	$(CROSS_COMPILE)gcc $(CFLAGS) -o testapp $(SOURCES)

clean:
	rm testapp
