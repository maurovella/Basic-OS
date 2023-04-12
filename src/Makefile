include Makefile.inc

EXECUTABLES = app vista
OBJECTS = $(EXECUTABLES)
SOURCES_UTILS = $(wildcard utils/*.c)

OUT_APP = app
OUT_SLAVE = slave
OUT_VISTA = vista

all: $(EXECUTABLES)

app:
	gcc application.c utils/* -o app -Wall -std=c99 -lrt -pthread
slave: 

vista: 
	gcc vista.c utils/* -o vista -Wall -std=c99 -lrt -pthread