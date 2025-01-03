CC = clang++

FLAGS = -Wall -Wextra -std=c++20 -g

# Source files
SRCS = **.cpp
# Header files
HDRS = **.h
# Output binary
TARGET = penc
# Test files
TESTS = $(wildcard test*.pen)

# Default rule
all: $(TARGET)

# Link object files to create the executable
$(TARGET): $(SRCS) $(HDRS)
	$(CC) $(FLAGS) -o $(TARGET) $(SRCS)

# Clean build artifacts
clean:
	rm -f $(TARGET) *.o

# Rebuild
rebuild: clean all

# Run tests
test: rebuild
	@for test in $(TESTS); do \
		echo "Testing: $$test"; \
		if ! ./$(TARGET) $$test; then \
			echo "Test failed: $$test"; \
			exit 1; \
		fi; \
	done
	@echo "All tests passed"

quick: rebuild
	./$(TARGET) example.pen