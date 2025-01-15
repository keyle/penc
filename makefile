CC = clang++

FLAGS = -Wall -Wextra -O0 -g -std=c++20
RELEASE_FLAGS = -Wall -Wextra -std=c++20 -O2

# Source files
SRCS = comp/**.cpp
# Header files
HDRS = comp/**.h
# Output binary
TARGET = penc
# Test files
TESTS = $(wildcard tests/*.pen)

# Default rule
all: $(TARGET)

# Link object files to create the executable
$(TARGET): $(SRCS) $(HDRS)
	@$(CC) $(FLAGS) -o $(TARGET) $(SRCS)

# Clean build artifacts
clean:
	@rm -rf $(TARGET) *.o *.dSYM

# Rebuild
rebuild: clean all

# Run tests
tests: rebuild
	@for test in $(TESTS); do \
		echo "Testing: $$test"; \
		if ! ./$(TARGET) $$test; then \
			echo "Test failed: $$test"; \
			exit 1; \
		fi; \
	done
	@echo "All tests passed"

# Temp debug test
quick: all
	./$(TARGET) -d -b tests/first_program.pen

# Release build
release: FLAGS := $(RELEASE_FLAGS)
release: rebuild
