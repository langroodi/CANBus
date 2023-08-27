INCLUDE(CMakeForceCompiler)

# CROSS COMPILER SETTING
SET(CMAKE_SYSTEM_NAME Generic)
CMAKE_MINIMUM_REQUIRED (VERSION 2.6)

# THE VERSION NUMBER
SET (Tutorial_VERSION_MAJOR 1)
SET (Tutorial_VERSION_MINOR 0)

# ENABLE ASM
ENABLE_LANGUAGE(ASM)

SET(CMAKE_STATIC_LIBRARY_PREFIX)
SET(CMAKE_STATIC_LIBRARY_SUFFIX)

SET(CMAKE_EXECUTABLE_LIBRARY_PREFIX)
SET(CMAKE_EXECUTABLE_LIBRARY_SUFFIX)

# RV32M1 SDK DIRECTORY
if (NOT DEFINED ENV{RISCV32SDK_DIR})
    set(FATAL_ERROR "RV32M1 SDK directory environment variable has not been set.")
endif()

# CURRENT DIRECTORY
SET(ProjDirPath ${CMAKE_CURRENT_SOURCE_DIR})

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -DDEBUG")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -D__STARTUP_CLEAR_BSS")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -g")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -march=rv32imcxpulpv2")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -Wall")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -fno-common")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -ffunction-sections")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -fdata-sections")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -ffreestanding")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -fno-builtin")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -std=gnu99")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -D__STARTUP_CLEAR_BSS")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -march=rv32imcxpulpv2")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -Wall")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -fno-common")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -ffunction-sections")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -fdata-sections")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -ffreestanding")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -fno-builtin")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -std=gnu99")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -D_DEBUG=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DCPU_RV32M1_ri5cy")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DUSB_STACK_FREERTOS")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DFSL_RTOS_FREE_RTOS")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -march=rv32imcxpulpv2")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -MMD")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -MP")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fno-common")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -ffunction-sections")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fdata-sections")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -ffreestanding")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fno-builtin")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -std=gnu99")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -D_DEBUG=0")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DCPU_RV32M1_ri5cy")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DUSB_STACK_FREERTOS")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DFSL_RTOS_FREE_RTOS")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O2")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fno-store-merging")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -march=rv32imcxpulpv2")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -MMD")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -MP")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wall")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fno-common")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -ffunction-sections")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fdata-sections")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -ffreestanding")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fno-builtin")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -std=gnu99")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -march=rv32imcxpulpv2")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -MMD")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -MP")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-common")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ffunction-sections")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fdata-sections")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ffreestanding")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-builtin")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-rtti")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-exceptions")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fno-store-merging")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -march=rv32imcxpulpv2")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -MMD")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -MP")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fno-common")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -ffunction-sections")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fdata-sections")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -ffreestanding")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fno-builtin")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fno-rtti")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fno-exceptions")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -g")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -march=rv32imcxpulpv2")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Wall")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fno-common")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -ffunction-sections")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fdata-sections")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -ffreestanding")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fno-builtin")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --gc-sections")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -static")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -z")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} muldefs")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -nostdlib")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -march=rv32imcxpulpv2")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Wall")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -fno-common")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -ffunction-sections")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -fdata-sections")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -ffreestanding")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -fno-builtin")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} --gc-sections")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -static")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -z")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} muldefs")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -nostdlib")

include_directories($ENV{RISCV32SDK_DIR}/boards/rv32m1_vega)

include_directories(${ProjDirPath}/..)

include_directories($ENV{RISCV32SDK_DIR}/rtos/freertos_9.0.0/Source/portable/GCC/RISCY)

include_directories($ENV{RISCV32SDK_DIR}/rtos/freertos_9.0.0/Source/include)

include_directories($ENV{RISCV32SDK_DIR}/RISCV)

include_directories($ENV{RISCV32SDK_DIR}/devices)

include_directories(${ProjDirPath}/../..)

include_directories($ENV{RISCV32SDK_DIR}/rtos/freertos_9.0.0/Source)

include_directories($ENV{RISCV32SDK_DIR}/middleware/usb_1.6.3)

include_directories($ENV{RISCV32SDK_DIR}/middleware/usb_1.6.3/osa)

include_directories($ENV{RISCV32SDK_DIR}/middleware/usb_1.6.3/include)

include_directories($ENV{RISCV32SDK_DIR}/middleware/usb_1.6.3/device)

include_directories($ENV{RISCV32SDK_DIR}/CMSIS/Include)

include_directories($ENV{RISCV32SDK_DIR}/devices/RV32M1/drivers)

include_directories($ENV{RISCV32SDK_DIR}/devices/RV32M1/utilities)

include_directories($ENV{RISCV32SDK_DIR}/devices/RV32M1/utilities/io)

include_directories($ENV{RISCV32SDK_DIR}/devices/RV32M1/utilities/str)

include_directories($ENV{RISCV32SDK_DIR}/devices/RV32M1/utilities/log)

include_directories($ENV{RISCV32SDK_DIR}/devices/RV32M1)

