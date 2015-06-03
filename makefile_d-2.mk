C++_SRCS := main.cpp
C++_OBJ := main.o
C++_EXE := main.exe

DRAWSTUFF_OBJ	:= C:\ode-0.11.1\build\ODE_D_2\obj\DebugDoubleLib\ODE_D_2\drawstuff\src\resources.res

INC:=	-IC:\ode-0.11.1\include

LIBS:=	-lode_doubled -ldrawstuffd -luser32 -lwinmm -lgdi32 -lopengl32 -lglu32
#LIBS:=	-ldrawstuffd

RM	:= del

# All Target
all: clean ODE_D_2.exe



ODE_D_2.exe: make_obj
#ODE_D_2.exe: $(OBJS) $(USER_OBJS)
	
	echo "linking..."
	
	g++ -L"C:\ode-0.11.1\lib\DebugDoubleLib" -o "ODE_D_2.exe" $(C++_OBJ) $(DRAWSTUFF_OBJ) $(LIBS)
	
#	g++.exe -L..\..\lib\DebugDoubleLib -o ..\..\ode_executables\DebugDoubleLib\ODE_D_2.exe  obj\DebugDoubleLib\ODE_D_2\build\ODE_D_2\main.o obj\DebugDoubleLib\ODE_D_2\drawstuff\src\resources.res  -lode_doubled -ldrawstuffd -luser32 -lwinmm -lgdi32 -lopengl32 -lglu32
	
#	g++ $(C++_SRCS) $(INC) -S -o $(C++_OBJ)
#	@echo 'Building target: $@'
#	@echo 'Invoking: MinGW C++ Linker'
#	g++ -L"C:\ode-0.11.1\lib\DebugDoubleLib" -o "ODE_D_2.exe" $(OBJS) $(USER_OBJS) $(LIBS)
#	@echo 'Finished building target: $@'
#	@echo ' '

make_obj:

	echo "creating obj files..."
	
#	echo g++ $(C++_SRCS) $(INC) -S -o $(C++_OBJ)
	g++ -g -D_DEBUG -DdDOUBLE -DWIN32 $(C++_SRCS) $(INC) -c -o $(C++_OBJ)
#	g++ -g -D_DEBUG -DdDOUBLE -DWIN32 $(C++_SRCS) $(INC) -S -o $(C++_OBJ)
	
	
clean:
	
	$(RM) $(C++_OBJ)
	