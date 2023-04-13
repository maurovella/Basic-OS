include Makefile.inc

EXE_DIR =./bin
SRC_DIR =./src

EXECUTABLES = application vista #slave
OBJECTS = $(EXECUTABLES:=.o)
UTILS = $(wildcard $(SRC_DIR)/utils/*.c)

.PHONY: all clean
all: $(EXECUTABLES)
	@mkdir -p $(EXE_DIR); mv -t $(EXE_DIR) $(EXECUTABLES)

application: $(SRC_DIR)/application.c $(UTILS)
	@$(GCC) $< $(UTILS) -o $@ $(GCCFLAGS) -lrt -lpthread

vista: $(SRC_DIR)/vista.c $(UTILS)
	@$(GCC) $< $(UTILS) -o $@ $(GCCFLAGS) -lrt -lpthread

slave: $(SRC_DIR)/slave.c
	@$(GCC) $< -o $@ $(GCCFLAGS)

clean: 
	@rm -rf $(EXE_DIR)
