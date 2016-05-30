DEBUG ?= TRUE
CC ?= gcc
AS ?= as
CXX ?= g++
LD ?= ld

CWD = $(shell pwd)

BACKEND ?= sdl

# CFLAGS = -Wall -W -fPIC --std=c99 `sdl-config --cflags`
# LDFLAGS = -Wall -W -shared --std=c99 `sdl-config --libs`

CFLAGS = -Wall \
         -pedantic \
         -Wstrict-aliasing \
         -Wstrict-overflow \
         -W \
         --std=c99 \
         -march=native


ifeq ($(DEBUG),FALSE)
	CFLAGS += -Os
else
	CFLAGS += -Og \
            -g \
            -D WARN
            # -D DEBUG
	          # -Werror \
	          # -Wshadow
endif

LDFLAGS = -Wall \
          -W \
          --std=c99 \
          -flto

ifeq ($(BACKEND), sdl)
	CFLAGS += -D SDL \
            `sdl-config --cflags`
	LDFLAGS += `sdl-config --libs`
else ifeq ($(BACKEND), sdl2)
	CFLAGS += -D SDL2 \
            `sdl2-config --cflags`
	LDFLAGS += `sdl2-config --libs`
endif

OBJDIR = build

CSOURCES = $(wildcard *.c) $(wildcard cpu/*.c) graphics/$(BACKEND).c
ASMSOURCES = $(wildcard *.S) $(wildcard cpu/*.S)
CPPOBJS = $(patsubst %.cpp,%.o,$(wildcard *.cpp)) $(patsubst %.cpp,%.o,$(wildcard cpu/*.cpp))
OBJS = $(patsubst %.c,%.o,$(CSOURCES)) $(patsubst %.S,%.o,$(ASMSOURCES)) $(CPPOBJS)
OBJS := $(addprefix $(OBJDIR)/,$(OBJS))
TEST_OBJS := $(filter-out $(OBJDIR)/main.o,$(OBJS))

UNITY_ROOT = test/Unity
TARGETS = TestCPU TestMMU TestArith8 TestGPU
GEN_RUNNER = ruby $(UNITY_ROOT)/auto/generate_test_runner.rb
UNITY_OBJ_FILES = $(OBJDIR)/test/unity.o $(addprefix $(OBJDIR)/test/,$(TARGETS:%=%.o) $(TARGETS:%=%_Runner.o))
INC_DIRS = -I$(CWD) -I$(CWD)/cpu -I$(UNITY_ROOT)/src
SYMBOLS = -DTEST

ifneq ($(strip $(CPPOBJS)),)
	LDFLAGS += --cpp
endif

# EXE = gem.so
# DISTDIR = lib
EXE = gem.bin
DISTDIR = bin

all: $(EXE)

gem.yucc: gem.yuck
	yuck gen $< > $@
# yuck scmver $< > $@

$(OBJDIR)/%.o: %.c gem.yucc
	$(CC) -c $< -o $@ $(CFLAGS)

$(OBJDIR)/%.o: %.cpp
	$(CXX) -c $< -o $@ $(CFLAGS)

$(OBJDIR)/%.o: %.S
	$(AS) -c $< -o $@

$(OBJDIR)/test/unity.o: $(UNITY_ROOT)/src/unity.c
	$(CC) -c $< -o $@ $(CFLAGS) $(INC_DIRS) $(SYMBOLS)

$(OBJDIR)/test/%.o: test/%.c $(OBJDIR)/test/unity.o $(TEST_OBJS)
	$(GEN_RUNNER) $< $(patsubst test/%.c,test/%_Runner.c,$<)
	$(CC) -c $< -o $@ $(CFLAGS) $(INC_DIRS) $(SYMBOLS)
	$(CC) -c $(patsubst test/%.c,test/%_Runner.c,$<) -o $(patsubst $(OBJDIR)/test/%.o,$(OBJDIR)/test/%_Runner.o,$@) $(CFLAGS) $(INC_DIRS) $(SYMBOLS)
	$(CC) $(TEST_OBJS) $(addprefix $(OBJDIR)/test/,unity.o) $@ $(patsubst $(OBJDIR)/test/%.o,$(OBJDIR)/test/%_Runner.o,$@) -o $(DISTDIR)/$(patsubst $(OBJDIR)/test/%.o,%,$@) $(LDFLAGS)

$(OBJS): | $(OBJDIR)

$(OBJDIR):
	mkdir -p $@
	mkdir -p $@/cpu
	mkdir -p $@/graphics
	mkdir -p $@/test
	mkdir -p $@/test/test_runners

$(EXE): $(OBJS)
	mkdir -p $(DISTDIR)
	$(CC) $^ -o $(DISTDIR)/$@ $(LDFLAGS)

.PHONY: $(TARGETS)
$(TARGETS): $(TEST_OBJS) $(UNITY_OBJ_FILES)

.PHONY: test
test: $(TARGETS)
	for target in $^; do \
		$(DISTDIR)/$$target; \
	done

.PHONY: clean
clean:
	rm -f *.elf $(DISTDIR)/*.gdb $(DISTDIR)/$(EXE) $(addprefix $(DISTDIR)/,$(TARGETS)) *.yucc
	rm -rf $(OBJDIR)
