TARGET_COMPILER			=	arm-none-eabi-gcc -c
TARGET_COMPILER_FLAGS	=	-mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard -gdwarf-2 -Og -Wall -fdata-sections -ffunction-sections $(C_DEFS) -DUSE_FULL_LL_DRIVER

TARGET_LINKER			=	arm-none-eabi-gcc
TARGET_LINKER_FLAGS		=	-mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard -gdwarf-2 -Og -specs=nano.specs -TSTM32F373RCTX_FLASH_APP.ld -Lboard -Lstatic_lib -lc -lm -lnosys -Wl,-Map=build/target/$(PROJECT).map,--cref -Wl,--gc-sections -u _printf_float -Wl,--print-memory-usage


TARGET_BIN2HEX			=	arm-none-eabi-objcopy -Oihex

TARGET_EXT				=	hex