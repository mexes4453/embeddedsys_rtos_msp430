#
PROJECT_DIR = /home/mexes/Documents/ES/projects/03_MSP430_rtos/02_rtos/lib

DIR_BUILD = $(PROJECT_DIR)/test/build
#SHELL = test

PATH_SRCS_TEST = $(PROJECT_DIR)/test/src
PATH_INC_TEST = $(PROJECT_DIR)/test/inc
PATH_SRCS_PROG = $(PROJECT_DIR)
PATH_INC_PROG = $(PROJECT_DIR)
DIR_OBJ_TEST = $(PROJECT_DIR)/test/obj
DIR_OBJ_PROG = $(PROJECT_DIR)/test/obj

# Source file listing ============
C_SRCS = \
$(PATH_SRCS_PROG)/xqueue/xqueue.c
$(info $(C_SRCS))


CPP_SRCS := \
$(PATH_SRCS_TEST)/main.cpp \
$(PATH_SRCS_TEST)/test_xqueue.cpp
$(info $(CPP_SRCS))
#----------------------------------



C_OBJS:=   $(C_SRCS:.c=.o)
OBJ_FILES_C := $(subst $(PATH_SRCS_PROG),$(DIR_OBJ_PROG),$(C_OBJS))

CPP_OBJS:= $(CPP_SRCS:.cpp=.o)
OBJ_FILES_CPP := $(subst $(PATH_SRCS_TEST),$(DIR_OBJ_TEST),$(CPP_OBJS))
$(info $(OBJ_FILES_CPP))
$(info $(OBJ_FILES_C))

FLAGS = -Werror -Wextra -Wall -g 
INCLUDES=  \
-I $(PATH_INC_TEST) \
-I $(PATH_INC_PROG)/xqueue


$(info )
# gcc libutils has been install using scoop via powershell
GCC = gcc
GPP = g++

NAME = test
TARGET=$(DIR_BUILD)/$(NAME)

#=============================================================================

TARGET : all
all : $(TARGET)
$(TARGET) : $(OBJ_FILES_C) $(OBJ_FILES_CPP)
	@echo "\033[1;33mCompiling Executables: $(NAME) \033[0m"
	@mkdir -p $(dir $@)
	@echo ""
	@echo Compiling Target: $(TARGET)
	$(GPP) $^ $(FLAGS) $(INCLUDES) -o $@
	@echo ""
	@echo ""


# obj files output for c file
$(DIR_OBJ_PROG)/%.o : $(PATH_SRCS_PROG)/%.c
	@echo "\033[1;33mCompiling obj files from c files: \033[0m" $^
	@mkdir -p $(dir $@)
	@echo $($^)
	@mkdir -p $(dir $@)
	@echo ""
#	@echo Compiling Obj File... $^
	$(GCC) -c $^ $(FLAGS) $(INCLUDES) -o $@
	@echo ""


# obj files output for cpp files
$(DIR_OBJ_TEST)/%.o : $(PATH_SRCS_TEST)/%.cpp
	@echo "\033[1;33mCompiling obj files from cpp files: \033[0m" $^
	@mkdir -p $(dir $@)
	@echo ""
#	@echo "compiling Obj File... " $^
	$(GPP) -c $^ $(FLAGS) $(INCLUDES) -o $@
	@echo ""
	@echo ""

run:
#	Execute test
	@$(TARGET) -s


# remove all object files
fclean:
	@yes | rm -rf $(OBJ_FILES_C) $(OBJ_FILES_CPP)

# remove final target files
clean: fclean
	@yes | rm -rf $(TARGET)

# recompile everything
re: clean all


.PHONY : all fclean clean re run