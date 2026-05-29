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
# Find all release source files
SRCS = $(shell find src -name '*.c')
# Exclude main.c for the test library
LIB_SRCS = $(filter-out src/main.c, $(SRCS))
# Test files
TEST_SRCS = $(wildcard test/*.c)

# --- Object & Dependency Mapping ---
# Convert src/engine/board.c -> bin/objs/board.o (flattened for simplicity)
NORMAL_OBJS = $(addprefix $(PATHO), $(notdir $(SRCS:.c=.o)))
TEST_OBJS   = $(addprefix $(PATHO), $(notdir $(TEST_SRCS:.c=.o))) $(addprefix $(PATHO), $(notdir $(LIB_SRCS:.c=.o))) $(PATHO)unity.o

# Convert test files to result files: test/test_board.c -> bin/results/test_board.txt
RESULTS = $(patsubst test/%.c, $(PATHR)%.txt, $(TEST_SRCS))

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
$(NORMAL_BIN): $(BUILD_PATHS) $(NORMAL_OBJS)
	$(LINK) -o $@ $(NORMAL_OBJS)

debug: CFLAGS += -g -O0 -DDEBUG
debug: $(DEBUG_BIN)
$(DEBUG_BIN): $(BUILD_PATHS) $(NORMAL_OBJS)
	$(LINK) -o $@ $(NORMAL_OBJS)

release: CFLAGS += -O2 -DNDEBUG
release: $(RELEASE_BIN)
$(RELEASE_BIN): $(BUILD_PATHS) $(NORMAL_OBJS)
	$(LINK) -o $@ $(NORMAL_OBJS)

# --- Testing Rules ---

test: $(BUILD_PATHS) $(RESULTS)
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
	-./$< > $@ 2>&1

# Link the test executable
$(PATHB)%.out: $(PATHO)%.o $(TEST_OBJS)
	$(LINK) -o $@ $^

# --- Compiling Object Files (VPATH helps Make find sources in subdirectories) ---
VPATH = src:test:unity/src

$(PATHO)%.o: %.c
	$(COMPILE) $(CFLAGS) $< -o $@

# --- Dependency Tracking ---
$(PATHD)%.d: %.c
	$(DEPEND) $@ $<

# Include generated dependencies if they exist
-include $(addprefix $(PATHD), $(notdir $(SRCS:.c=.d)))

# --- Directory Creation ---
$(BUILD_PATHS):
	mkdir -p $@

# --- Cleanup ---
clean:
	rm -f $(NORMAL_BIN) $(DEBUG_BIN) $(RELEASE_BIN)
	rm -rf $(PATHB)
