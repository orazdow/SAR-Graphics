TARGET = out

SRC = main.cpp 
SRC2 = D:/Libraries/glad/src
SRC += $(SRC2)/glad.c
 
INCLUDE = -I D:/Libraries/glad/include 
INCLUDE += -I D:/Libraries/glfw/include
INCLUDE += -I D:/Libraries/SOIL/src

LIBS = -L D:/Libraries/glfw/lib-mingw 
LIBS += -L D:/Libraries/SOIL/lib 
LIBS += -lSOIL -lglfw3 -lgdi32 -lopengl32 -lpthread 

CXX = g++
FLAGS = -std=c++17

OBJ = $(notdir $(patsubst %.c, %.o,  $(SRC)))
OBJ := $(notdir $(patsubst %.cpp, %.o,  $(OBJ)))
DEPS = $(wildcard *.h)

%.o : $(SRC2)/%.c
	$(CXX) -x c $(INCLUDE) $(FLAGS) -c $< -o $@ 

%.o : %.cpp $(DEPS)
	$(CXX) $(INCLUDE) $(FLAGS) -c $< -o $@  

all: $(TARGET)

$(TARGET): $(OBJ) 
	$(CXX) $(INCLUDE) $(FLAGS) $^ -o $@ $(LIBS)

clean:
	rm -f $(TARGET) $(wildcard *.o)

run:
	@./$(TARGET)
