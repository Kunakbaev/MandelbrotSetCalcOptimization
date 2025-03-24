LIBS 						:= -lsfml-graphics -lsfml-window -lsfml-system

CC 							:= g++
# CFLAGS 						:= -O3 -D _DEBUG -lm -ggdb3 -std=c++17 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -pie -fPIE -Werror=vla
CFLAGS						:= -Ofast -D _DEBUG

MAIN_TARGET_NAME			:= mandlebrot
BUILD_DIR_PATH   			:= ./building
BUILD_DIR_TARGET 			:= createBuildDir
SRC_DIR						:= .
SFML_PATH					:= -I/libs/sfml/include

SRC 						:= $(SRC_DIR)/main.cpp
OBJ 						:= $(patsubst %.cpp, $(BUILD_DIR_PATH)/%.o, $(notdir ${SRC}))




.PHONY: $(BUILD_DIR_TARGET) compile run compileAndRun clean

compile: $(OBJ)
	$(CC) $^ -o $(BUILD_DIR_PATH)/$(MAIN_TARGET_NAME) $(LIBS) $(CFLAGS)

run:
	$(BUILD_DIR_PATH)/$(MAIN_TARGET_NAME)

# WARNING: cleans build directory every time
compileAndRun: clean compile run

$(BUILD_DIR_PATH)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR_TARGET)
	$(CC) -c $< $(CFLAGS) -o $@

$(BUILD_DIR_TARGET):
	mkdir -p $(BUILD_DIR_PATH)

clean:
	rm -rf $(BUILD_DIR_PATH)
