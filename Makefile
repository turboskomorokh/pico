CC          :=
STRIP		:=
CFLAGS      := -Os -fdata-sections -ffunction-sections -Wl,--gc-sections

ifeq ($(CC),)
CC          := gcc
endif

ifeq ($(STRIP),)
STRIP		:= strip
endif

ifneq ($(shell which upx),)
UPX_P		:= $(shell which upx)
endif

CSOURCES    := $(shell find src/*.c)
BIN         := pico
LIBFLAGS	:= -larchive -lacl -llzma

COBJ        = $(CSOURCES:.c=.o)
all: pico strip upx
	

strip:
	@echo "  STRIP $(BIN)"
	@$(STRIP) $(BIN)

pico: $(COBJ)
	@echo "  CCLD  $(BIN)"
	@$(CC) $(CFLAGS) -o $(BIN) $^ $(LIBFLAGS)

%.o: %.c
	@echo "  CC    $@"
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "Cleaning up..."
	@rm -f $(COBJ) $(BIN)

ifneq ($(UPX_P),)
upx:
	@echo "  UPX   $(BIN)"
	@$(UPX_P) -9q $(BIN) > /dev/null 2>&1
else
upx:
	
endif