add_executable(open_can_sniffer.elf 
"$ENV{RISCV32SDK_DIR}/rtos/freertos_9.0.0/Source/portable/GCC/RISCY/port.c"
"$ENV{RISCV32SDK_DIR}/rtos/freertos_9.0.0/Source/portable/GCC/RISCY/portASM.S"
"$ENV{RISCV32SDK_DIR}/rtos/freertos_9.0.0/Source/portable/GCC/RISCY/fsl_tickless_generic.h"
"$ENV{RISCV32SDK_DIR}/rtos/freertos_9.0.0/Source/portable/GCC/RISCY/portmacro.h"
"$ENV{RISCV32SDK_DIR}/devices/RV32M1/gcc/startup_RV32M1_ri5cy.S"
"$ENV{RISCV32SDK_DIR}/middleware/usb_1.6.3/device/usb_device_khci.c"
"$ENV{RISCV32SDK_DIR}/middleware/usb_1.6.3/device/usb_device_khci.h"
"$ENV{RISCV32SDK_DIR}/middleware/usb_1.6.3/include/usb_khci.h"
"$ENV{RISCV32SDK_DIR}/devices/RV32M1/drivers/fsl_lpuart.h"
"$ENV{RISCV32SDK_DIR}/devices/RV32M1/drivers/fsl_lpuart.c"
"$ENV{RISCV32SDK_DIR}/devices/RV32M1/drivers/fsl_intmux.h"
"$ENV{RISCV32SDK_DIR}/devices/RV32M1/drivers/fsl_intmux.c"
"$ENV{RISCV32SDK_DIR}/devices/RV32M1/drivers/fsl_lpspi.h"
"$ENV{RISCV32SDK_DIR}/devices/RV32M1/drivers/fsl_lpspi.c"
"$ENV{RISCV32SDK_DIR}/devices/RV32M1/drivers/fsl_lpspi_freertos.h"
"$ENV{RISCV32SDK_DIR}/devices/RV32M1/drivers/fsl_lpspi_freertos.c"
"$ENV{RISCV32SDK_DIR}/middleware/usb_1.6.3/osa/usb_osa.h"
"$ENV{RISCV32SDK_DIR}/middleware/usb_1.6.3/osa/usb_osa_freertos.h"
"$ENV{RISCV32SDK_DIR}/middleware/usb_1.6.3/osa/usb_osa_freertos.c"
"$ENV{RISCV32SDK_DIR}/rtos/freertos_9.0.0/Source/portable/MemMang/heap_4.c"
"${ProjDirPath}/../usb_device_ch9.c"
"${ProjDirPath}/../usb_device_ch9.h"
"${ProjDirPath}/../usb_device_class.c"
"${ProjDirPath}/../usb_device_class.h"
"${ProjDirPath}/../usb_device_cdc_acm.c"
"${ProjDirPath}/../usb_device_cdc_acm.h"
"${ProjDirPath}/../usb_device_descriptor.c"
"${ProjDirPath}/../usb_device_descriptor.h"
"${ProjDirPath}/../open_can_sniffer.c"
"${ProjDirPath}/../usb_device_config.h"
"${ProjDirPath}/../FreeRTOSConfig.h"
"${ProjDirPath}/../riscveclipse/can_frame.c"
"${ProjDirPath}/../riscveclipse/can_frame.h"
"${ProjDirPath}/../riscveclipse/mcp2515.c"
"${ProjDirPath}/../riscveclipse/mcp2515.h"
"${ProjDirPath}/../riscveclipse/spi_helper.c"
"${ProjDirPath}/../riscveclipse/spi_helper.h"
"${ProjDirPath}/../riscveclipse/usb_helper.c"
"${ProjDirPath}/../riscveclipse/usb_helper.h"
"${ProjDirPath}/../riscveclipse/usb_packet.c"
"${ProjDirPath}/../riscveclipse/usb_packet.h"
"$ENV{RISCV32SDK_DIR}/rtos/freertos_9.0.0/Source/include/croutine.h"
"$ENV{RISCV32SDK_DIR}/rtos/freertos_9.0.0/Source/include/event_groups.h"
"$ENV{RISCV32SDK_DIR}/rtos/freertos_9.0.0/Source/include/FreeRTOS.h"
"$ENV{RISCV32SDK_DIR}/rtos/freertos_9.0.0/Source/include/list.h"
"$ENV{RISCV32SDK_DIR}/rtos/freertos_9.0.0/Source/include/mpu_wrappers.h"
"$ENV{RISCV32SDK_DIR}/rtos/freertos_9.0.0/Source/include/portable.h"
"$ENV{RISCV32SDK_DIR}/rtos/freertos_9.0.0/Source/include/projdefs.h"
"$ENV{RISCV32SDK_DIR}/rtos/freertos_9.0.0/Source/include/queue.h"
"$ENV{RISCV32SDK_DIR}/rtos/freertos_9.0.0/Source/include/semphr.h"
"$ENV{RISCV32SDK_DIR}/rtos/freertos_9.0.0/Source/include/StackMacros.h"
"$ENV{RISCV32SDK_DIR}/rtos/freertos_9.0.0/Source/include/task.h"
"$ENV{RISCV32SDK_DIR}/rtos/freertos_9.0.0/Source/include/timers.h"
"$ENV{RISCV32SDK_DIR}/rtos/freertos_9.0.0/Source/include/deprecated_definitions.h"
"$ENV{RISCV32SDK_DIR}/rtos/freertos_9.0.0/Source/include/freertos_tasks_c_additions.h"
"$ENV{RISCV32SDK_DIR}/rtos/freertos_9.0.0/Source/croutine.c"
"$ENV{RISCV32SDK_DIR}/rtos/freertos_9.0.0/Source/list.c"
"$ENV{RISCV32SDK_DIR}/rtos/freertos_9.0.0/Source/queue.c"
"$ENV{RISCV32SDK_DIR}/rtos/freertos_9.0.0/Source/tasks.c"
"$ENV{RISCV32SDK_DIR}/rtos/freertos_9.0.0/Source/timers.c"
"$ENV{RISCV32SDK_DIR}/rtos/freertos_9.0.0/Source/event_groups.c"
"$ENV{RISCV32SDK_DIR}/devices/RV32M1/system_RV32M1_ri5cy.c"
"$ENV{RISCV32SDK_DIR}/devices/RV32M1/system_RV32M1_ri5cy.h"
"$ENV{RISCV32SDK_DIR}/devices/RV32M1/utilities/fsl_debug_console.c"
"$ENV{RISCV32SDK_DIR}/devices/RV32M1/utilities/fsl_debug_console.h"
"$ENV{RISCV32SDK_DIR}/devices/RV32M1/utilities/fsl_debug_console_conf.h"
"$ENV{RISCV32SDK_DIR}/devices/RV32M1/utilities/fsl_io.c"
"$ENV{RISCV32SDK_DIR}/devices/RV32M1/utilities/fsl_io.h"
"$ENV{RISCV32SDK_DIR}/devices/RV32M1/utilities/fsl_log.c"
"$ENV{RISCV32SDK_DIR}/devices/RV32M1/utilities/fsl_log.h"
"$ENV{RISCV32SDK_DIR}/devices/RV32M1/utilities/fsl_str.c"
"$ENV{RISCV32SDK_DIR}/devices/RV32M1/utilities/fsl_str.h"
"$ENV{RISCV32SDK_DIR}/middleware/usb_1.6.3/include/usb.h"
"$ENV{RISCV32SDK_DIR}/middleware/usb_1.6.3/include/usb_misc.h"
"$ENV{RISCV32SDK_DIR}/middleware/usb_1.6.3/include/usb_spec.h"
"$ENV{RISCV32SDK_DIR}/middleware/usb_1.6.3/device/usb_device.h"
"$ENV{RISCV32SDK_DIR}/middleware/usb_1.6.3/device/usb_device_dci.c"
"$ENV{RISCV32SDK_DIR}/middleware/usb_1.6.3/device/usb_device_dci.h"
"$ENV{RISCV32SDK_DIR}/devices/RV32M1/drivers/fsl_port.h"
"$ENV{RISCV32SDK_DIR}/devices/RV32M1/drivers/fsl_sim.h"
"$ENV{RISCV32SDK_DIR}/devices/RV32M1/drivers/fsl_sim.c"
"$ENV{RISCV32SDK_DIR}/devices/RV32M1/drivers/fsl_clock.h"
"$ENV{RISCV32SDK_DIR}/devices/RV32M1/drivers/fsl_clock.c"
"$ENV{RISCV32SDK_DIR}/devices/RV32M1/drivers/fsl_common.h"
"$ENV{RISCV32SDK_DIR}/devices/RV32M1/drivers/fsl_common.c"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"$ENV{RISCV32SDK_DIR}/devices/RV32M1/drivers/fsl_gpio.h"
"$ENV{RISCV32SDK_DIR}/devices/RV32M1/drivers/fsl_gpio.c"
"$ENV{RISCV32SDK_DIR}/devices/RV32M1/drivers/fsl_msmc.h"
"$ENV{RISCV32SDK_DIR}/devices/RV32M1/drivers/fsl_msmc.c"
)

set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -T${ProjDirPath}/RV32M1_ri5cy_flash.ld -static")

set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -T${ProjDirPath}/RV32M1_ri5cy_flash.ld -static")

TARGET_LINK_LIBRARIES(open_can_sniffer.elf -Wl,--start-group)
target_link_libraries(open_can_sniffer.elf debug m)

target_link_libraries(open_can_sniffer.elf debug c)

target_link_libraries(open_can_sniffer.elf debug gcc)

target_link_libraries(open_can_sniffer.elf debug nosys)

target_link_libraries(open_can_sniffer.elf optimized m)

target_link_libraries(open_can_sniffer.elf optimized c)

target_link_libraries(open_can_sniffer.elf optimized gcc)

target_link_libraries(open_can_sniffer.elf optimized nosys)

TARGET_LINK_LIBRARIES(open_can_sniffer.elf -Wl,--end-group)
