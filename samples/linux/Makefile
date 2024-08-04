CC = gcc
CFLAGS = -Wall -g
C_DEF = _SAMPLE_LINUX_
C_SRC = $(wildcard *.c ../../kernel/*.c)
C_INC = -I ../../kernel -I ../../ -I ../../ports
#C_OBJ = $(patsubst %.c, %.o, $(notdir $(C_SRC)))
LIBS += -lpthread

TARGET = mos_linux_sample.a

all:

	@mkdir -p ./output
	$(CC) $(C_SRC) $(CFLAGS) -D$(C_DEF) $(C_INC) -o output/$(TARGET) 

clean:

	@rm -fr ./output