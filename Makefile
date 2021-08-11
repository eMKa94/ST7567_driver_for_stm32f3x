#	#	#	MAKEFILE	#	#	#
#								#
#		eMKA elektronika		#
#		Maciej Kowalski			#
#								#
#	Embedded Project Makefile	#
#								#	
#	#	#	#	#	#	#	#	#

.SECONDARY:
PROJECT		=	IPA

TARGET_TOOLCHAIN	=	STM32F373RC_TOOLCHAIN
TEST_TOOLCHAIN		=	DEFAULT_TEST_TOOLCHAIN

include makefile_tools/$(TARGET_TOOLCHAIN).mk
include makefile_tools/$(TEST_TOOLCHAIN).mk

#PROJECT PATHS

#GENERAL
SOURCE_PATH				=	source
INCLUDE_PATH			=	include
BUILD_PATH				=	build
BOARD_PATH				=	board
STM32F3XX_LL_DRV_PATH	=	$(BOARD_PATH)/stm32f3xx_ll_drivers
MCU_FILES_PATH			=	$(BOARD_PATH)/mcu_files
STATIC_LIB_PATH			=	static_lib

#TARGET PATHS
TARGET_BUILD_PATH		=	$(BUILD_PATH)/target
TARGET_OBJECTS_PATH		=	$(TARGET_BUILD_PATH)/objects
TARGET_DEPEND_PATH		=	$(TARGET_BUILD_PATH)/depend

#TESTS PATHS
TESTS_SOURCE_PATH		=	tests
TEST_BUILD_PATH			=	$(BUILD_PATH)/tests
TEST_OBJECTS_PATH		=	$(TEST_BUILD_PATH)/objects
TEST_LOG_PATH			=	$(TEST_BUILD_PATH)/log

#INCLUDE FLAGS
INCLUDE_FLAGS			=	-I. -I$(INCLUDE_PATH) -I$(TESTS_SOURCE_PATH) -I$(BOARD_PATH) -I$(STATIC_LIB_PATH) -I$(STM32F3XX_LL_DRV_PATH) -I$(MCU_FILES_PATH)

#FILES
PROJECT_TARGET			=	$(TARGET_BUILD_PATH)/$(PROJECT).$(TARGET_EXT)

