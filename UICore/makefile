RM := rm -rf

#TARGET = elf32-i386
TARGET = elf64-x86-64 
#ARCH = -m32
ARCH = -m64

# Add inputs to the build variables 
SOURCES += \
background_worker.cpp \
hull.cpp \
image_viewer.cpp \
interaction_manager.cpp \
layer.cpp \
layer_manager.cpp \
optical_flow.cpp \
parameters_ui_builder.cpp \
signal_watchdog.cpp

OBJ_DIR = bin/
OBJECTS = $(patsubst %.cpp,%.o,$(SOURCES))
OBJ_PATHS = $(addprefix $(OBJ_DIR),$(OBJECTS))

LIBS := -lCore

LIB_OUT = bin/libUICore.a


# Each subdirectory must supply rules for building sources it contributes
%.o: %.cpp libCore.a
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ $(ARCH) -o $(OBJ_DIR)"$@" "$<" -I"../Core" -I"../Core/include" `pkg-config gtkmm-3.0 --cflags` -O0 -g3 -Wall -c -fmessage-length=0
	@echo 'Finished building: $<'
	@echo ' '

# All Target
all: libUICore.a

dependents:
	-cd ../Core && $(MAKE) clean all

# Tool invocations
libUICore.a: $(OBJECTS) libCore.a 
	@echo 'Building target: $@'
	@echo 'Invoking: GCC Archiver'
	ar -r  $(LIB_OUT) $(OBJ_PATHS) --target=$(TARGET)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJ_PATHS) $(CPP_DEPS) $(LIB_OUT)
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:
libCore.a:
	-cd ../Core && $(MAKE)

