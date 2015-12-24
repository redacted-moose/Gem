DEBUG = TRUE
CC = gcc
AS = as
CXX= g++
LD = ld

CWD = $(shell pwd)

# CFLAGS = -Wall -W -fPIC --std=c99 `sdl-config --cflags`
# LDFLAGS = -Wall -W -shared --std=c99 `sdl-config --libs`

CFLAGS = -Wall
CFLAGS += -W
CFLAGS += --std=c99
CFLAGS += `sdl-config --cflags`

LDFLAGS = -Wall
LDFLAGS += -W
LDFLAGS += --std=c99
LDFLAGS += `sdl-config --libs`

ifeq ($(DEBUG),FALSE)
	CFLAGS += -Os
else
	CFLAGS += -Og -g -D DEBUG
    # LDFLAGS += --debug
endif

CSOURCES = $(wildcard *.c) $(wildcard cpu/*.c) graphics/sdl.c
ASMSOURCES = $(wildcard *.S) $(wildcard cpu/*.S)
CPPOBJS = $(patsubst %.cpp,%.o,$(wildcard *.cpp)) $(patsubst %.cpp,%.o,$(wildcard cpu/*.cpp))
OBJS = $(patsubst %.c,%.o,$(CSOURCES)) $(patsubst %.S,%.o,$(ASMSOURCES)) $(CPPOBJS)
TEST_OBJS := $(filter-out main.o,$(OBJS))

UNITY_ROOT = ../Unity
TARGET_BASE1 = test_cpu
TARGET_BASE2 = test_mmu
TARGET_BASE3 = test_arith8
TARGET1 = $(TARGET_BASE1)$(TARGET_EXTENSION)
TARGET2 = $(TARGET_BASE2)$(TARGET_EXTENSION)
TARGET3 = $(TARGET_BASE3)$(TARGET_EXTENSION)
SRC_FILES1 = test/unity.c test/TestCPU.c test/test_runners/TestCPU_Runner.c
SRC_FILES2 = test/unity.c test/TestMMU.c test/test_runners/TestMMU_Runner.c
SRC_FILES3 = test/unity.c test/TestArith8.c test/test_runners/TestArith8_Runner.c
OBJ_FILES1 = unity.o TestCPU.o TestCPU_Runner.o
OBJ_FILES2 = unity.o TestMMU.o TestMMU_Runner.o
OBJ_FILES3 = unity.o TestArith8.o TestArith8_Runner.o
INC_DIRS = -I$(CWD) -I$(CWD)/cpu -I$(UNITY_ROOT)/src
SYMBOLS = -DTEST

ifneq ($(strip $(CPPOBJS)),)
	LDFLAGS += --cpp
endif
# EXE = gem.so
# DISTDIR = lib
EXE = gem.bin
DISTDIR = bin
vpath %.tns $(DISTDIR)


all: $(EXE)

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

%.o: %.cpp
	$(CXX) -c $< -o $@ $(CFLAGS)

%.o: %.S
	$(AS) -c $< -o $@

$(EXE): $(OBJS)
	mkdir -p $(DISTDIR)
# 	$(LD) $^ -o $(DISTDIR)/$@ $(LDFLAGS)
# ifeq ($(DEBUG),FALSE)
# 	@rm -f $(DISTDIR)/*.gdb
# endif
	$(CC) $^ -o $(DISTDIR)/$@ $(LDFLAGS)

.PHONY: test
test: $(TEST_OBJS)
	ruby $(UNITY_ROOT)/auto/generate_test_runner.rb test/TestCPU.c test/test_runners/TestCPU_Runner.c
	ruby $(UNITY_ROOT)/auto/generate_test_runner.rb test/TestMMU.c test/test_runners/TestMMU_Runner.c
	ruby $(UNITY_ROOT)/auto/generate_test_runner.rb test/TestArith8.c test/test_runners/TestArith8_Runner.c
	$(CC) $(CFLAGS) $(INC_DIRS) $(SYMBOLS) -c $(SRC_FILES1)
	$(CC) $(CFLAGS) $(INC_DIRS) $(SYMBOLS) -c $(SRC_FILES2)
	$(CC) $(CFLAGS) $(INC_DIRS) $(SYMBOLS) -c $(SRC_FILES3)
	$(CC) $^ $(OBJ_FILES1) -o $(DISTDIR)/$(TARGET1) $(LDFLAGS)
	$(CC) $^ $(OBJ_FILES2) -o $(DISTDIR)/$(TARGET2) $(LDFLAGS)
	$(CC) $^ $(OBJ_FILES3) -o $(DISTDIR)/$(TARGET3) $(LDFLAGS)
	$(DISTDIR)/$(TARGET1)
	$(DISTDIR)/$(TARGET2)
	$(DISTDIR)/$(TARGET3)

.PHONY: clean
clean:
	rm -f $(OBJS) $(OBJ_FILES1) $(OBJ_FILES2) *.elf $(DISTDIR)/*.gdb $(DISTDIR)/$(EXE) $(DISTDIR)/$(TARGET1) $(DISTDIR)/$(TARGET2)
