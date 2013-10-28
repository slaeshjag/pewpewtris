# Project: libDarnit

OUTBIN		=	../bin/pewpewtris.elf

DEPENDS		=

CFLAGS		+=	-O0 -g
LDFLAGS		+=	-ldarnit
SRCFILES	=	$(wildcard *.c)
OBJFILES	=	$(SRCFILES:.c=.o)
.PHONY: all clean

all: $(OBJFILES) $(DEPENDS)
	@echo " [ LD ] $(OUTBIN)"
	@$(CC) $(CFLAGS) -o $(OUTBIN) $(OBJFILES) $(LDFLAGS)
	@echo "Done."
	@echo
	
clean:
	@echo
	@echo " [ RM ] $(OBJFILES)"
	@$(RM) $(OBJFILES)
	@echo "Done."
	@echo 

%.o: %.c %.h
	@echo " [ CC ] darnit/$<"
	@$(CC) $(CFLAGS) -c -o $@ $<
	
