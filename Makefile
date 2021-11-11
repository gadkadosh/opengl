CXX=clang++
CXXFLAGS=-g
INCLUDES=-I./vendor/GLEW/include -I./vendor/GLFW/include
LIBS=-L./vendor/GLEW/lib -lglew -L./vendor/GLFW -lglfw3
FRAMEWORKS=-framework Cocoa -framework OpenGL -framework IOKit

SRC_DIR=src
BUILD_DIR=build
SRCS=$(shell find $(SRC_DIR) -name *.cpp)
OBJS=$(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

$(BUILD_DIR)/opengl: $(OBJS)
	$(CXX) $(CXXFLAGS) $(FRAMEWORKS) $(LIBS) $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -r $(BUILD_DIR)
