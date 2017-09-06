programName := RayTracer
sourceFolder := sources
headerFolder := headers
objectFolder := objects

sources := $(wildcard $(sourceFolder)/*.cpp)
tmpVar := $(sources:.cpp=.o)
objects := $(subst $(sourceFolder),$(objectFolder),$(tmpVar))

incDir := /home/athis/Documents/Work/Code/C++/versioning_philips/MathUtils/ $(sourceFolder) $(headerFolder)
libDir := /usr/local/lib
lib := SDL_draw pthread
SDL_config := `sdl-config --libs`
OPT := -ggdb -std=c++11 -O0

CPPFLAGS += $(foreach includedir,$(incDir),-I$(includedir))
LDFLAGS += $(foreach librarydir,$(libDir),-L$(librarydir))
LDFLAGS += $(foreach library,$(lib),-l$(library))



all : $(programName)

$(objects) : $(objectFolder)/%.o : $(sourceFolder)/%.cpp
	g++ $(OPT)  $(CPPFLAGS) -c $< $(LDFLAGS) $(SDL_config)
	mv *.o $(objectFolder)
	

$(programName) : $(objects)	
	g++ $(OPT) $(CPPFLAGS) -o $(programName) $(objects) $(LDFLAGS) $(SDL_config)	

clean :
	rm -rf $(objectFolder)/*.o
	rm -f RayTracer

report :
	@echo "--------------Reporting -------------"
	@echo "Program Name : " $(programName)
	@echo "Source Folder : " $(sourceFolder)
	@echo "Header Folder : " $(headerFolder)
	@echo "Sources C: " $(sourcesC)
	@echo "Sourecs CPP : " $(sourcesCPP)
	@echo "Full sources : " $(sources)
	@echo "Objects C : " $(objectsC)
	@echo "Objects CPP : " $(objectsCPP)
	@echo "Full objects : " $(objects)
	@echo "Include directory : " $(incDir)
	@echo "Library directory : " $(libDir)
	@echo "Libraries" $(lib)
	@echo "CPPFLAGS : " $(CPPFLAGS)
	@echo "LDFLAGS : " $(LDFLAGS)
	@echo "--------------Reporting -------------"
	@echo ""