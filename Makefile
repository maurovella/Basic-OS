include Makefile.inc

EXE_DIR =./bin
SRC_DIR =./src

EXECUTABLES = application vista #slave
OBJECTS = $(EXECUTABLES:=.o)
OUT = respuesta.txt
UTILS = $(wildcard $(SRC_DIR)/utils/*.c)
SLAVE = $(SRC_DIR)/slave.c

.PHONY: all clean

all: $(EXECUTABLES)
	@mkdir -p $(EXE_DIR); mv -t $(EXE_DIR) $(EXECUTABLES)

application: $(SRC_DIR)/application.c $(UTILS)
	@$(GCC) $< $(UTILS) $(SLAVE) -o $@ $(GCCFLAGS) -lrt -lpthread

vista: $(SRC_DIR)/vista.c $(UTILS)
	@$(GCC) $< $(UTILS) -o $@ $(GCCFLAGS) -lrt -lpthread

clean: 
	@rm -rf $(EXE_DIR)
	@rm -f $(OUT)
