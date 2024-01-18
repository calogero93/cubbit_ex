CC = g++
CFLAGS = -std=c++2a -Wall -Wextra
SRC_DIR = src
INC_DIR = include
BUILD_DIR = build
EXEC_DIR = exec
EXECUTABLE = $(EXEC_DIR)/horcrux
LDFLAGS = -luuid -lboost_system -lboost_thread -pthread -lssl -lcrypto

SRCS := $(wildcard $(SRC_DIR)/*.cpp)

OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

DEPS := $(OBJS:.o=.d)

all: $(EXECUTABLE)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -I$(INC_DIR) -MMD -c $< -o $@

$(EXECUTABLE): $(OBJS)
	@mkdir -p $(@D)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

clean:
	rm -rf $(BUILD_DIR) $(EXEC_DIR)

-include $(DEPS)
