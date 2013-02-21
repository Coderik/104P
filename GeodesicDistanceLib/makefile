RM := rm -rf

TARGET = elf32-i386
#TARGET = elf64-x86-64 
ARCH = -m32
#ARCH = -m64

# Add inputs to the build variables 
CPP_SRCS += \
./abstract_marching_algorithm.cpp \
./coarse_to_fine.cpp \
./color_code.cpp \
./dijkstra_algorithm.cpp \
./fast_march_algorithm.cpp \
./field_operations.cpp \
./image.cpp \
./intensity_code.cpp \
./interpolation.cpp \
./optical_flow_container.cpp \
./optical_flow_io.cpp \
./pgm_util.cpp \
./point.cpp \
./sequence.cpp \
./shape.cpp \
./simple_distance_calculation.cpp \
./util.cpp \
./warping.cpp \
./zach_TVL1_optical_flow.cpp 

OBJS += \
bin/abstract_marching_algorithm.o \
bin/coarse_to_fine.o \
bin/color_code.o \
bin/dijkstra_algorithm.o \
bin/fast_march_algorithm.o \
bin/field_operations.o \
bin/image.o \
bin/intensity_code.o \
bin/interpolation.o \
bin/optical_flow_container.o \
bin/optical_flow_io.o \
bin/pgm_util.o \
bin/point.o \
bin/sequence.o \
bin/shape.o \
bin/simple_distance_calculation.o \
bin/util.o \
bin/warping.o \
bin/zach_TVL1_optical_flow.o 

CPP_DEPS += \
bin/abstract_marching_algorithm.d \
bin/coarse_to_fine.d \
bin/color_code.d \
bin/dijkstra_algorithm.d \
bin/fast_march_algorithm.d \
bin/field_operations.d \
bin/image.d \
bin/intensity_code.d \
bin/interpolation.d \
bin/optical_flow_container.d \
bin/optical_flow_io.d \
bin/pgm_util.d \
bin/point.d \
bin/sequence.d \
bin/shape.d \
bin/simple_distance_calculation.d \
bin/util.d \
bin/warping.d \
bin/zach_TVL1_optical_flow.d 

LIB_OUT = bin/libGeodesicDistanceLib.a


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ $(ARCH) -o "$@" "$<" -O0 -g3 -Wall -c -fmessage-length=0
	@echo 'Finished building: $<'
	@echo ' '

# All Target
all: libGeodesicDistanceLib.a

# Tool invocations
libGeodesicDistanceLib.a: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC Archiver'
	ar -r  $(LIB_OUT) $(OBJS) $(USER_OBJS) $(LIBS) --target=$(TARGET)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS) $(CPP_DEPS) $(LIB_OUT)
	-@echo ' '

.PHONY: all clean dependents

