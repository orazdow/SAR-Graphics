TARGET = out

SRC = main.cpp 
SRC2 = D:/Libraries/glad/src
SRC += $(SRC2)/glad.c

OBJDIR = build
 
INCLUDE = -I D:/Libraries/glad/include 
INCLUDE += -I D:/Libraries/glfw/include
INCLUDE += -I D:/Libraries/SOIL/src

LIBS = -L D:/Libraries/glfw/lib-mingw 
LIBS += -L D:/Libraries/SOIL/lib 
LIBS += -lSOIL -lglfw3 -lgdi32 -lopengl32 -lpthread 

CXX = g++
# FLAGS = -std=c++17

OBJ = $(notdir $(patsubst %.c, %.o,  $(SRC)))
OBJ := $(notdir $(patsubst %.cpp, %.o,  $(OBJ)))
OBJ := $(addprefix $(OBJDIR)/, $(OBJ))
DEPS = $(wildcard *.h) $(wildcard scenes/*.h)

$(OBJDIR)/%.o : $(SRC2)/%.c
	$(CXX) -x c $(INCLUDE) $(FLAGS) -c $< -o $@ 

$(OBJDIR)/%.o : %.cpp $(DEPS)
	$(CXX) $(INCLUDE) $(FLAGS) -c $< -o $@  

all: make_dir $(TARGET)

$(TARGET): $(OBJ) 
	$(CXX) $(INCLUDE) $(FLAGS) $^ -o $@ $(LIBS)

make_dir: $(OBJDIR)
$(OBJDIR):
	@mkdir -p $(OBJDIR)

clean:
	rm -f $(TARGET) $(wildcard $(OBJDIR)/*.o)

run:
	@./$(TARGET)


