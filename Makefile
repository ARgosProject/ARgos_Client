EXEC := argos

DIRLIBS := libs/
DIRSRC := src/
DIROBJ := obj/
DIRHEA := include/
DIRSHADERS := shaders/

CXX := g++

CXXFLAGS := `pkg-config --cflags opencv`
CXXFLAGS += -Wall -fexceptions -ggdb -O0 -std=c++0x -MMD -MP
CXXFLAGS += -DGLM_FORCE_RADIANS

INCLUDES := -I$(SDKSTAGE)/opt/vc/include/ -I$(SDKSTAGE)/opt/vc/include/interface/vcos/pthreads -I$(SDKSTAGE)/opt/vc/include/interface/vmcs_host/linux
INCLUDES += -I$/opt/vc/include/interface/mmal -I/usr/include/freetype2 -I./libs/ilclient
INCLUDES += -I$(DIRHEA) -I$(DIRLIBS)
INCLUDES += -I/usr/local/include

LDLIBS := -L$(SDKSTAGE)/opt/vc/lib/ -lGLESv2 -lEGL -lopenmaxil -lbcm_host
LDLIBS += -lboost_system -lboost_thread
LDLIBS += -lrt
LDLIBS += -L./libs/ilclient -lilclient
LDLIBS += -lX11 # sudo apt-get install libx11-dev
LDLIBS += -lfreetype # sudo apt-get install libfreetype6-dev
LDLIBS += -lstdc++
LDLIBS += `pkg-config --libs opencv`
LDLIBS += -lraspicam -lraspicam_cv
LDLIBS += -lmmal -lmmal_core -lmmal_util
LDLIBS += -lSOIL # sudo apt-get install libsoil-dev
LDLIBS += -lSDL -lSDL_mixer # sudo apt-get install libsdl-1.2-dev libsdl-mixer-1.2-dev
LDLIBS += -lavcodec -lavformat

OBJS := $(subst $(DIRSRC), $(DIROBJ), $(patsubst %.cpp, %.o, $(wildcard $(DIRSRC)*.cpp)))
OBJS += $(subst $(DIRLIBS)freetypeGlesRpi/, $(DIROBJ), $(patsubst %.cpp, %.o, $(wildcard $(DIRLIBS)freetypeGlesRpi/*.cpp)))
DEPS :=  $(OBJS:.o=.d)

COLOR_FIN := \033[00m
COLOR_OK := \033[01;32m
COLOR_ERROR := \033[01;31m
COLOR_AVISO := \033[01;33m
COLOR_COMP := \033[01;34m
COLOR_ENL := \033[01;35m

.PHONY: all clean

all: info $(EXEC)

info:
	@echo -e '$(COLOR_AVISO)------------------$(COLOR_FIN)'
	@echo -e '$(COLOR_AVISO)Construyendo $(COLOR_ERROR)$(EXEC)$(COLOR_FIN)'
	@echo -e '$(COLOR_AVISO)------------------$(COLOR_FIN)'
	@echo ''

$(EXEC): $(OBJS)
	@echo -e '$(COLOR_ENL)Enlazando$(COLOR_FIN): $(notdir $<)'
	@$(CXX) -o $@ $^ $(LDLIBS)
	@echo -e '$(COLOR_OK)Terminado.$(COLOR_FIN)'

-include $(DEPS)

$(DIROBJ)%.o: $(DIRSRC)%.cpp
	@echo -e '$(COLOR_COMP)Compilando$(COLOR_FIN): $(notdir $<)'
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(DIROBJ)%.o: $(DIRLIBS)freetypeGlesRpi/%.cpp
	@echo -e '$(COLOR_COMP)Compilando$(COLOR_FIN): $(notdir $<)'
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	find . \( -name '*.log' -or -name '*~' \) -delete
	rm -f $(EXEC) $(DIROBJ)*
