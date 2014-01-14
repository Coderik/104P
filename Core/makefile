RM := rm -rf

#TARGET = elf32-i386
TARGET = elf64-x86-64 
#ARCH = -m32
ARCH = -m64

# Add inputs to the build variables 
SOURCES += \
coarse_to_fine.cpp \
color_code.cpp \
composite_image_mask.cpp \
distance_transform.cpp \
field_operations.cpp \
filtering.cpp \
gaussian_kernel.cpp \
image.cpp \
image_mask.cpp \
intensity_code.cpp \
interpolation.cpp \
mask_iterator.cpp \
optical_flow_container.cpp \
optical_flow_io.cpp \
optical_flow_io_legacy.cpp \
patch.cpp \
pgm_util.cpp \
point.cpp \
range_parameter.cpp \
sequence.cpp \
sequence_mask.cpp \
shape.cpp \
utility.cpp \
visualization.cpp \
warping.cpp \
zach_TVL1_optical_flow.cpp

OBJ_DIR = bin/
OBJECTS = $(patsubst %.cpp,%.o,$(SOURCES))
OBJ_PATHS = $(addprefix $(OBJ_DIR),$(OBJECTS))

LIB_OUT = bin/libCore.a


# Each subdirectory must supply rules for building sources it contributes
%.o: %.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ $(ARCH) -o $(OBJ_DIR)"$@" "$<" -Iinclude `pkg-config sigc++-2.0 --cflags` -O0 -g3 -Wall -c -fmessage-length=0
	@echo 'Finished building: $<'
	@echo ' '

# All Target
all: libCore.a

# Tool invocations
libCore.a: $(OBJECTS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC Archiver'
	ar -r  $(LIB_OUT) $(OBJ_PATHS) $(USER_OBJS) $(LIBS) --target=$(TARGET)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJ_PATHS) $(CPP_DEPS) $(LIB_OUT)
	-@echo ' '

.PHONY: all clean dependents
