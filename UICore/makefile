RM := rm -rf

TARGET = elf32-i386
#TARGET = elf64-x86-64 
ARCH = -m32
#ARCH = -m64

# Add inputs to the build variables 
CPP_SRCS += \
./hull.cpp \
./layer.cpp \
./layer_manager.cpp \
./optical_flow.cpp \
./selectable_image.cpp \
./signal_watchdog.cpp 

OBJS += \
bin/hull.o \
bin/layer.o \
bin/layer_manager.o \
bin/optical_flow.o \
bin/selectable_image.o \
bin/signal_watchdog.o 

CPP_DEPS += \
bin/hull.d \
bin/layer.d \
bin/layer_manager.d \
bin/optical_flow.d \
bin/selectable_image.d \
bin/signal_watchdog.d 

LIBS := -lAlgorithm

LIB_OUT = bin/libUICore.a


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp libAlgorithm.a
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ $(ARCH) -o "$@" "$<" -I"../Algorithm" -I"../Algorithm/headers" `pkg-config gtkmm-3.0 --cflags` -O0 -g3 -Wall -c -fmessage-length=0
	@echo 'Finished building: $<'
	@echo ' '

# All Target
all: libUICore.a

dependents:
	-cd ../Algorithm && $(MAKE) clean all

# Tool invocations
libUICore.a: $(OBJS) libAlgorithm.a 
	@echo 'Building target: $@'
	@echo 'Invoking: GCC Archiver'
	ar -r  $(LIB_OUT) $(OBJS) --target=$(TARGET)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS) $(CPP_DEPS) $(LIB_OUT)
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:
libAlgorithm.a:
	-cd ../Algorithm && $(MAKE)

