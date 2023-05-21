OUTPUT_BUILD = ../build
OUTPUT_BIN = ../bin

EXE = wexem
APP_DIR = application
CONTRL_DIR = controller
FILEBR_DIR = FileBrowser
GUI_DIR = gui
INTRFS_DIR = interfaces
LOCALIZ_DIR = localization
MODEL_DIR = model
OBS_DIR = observer
SECURITY_DIR = security
TSYSTEM_DIR = tsystem
USERS_DIR = users

SOURCES = main.cpp
SOURCES += $(APP_DIR)/application.cpp
SOURCES += $(CONTRL_DIR)/appcontroller.cpp
SOURCES += $(FILEBR_DIR)/ImGuiFileBrowser.cpp
SOURCES += $(GUI_DIR)/gui.cpp $(GUI_DIR)/imguimanager.cpp
SOURCES += $(GUI_DIR)/imgui/imgui.cpp $(GUI_DIR)/imgui/imgui_draw.cpp $(GUI_DIR)/imgui/imgui_stdlib.cpp $(GUI_DIR)/imgui/imgui_tables.cpp $(GUI_DIR)/imgui/imgui_widgets.cpp
SOURCES += $(GUI_DIR)/imgui/backends/imgui_impl_glfw.cpp $(GUI_DIR)/imgui/backends/imgui_impl_opengl3.cpp

OBJS = $(addsuffix .o, $(basename $(notdir $(SOURCES))))
UNAME_S := $(shell uname -s)
LINUX_GL_LIBS = -lGL

CXXFLAGS = -std=c++17 -I$(IMGUI_DIR) -I$(IMGUI_DIR)/backends
CXXFLAGS += -g -Wall -Wformat
LIBS =

##---------------------------------------------------------------------
## BUILD FLAGS PER PLATFORM
##---------------------------------------------------------------------

ifeq ($(UNAME_S), Linux) #LINUX
	ECHO_MESSAGE = "Linux"
	LIBS += $(LINUX_GL_LIBS) `pkg-config --static --libs glfw3`

	CXXFLAGS += `pkg-config --cflags glfw3`
	CFLAGS = $(CXXFLAGS)
endif

ifeq ($(OS), Windows_NT)
	ECHO_MESSAGE = "MinGW"
	LIBS += -lglfw3 -lgdi32 -lopengl32 -limm32

	CXXFLAGS += `pkg-config --cflags glfw3`
	CFLAGS = $(CXXFLAGS)
endif

##---------------------------------------------------------------------
## BUILD RULES
##---------------------------------------------------------------------

%.o:%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:$(IMGUI_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:$(IMGUI_DIR)/backends/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

all: $(EXE)
	mkdir -p $(OUTPUT_BIN)
	mv $(OBJS) $(OUTPUT_BIN)
	@echo Build complete for $(ECHO_MESSAGE)

$(EXE): $(OBJS)
	mkdir -p $(OUTPUT_BUILD)
	$(CXX) -o $(OUTPUT_BUILD)/$@ $^ $(CXXFLAGS) $(LIBS)

clean:
	rm -f $(EXE) $(OBJS) $(OUTPUT_BIN)/* $(OUTPUT_BUILD)/*

