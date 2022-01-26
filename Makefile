all: test_jack_client_close

LDLIBS += -ljack
CFLAGS += -Wall

%: %.c
