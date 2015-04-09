all: qgen

#-----------------------------------------------------------------------------

MTRAND =
DEBUG =

DFLAG =

#-----------------------------------------------------------------------------

ifneq ($(MTRAND), )
	DFLAG += -DMTRAND
endif

#-----------------------------------------------------------------------------

OBJDIR = obj/
INCDIR = include/
SRCDIR = src/
BINDIR = lib/

#-----------------------------------------------------------------------------

CC     = mpicxx
CUDACC = g++
CFLAG  = -I$(INCDIR)

ifeq ($(DEBUG), )
	CFLAG += -O3
endif

#-----------------------------------------------------------------------------

BINFILE = qgen_cpu
ifneq ($(DEBUG), )
	BINFILE := $(BINFILE)_d
endif
BINFILE := $(BINFILE).a

#-----------------------------------------------------------------------------

FILES = mpicheck pugixml qaco \
        qhill qworld \
        sparams randomizer

ifneq ($(MTRAND), )
	FILES += mtrand
endif

#-----------------------------------------------------------------------------

OBJECTS = $(addprefix $(OBJDIR), $(addsuffix .o, $(FILES)))
CFILES =  $(addprefix $(SRCDIR), $(addsuffix .cpp, $(FILES)))

#-----------------------------------------------------------------------------

qgen: $(OBJECTS)
	@mkdir -p lib
	@echo "\033[30;1;41m "lib" dir was created \033[0m"
	@ar rs $(BINDIR)$(BINFILE) $(OBJECTS)
	@echo "\033[30;1;41m QAntHill builded successfully! \033[0m"
	@echo "\033[30;1;41m --> $(BINDIR)$(BINFILE) \033[0m"

$(OBJDIR)%.o: $(SRCDIR)%.cpp
	@mkdir -p $(OBJDIR)
	@$(CC) -c $(DFLAG) $(CFLAG) $^ -o $@
	@echo "\033[30;1;46m $@ - done \033[0m\n"

clean:
	rm -r -f lib
	rm -r -f obj

#-----------------------------------------------------------------------------
