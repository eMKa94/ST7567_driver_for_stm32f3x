TARGET_COMPILER			=	xc16-gcc -c
TARGET_COMPILER_FLAGS	=	-mcpu=24FJ256GA705   -mno-eds-warn  -g -omf=elf -DXPRJ_default=default  -legacy-libc    -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="C:/Program Files/Microchip/MPLABX/v5.50/packs/Microchip/PIC24F-GA-GB_DFP/1.4.141/xc16" 

TARGET_LINKER			=	xc16-gcc
TARGET_LINKER_FLAGS		=	 -mcpu=24FJ256GA705        -omf=elf -DXPRJ_default=default  -legacy-libc    -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,,--script=p24FJ256GA705.gld,--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,--report-mem,  -mdfp="C:/Program Files/Microchip/MPLABX/v5.50/packs/Microchip/PIC24F-GA-GB_DFP/1.4.141/xc16"

TARGET_BIN2HEX			=	xc16-bin2hex -a  -omf=elf   -mdfp="C:/Program Files/Microchip/MPLABX/v5.50/packs/Microchip/PIC24F-GA-GB_DFP/1.4.141/xc16"

TARGET_EXT				=	hex




#TARGET_COMPILER		=	xc16-gcc -c -mcpu=24FJ256GA705  -MP -MMD -MF "build/default/production/mcc_generated_files/traps.o.d"                -mno-eds-warn  -g -omf=elf -DXPRJ_default=default  -legacy-libc    -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="C:/Program Files/Microchip/MPLABX/v5.50/packs/Microchip/PIC24F-GA-GB_DFP/1.4.141/xc16"
#TARGET_LINKER		=	xc16-gcc -mcpu=24FJ256GA705        -omf=elf -DXPRJ_default=default  -legacy-libc    -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,,--script=p24FJ256GA705.gld,--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="dist/default/production/ITS_Firmware.X.production.map",--report-mem,--memorysummary,dist/default/production/memoryfile.xml  -mdfp="C:/Program Files/Microchip/MPLABX/v5.50/packs/Microchip/PIC24F-GA-GB_DFP/1.4.141/xc16" 

#TARGET_EXT			=	elf

