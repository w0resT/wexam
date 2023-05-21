#CXX = g++
#CXX = clang++

RESOURCES = ../resources
OUTPUT_BUILD = ../build
OUTPUT_BIN = ../bin

EXE = wexam
APP_DIR = application
CONTRL_DIR = controller
FILEBR_DIR = FileBrowser
GUI_DIR = gui
INTRFS_DIR = interfaces
LOCALIZ_DIR = localization
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
SOURCES += $(GUI_DIR)/pages/pagemanager.cpp $(GUI_DIR)/pages/page_users.cpp $(GUI_DIR)/pages/page_tests.cpp $(GUI_DIR)/pages/page_settings.cpp $(GUI_DIR)/pages/page_info.cpp
SOURCES += $(LOCALIZ_DIR)/localization_manager.cpp $(LOCALIZ_DIR)/localization_reader.cpp $(LOCALIZ_DIR)/localization_writer.cpp
SOURCES += $(OBS_DIR)/observer.cpp
SOURCES += $(SECURITY_DIR)/aes_text_cipher.cpp $(SECURITY_DIR)/totp_generator.cpp
SOURCES += $(TSYSTEM_DIR)/question_with_free_answer.cpp $(TSYSTEM_DIR)/question_with_options.cpp $(TSYSTEM_DIR)/test.cpp $(TSYSTEM_DIR)/testmanager.cpp $(TSYSTEM_DIR)/testrepository.cpp $(TSYSTEM_DIR)/testresult.cpp
SOURCES += $(USERS_DIR)/student.cpp $(USERS_DIR)/studentrepository.cpp $(USERS_DIR)/usermanager.cpp

OBJS = $(addsuffix .o, $(basename $(notdir $(SOURCES))))
UNAME_S := $(shell uname -s)
LINUX_GL_LIBS = -lGL

CXXFLAGS = -std=c++17
CXXFLAGS += -I$(APP_DIR) -I$(CONTRL_DIR) -I$(FILEBR_DIR) 
CXXFLAGS += -I$(GUI_DIR) -I$(GUI_DIR)/imgui -I$(GUI_DIR)/imgui/backend -I$(GUI_DIR)/pages 
CXXFLAGS += -I$(INTRFS_DIR) -I$(INTRFS_DIR)/controller -I$(INTRFS_DIR)/gui -I$(INTRFS_DIR)/localization -I$(INTRFS_DIR)/security -I$(INTRFS_DIR)/tsystem -I$(INTRFS_DIR)/users
CXXFLAGS += -I$(LOCALIZ_DIR) -I$(OBS_DIR) -I$(SECURITY_DIR) -I$(TSYSTEM_DIR) -I$(USERS_DIR)

CXXFLAGS += -g -Wall -Wformat
LIBS =

##---------------------------------------------------------------------
## BUILD FLAGS PER PLATFORM
##---------------------------------------------------------------------

ifeq ($(UNAME_S), Linux) #LINUX
	ECHO_MESSAGE = "Linux"
	LIBS += $(LINUX_GL_LIBS) `pkg-config --static --libs glfw3`
	LIBS += -L/usr/lib -lssl -lcrypto

	CXXFLAGS += `pkg-config --cflags glfw3`
	CXXFLAGS += -I/usr/include
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

%.o:$(APP_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:$(CONTRL_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<
	
%.o:$(FILEBR_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<
	
%.o:$(GUI_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:$(GUI_DIR)/imgui/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<
	
%.o:$(GUI_DIR)/imgui/backend/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<
	
%.o:$(GUI_DIR)/pages/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<
	
%.o:$(LOCALIZ_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<
	
%.o:$(OBS_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<
	
%.o:$(SECURITY_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<
	
%.o:$(TSYSTEM_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<
	
%.o:$(USERS_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

all: $(EXE)
	mkdir -p $(OUTPUT_BIN)
	mv $(OBJS) $(OUTPUT_BIN)
	cp -R $(RESOURCES) $(OUTPUT_BUILD)
	@echo Build complete for $(ECHO_MESSAGE)

$(EXE): $(OBJS)
	mkdir -p $(OUTPUT_BUILD)
	$(CXX) -o $(OUTPUT_BUILD)/$@ $^ $(CXXFLAGS) $(LIBS)

clean:
	rm -f $(EXE) $(OBJS) $(OUTPUT_BIN)/* $(OUTPUT_BUILD)/*

