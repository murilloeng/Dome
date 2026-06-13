#compiler
CXX = g++
WARS = -Wall -Werror
INCS = -I .. -I /usr/include/freetype2
LIBS = -l GL -l freetype -l glfw -l umfpack -l fftw3 -l quadrule
CXXFLAGS = -std=c++20 -fPIC -pipe -fopenmp -MT $@ -MMD -MP -MF $(subst .o,.d, $@) $(DEFS) $(INCS) $(WARS)

#mode
ifneq ($(m), r)
	mode = debug
	CXXFLAGS += -ggdb3
else
	mode = release
	CXXFLAGS += -Ofast
endif

#profiling
ifeq ($(p), 1)
	LNKFLAGS += -pg
	CXXFLAGS += -pg
	prof_dir = profiling/
endif

#output
out = dist/$(prof_dir)$(mode)/dome.out

#libraries
libMath = ../Math/dist/$(mode)/libmath.so
libCanvas = ../Canvas/dist/$(mode)/libcanvas.so

#sources
src := $(sort $(shell find -path './src/*.cpp'))

#objects
obj = $(sort $(subst ./src/, build/$(prof_dir)$(mode)/, $(addsuffix .o, $(basename $(src)))))

#dependencies
dep = $(subst .o,.d, $(obj))

#rules
all : exe

run : exe
	./$(out)

debug : exe
	gdb ./$(out)

exe : math canvas $(out)
	@echo 'executable build - $(mode): complete!'

math : 
	+@cd ../Math && $(MAKE) -f Makefile m=$m

canvas : 
	+@cd ../Canvas && $(MAKE) -f Makefile m=$m

plot : 
	@gnuplot -p plot.gp

animation :
	@./animation.sh

$(out) : $(obj)
	@mkdir -p $(dir $@)
	@$(CXX) $(LNKFLAGS) -o $(out) $(obj) $(libMath) $(libCanvas) $(LIBS)
	@echo 'executable - $(mode): $@'

build/$(prof_dir)$(mode)/%.o : src/%.cpp build/$(prof_dir)$(mode)/%.d
	@echo 'compiling - $(mode): $<'
	@mkdir -p $(dir $@) && rm -rf $@
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(dep) : ;

-include $(dep)

clean :
	@rm -rf dist/$(mode)
	@rm -rf build/$(mode)
	@echo 'clean - $(mode): complete!'

print-% :
	@echo $* = $($*)

.PHONY : all clean print-%