LIBS 						:= -lsfml-graphics -lsfml-window -lsfml-system

CC 							:= g++
CFLAGS 						:= -D NDEBUG -D _DEBUG -lm -ggdb3 -std=c++17 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -pie -fPIE -Werror=vla
# CFLAGS						:= -D _DEBUG
# CFLAGS += -fsanitize=address

MY_LOG_LIB_NAME				:= my_loglib
MAIN_TARGET_NAME			:= mandlebrot
TEST_PERF_TARGET			:= testPerf
BUILD_DIR_PATH   			:= ./building
BUILD_DIR_TARGET 			:= createBuildDir
SRC_DIR						:= .
TEST_PERF_DIR				:= ./testPerfomance
SFML_PATH					:= -I/libs/sfml/include
ERRORS_HANDLER_DIR			:= errorsHandler
GRAPHICAL_INTERFACE_DIR		:= graphicalInterface
CALC_POINTS_INFO_DIR		:= calcPointsInfo


SRC_COMMON			 		:= $(ERRORS_HANDLER_DIR)/errorsHandler.cpp						\
							   $(CALC_POINTS_INFO_DIR)/calcPointsInfo.cpp					\
							   $(CALC_POINTS_INFO_DIR)/calcPointsInfoFloat.cpp				\
							   $(CALC_POINTS_INFO_DIR)/calcInfoHighResolution.cpp			\
							   $(CALC_POINTS_INFO_DIR)/calcPointsInfoArrays.cpp  			\
							   $(CALC_POINTS_INFO_DIR)/calcInfoOptimizedWithIntrinsics.cpp  \


SRC_DRAW_MANDELBROT 		:= $(SRC_COMMON)												\
							   $(SRC_DIR)/main.cpp											\
							   $(GRAPHICAL_INTERFACE_DIR)/graphicalInterface.cpp			\
							   $(GRAPHICAL_INTERFACE_DIR)/mandelbrotColoring.cpp			\

SRC_TEST_PERFOMANCE			:= $(SRC_COMMON)												\
							   $(TEST_PERF_DIR)/findPerfomanceOfCalcPointsInfo.cpp				\

OBJ_DRAW_MANDLEBROT 		:= $(patsubst %.cpp, $(BUILD_DIR_PATH)/%.o, $(notdir ${SRC_DRAW_MANDELBROT}))
OBJ_TEST_PERFOMANCE 		:= $(patsubst %.cpp, $(BUILD_DIR_PATH)/%.o, $(notdir ${SRC_TEST_PERFOMANCE}))



.PHONY: $(BUILD_DIR_TARGET) compile run compileAndRun clean compileTestPerf compileAndRunTestPerf

compile: $(OBJ_DRAW_MANDLEBROT)
	$(CC) $^ -o $(BUILD_DIR_PATH)/$(MAIN_TARGET_NAME) -l$(MY_LOG_LIB_NAME) $(LIBS) $(CFLAGS)

run:
	$(BUILD_DIR_PATH)/$(MAIN_TARGET_NAME)

compileTestPerf: $(OBJ_TEST_PERFOMANCE)
	$(CC) $^ -o $(BUILD_DIR_PATH)/$(TEST_PERF_TARGET) -l$(MY_LOG_LIB_NAME) $(CFLAGS)

runTestPerf:
	$(BUILD_DIR_PATH)/$(TEST_PERF_TARGET)

# WARNING: cleans build directory every time
compileAndRun: compile run

compileAndRunTestPerf: compileTestPerf runTestPerf

$(BUILD_DIR_PATH)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR_TARGET)
	$(CC) -c $< $(CFLAGS) -o $@

$(BUILD_DIR_PATH)/%.o: $(ERRORS_HANDLER_DIR)/%.cpp | $(BUILD_DIR_TARGET)
	$(CC) -c $< $(CFLAGS) -o $@

$(BUILD_DIR_PATH)/%.o: $(CALC_POINTS_INFO_DIR)/%.cpp | $(BUILD_DIR_TARGET)
	$(CC) -c $< $(CFLAGS) -o $@

$(BUILD_DIR_PATH)/%.o: $(GRAPHICAL_INTERFACE_DIR)/%.cpp | $(BUILD_DIR_TARGET)
	$(CC) -c $< $(CFLAGS) -o $@

$(BUILD_DIR_PATH)/%.o: $(TEST_PERF_DIR)/%.cpp | $(BUILD_DIR_TARGET)
	$(CC) -c $< $(CFLAGS) -o $@

$(BUILD_DIR_TARGET):
	mkdir -p $(BUILD_DIR_PATH)

clean:
	rm -rf $(BUILD_DIR_PATH)