TARGET_SOURCE_FILES		=	$(wildcard $(SOURCE_PATH)/*.c )
TARGET_OBJECTS			=	$(patsubst $(SOURCE_PATH)/%.c, $(TARGET_OBJECTS_PATH)/%.o, $(wildcard $(SOURCE_PATH)/*.c ))

BOARD_SOURCE_FILES		=	$(wildcard $(BOARD_PATH)/*.c )
BOARD_OBJECTS			=	$(patsubst $(BOARD_PATH)/%.c, $(TARGET_OBJECTS_PATH)/%.o, $(wildcard $(BOARD_PATH)/*.c ))
BOARD_OBJECTS			+=	$(patsubst $(STM32F3XX_LL_DRV_PATH)/%.c, $(TARGET_OBJECTS_PATH)/%.o, $(wildcard $(STM32F3XX_LL_DRV_PATH)/*.c ))
BOARD_OBJECTS			+=	$(patsubst $(MCU_FILES_PATH)/%.c, $(TARGET_OBJECTS_PATH)/%.o, $(wildcard $(MCU_FILES_PATH)/*.c ))
BOARD_OBJECTS			+=	$(patsubst $(BOARD_PATH)/%.s, $(TARGET_OBJECTS_PATH)/%.o, $(wildcard $(BOARD_PATH)/*.s ))

STATIC_LIB_FILES		=	$(wildcard $(STATIC_LIB_PATH)/*.a)
STATIC_LIBRARIES		=	$(patsubst	$(STATIC_LIB_PATH)/lib%.a, %, $(STATIC_LIB_FILES))

TEST_SOURCE_FILES		=	$(wildcard	$(TESTS_SOURCE_PATH)/test_*.c)

TEST_RUNNERS			=	$(patsubst	$(TESTS_SOURCE_PATH)/test_%.c, $(TEST_BUILD_PATH)/test_%.$(TEST_EXT), $(TEST_SOURCE_FILES))

#Extra linker flags
LINKER_FLAGS			=	-L $(STATIC_LIB_PATH) $(foreach lib, $(STATIC_LIBRARIES), -l $(lib))

.PHONY: build_project
build_project:	PROJECT_TEST	$(PROJECT_TARGET)	

$(TARGET_BUILD_PATH)/$(PROJECT).exe:	$(TARGET_OBJECTS) $(BOARD_OBJECTS)
	$(TARGET_LINKER) -o $@ $^ $(TARGET_LINKER_FLAGS) $(LINKER_FLAGS)

$(TARGET_BUILD_PATH)/$(PROJECT).elf:	$(TARGET_OBJECTS)	$(BOARD_OBJECTS)
	@echo linking target to $@
	@$(TARGET_LINKER) -o $@ $^ $(TARGET_LINKER_FLAGS) $(LINKER_FLAGS)

$(TARGET_BUILD_PATH)/$(PROJECT).hex:	$(TARGET_BUILD_PATH)/$(PROJECT).elf
	@echo Converting $< to $@
	@$(TARGET_BIN2HEX) $< $@
	@echo ...........................................................................
	@echo ......@@.@@....................................@@.@@...................@@..
	@echo .....@..@..@..................................@..@..@..................@@..
	@echo ....@...@..@....@@@.....@@@.@@@...@..@.......@...@..@....@@@.....@@@...@@..
	@echo ...@.......@...@...@...@...@...@...@........@.......@...@...@...@..........
	@echo ..@........@....@@@.@..@.......@..@........@........@....@@@.....@@@...@@..
	@echo ...........................................................................

#TESTING RULES

.PHONY:	PROJECT_TEST
PROJECT_TEST:	$(TEST_RUNNERS)

.PHONY: run

#TEST RUNNERS

$(TEST_BUILD_PATH)/test_%.$(TEST_EXT):	$(TEST_OBJECTS_PATH)/test_%.o	$(TEST_OBJECTS_PATH)/unity.o 	$(TEST_OBJECTS_PATH)/%.o
	@echo Linking test	$(patsubst $(TEST_BUILD_PATH)/%, %, $@)	:
	$(TEST_LINKER) -o $@	$^
	$@

#	TEST OBJECTS	RULES

$(TEST_OBJECTS_PATH)/%.o::	$(SOURCE_PATH)/%.c $(INCLUDE_PATH)/%.h
	@echo Compiling for tests:
	$(TEST_COMPILER) -o $@ $(TEST_CFLAGS) $(INCLUDE_FLAGS)	$<

$(TEST_OBJECTS_PATH)/%.o::	$(BOARD_PATH)/%.c $(BOARD_PATH)/%.h
	@echo Compiling for tests:
	$(TEST_COMPILER) -o $@ $(TEST_CFLAGS) $(INCLUDE_FLAGS)	$<	

$(TEST_OBJECTS_PATH)/test_%.o::	$(TESTS_SOURCE_PATH)/test_%.c
	@echo Compiling for tests:
	$(TEST_COMPILER) -o $@ $(TEST_CFLAGS)	$(INCLUDE_FLAGS)	$<

$(TEST_OBJECTS_PATH)/unity.o:	$(TESTS_SOURCE_PATH)/unity.c
	@echo Compiling for tests:
	$(TEST_COMPILER) -o $@ $(TEST_CFLAGS)	$(INCLUDE_FLAGS)	$<

#	TARGET OBJECTS RULES

$(TARGET_OBJECTS_PATH)/%.o::	$(SOURCE_PATH)/%.c $(INCLUDE_PATH)/%.h
	@echo Compiling $< to object: $@
	@$(TARGET_COMPILER) $< $(INCLUDE_FLAGS) $(TARGET_COMPILER_FLAGS) -o $@

$(TARGET_OBJECTS_PATH)/%.o::	$(SOURCE_PATH)/%.c
	@echo Compiling $< to object: $@
	@$(TARGET_COMPILER) $< $(INCLUDE_FLAGS) $(TARGET_COMPILER_FLAGS) -o $@

$(TARGET_OBJECTS_PATH)/%.o::	$(SOURCE_PATH)/%.s
	@echo Compiling $< to object: $@
	@$(TARGET_COMPILER) $< $(INCLUDE_FLAGS) $(TARGET_COMPILER_FLAGS) -o $@

$(TARGET_OBJECTS_PATH)/%.o::	$(BOARD_PATH)/%.c $(BOARD_PATH)/%.h
	@echo Compiling $< to object: $@
	@$(TARGET_COMPILER) $< $(INCLUDE_FLAGS) $(TARGET_COMPILER_FLAGS) -o $@

$(TARGET_OBJECTS_PATH)/%.o::	$(BOARD_PATH)/%.c
	@echo Compiling $< to object: $@
	@$(TARGET_COMPILER) $< $(INCLUDE_FLAGS) $(TARGET_COMPILER_FLAGS) -o $@

$(TARGET_OBJECTS_PATH)/%.o::	$(STM32F3XX_LL_DRV_PATH)/%.c
	@echo Compiling $< to object: $@
	@$(TARGET_COMPILER) $< $(INCLUDE_FLAGS) $(TARGET_COMPILER_FLAGS) -o $@

$(TARGET_OBJECTS_PATH)/%.o::	$(MCU_FILES_PATH)/%.c
	@echo Compiling $< to object: $@
	@$(TARGET_COMPILER) $< $(INCLUDE_FLAGS) $(TARGET_COMPILER_FLAGS) -o $@

$(TARGET_OBJECTS_PATH)/%.o::	$(BOARD_PATH)/%.s
	@echo Compiling $< to object: $@
	@$(TARGET_COMPILER) $< $(INCLUDE_FLAGS) $(TARGET_COMPILER_FLAGS) -o $@

run:	PROJECT_TARGET
.PHONY: PROJECT_TEMPLATE

PROJECT_TEMPLATE:	$(SOURCE_PATH)	$(BUILD_PATH)	$(INCLUDE_PATH)	$(TESTS_PATH) $(TARGET_OBJECTS_PATH)

$(SOURCE_PATH):
	mkdir $@

$(BUILD_PATH):
	mkdir $@

$(INCLUDE_PATH):
	mkdir $@

$(TESTS_PATH):
	mkdir $@

$(TARGET_OBJECTS_PATH):
	mkdir $@

$(TEST_LOG_PATH):
	mkdir $@


.PHONY: info
info:
	@echo Building project $(PROJECT)

DEL_CMD	=	rm -rf

.PHONY: clean
clean:	clean_target	clean_test

.PHONY: clean_target
clean_target:
	$(DEL_CMD) $(TARGET_BUILD_PATH)/*
	$(DEL_CMD) $(TARGET_OBJECTS_PATH)/*

.PHONY: clean_test
clean_test:
	$(DEL_CMD) $(TEST_BUILD_PATH)/*
	$(DEL_CMD) $(TEST_OBJECTS_PATH)/*

.PHONY: list_objects
list_objects:

	@echo TARGET SOURCE FILES:
	@echo $(TARGET_SOURCE_FILES)

	@echo TARGET OBJECTS:
	@echo $(TARGET_OBJECTS)

	@echo BOARD SOURCE FILES:
	@echo $(BOARD_SOURCE_FILES)

	@echo BOARD OBJECTS:
	@echo $(BOARD_OBJECTS)

flash:	$(TARGET_BUILD_PATH)/$(PROJECT).$(TARGET_EXT)
	openocd.exe -f interface/stlink.cfg -f target/stm32f3x.cfg -c "program $< verify reset exit"
