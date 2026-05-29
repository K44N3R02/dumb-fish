# --- Toolchain ---
COMPILE = gcc -c
LINK    = gcc
DEPEND  = gcc -MM -MG -MF
CFLAGS  = -Wall -Wextra -Isrc -Isrc/engine -Isrc/display -Isrc/utils -Iunity/src

# --- Directories ---
PATHB = bin/
PATHO = $(PATHB)objs/
PATHD = $(PATHB)depends/
PATHR = $(PATHB)results/

BUILD_PATHS = $(PATHB) $(PATHO) $(PATHD) $(PATHR)

# --- Source Files ---
SRCS = $(shell find src -name '*.c')
LIB_SRCS = $(filter-out src/main.c, $(SRCS))
TEST_SRCS = $(wildcard test/*.c test/**/*.c)
UNITY_SRC = unity/src/unity.c

# --- Object & Dependency Mapping (Tree Preserved) ---
# Maps src/engine/board.c -> bin/objs/src/engine/board.o
NORMAL_OBJS = $(patsubst %.c, $(PATHO)%.o, $(SRCS))
TEST_OBJS   = $(patsubst %.c, $(PATHO)%.o, $(TEST_SRCS) $(LIB_SRCS) $(UNITY_SRC))
DEPS        = $(patsubst %.c, $(PATHD)%.d, $(SRCS))
RESULTS     = $(patsubst %.c, $(PATHR)%.txt, $(TEST_SRCS))

# --- Target Binaries ---
NORMAL_BIN  = a.out
DEBUG_BIN   = d.out
RELEASE_BIN = dfish

.PHONY: all full normal debug release test clean
.PRECIOUS: $(PATHB)test_%.out $(PATHD)%.d $(PATHO)%.o $(PATHR)%.txt

# Default
all: normal debug
full: normal debug release

# --- Build Rules ---

normal: $(NORMAL_BIN)
$(NORMAL_BIN): $(NORMAL_OBJS)
	$(LINK) -o $@ $(NORMAL_OBJS)

debug: CFLAGS += -g -O0 -DDEBUG
debug: $(DEBUG_BIN)
$(DEBUG_BIN): $(NORMAL_OBJS)
	$(LINK) -o $@ $(NORMAL_OBJS)

release: CFLAGS += -O2 -DNDEBUG
release: $(RELEASE_BIN)
$(RELEASE_BIN): $(NORMAL_OBJS)
	$(LINK) -o $@ $(NORMAL_OBJS)

# --- Testing Rules ---

test: $(RESULTS)
	@echo "\n-----------------------"
	@echo "IGNORES:"
	@echo "-----------------------"
	@grep -s IGNORE $(PATHR)*.txt || true
	@echo "-----------------------"
	@echo "FAILURES:"
	@echo "-----------------------"
	@grep -s FAIL $(PATHR)*.txt || true
	@echo "\nDONE"

# Run the test executable and pipe output to a results text file
$(PATHR)%.txt: $(PATHB)%.out
	@mkdir -p $(dir $@)
	-./$< > $@ 2>&1

# Link the test executable
$(PATHB)%.out: $(PATHO)%.o $(TEST_OBJS)
	@mkdir -p $(dir $@)
	$(LINK) -o $@ $^

# --- Compiling Object Files ---
$(PATHO)%.o: %.c
	@mkdir -p $(dir $@)
	$(COMPILE) $(CFLAGS) $< -o $@

# --- Dependency Tracking ---
$(PATHD)%.d: %.c
	@mkdir -p $(dir $@)
	$(DEPEND) $@ $<

# Include generated dependencies if they exist
-include $(DEPS)

# --- Cleanup ---
clean:
	rm -f $(NORMAL_BIN) $(DEBUG_BIN) $(RELEASE_BIN)
	rm -rf $(PATHB)
