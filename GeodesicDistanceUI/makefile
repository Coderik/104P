RM := rm -rf

ARCH = -m32
#ARCH = -m64

# Add inputs and outputs to the build variables 
CPP_SRCS += \
./geodesic_distance_ui.cpp \
./main.cpp \
./optical_flow.cpp \
./selectable_image.cpp \
./signal_watchdog.cpp 

OBJS += \
bin/geodesic_distance_ui.o \
bin/main.o \
bin/optical_flow.o \
bin/selectable_image.o \
bin/signal_watchdog.o 

CPP_DEPS += \
bin/geodesic_distance_ui.d \
bin/main.d \
bin/optical_flow.d \
bin/selectable_image.d \
bin/signal_watchdog.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp libGeodesicDistanceLib.a
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ $(ARCH) -o "$@" "$<" -I"../GeodesicDistanceLib" -I"../GeodesicDistanceLib/headers" -O0 -g3 -Wall -c -fmessage-length=0 `pkg-config gtkmm-3.0 --cflags --libs`
	@echo 'Finished building: $<'
	@echo ' '

LIBS := -lGeodesicDistanceLib

# All Target
all: GeodesicDistanceUI

dependents:
	-cd ../GeodesicDistanceLib && $(MAKE) clean all

# Tool invocations
GeodesicDistanceUI: $(OBJS) libGeodesicDistanceLib.a
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	g++ $(ARCH) -o "bin/GeodesicDistanceUI" $(OBJS) $(LIBS) -L"../GeodesicDistanceLib/bin" `pkg-config gtkmm-3.0 --cflags --libs`
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(CPP_DEPS)$(EXECUTABLES) bin/GeodesicDistanceUI
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:
libGeodesicDistanceLib.a:
	-cd ../GeodesicDistanceLib && $(MAKE)
