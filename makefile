#=============================================================================================================================================================================

VR_FLAGS = -D _DEBUG -g -ggdb3 -std=c++17 -O3

ASSAN = -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr

DOP = -march=native -mavx -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -Wlarger-than=8192 -Wstack-usage=8192 -pie -fPIE

DIRECTORIES =  -IEXTRA -IEXTRA/STACK -I$(VR_SIMD)_F -I$(VR_NOSIMD)_F -IMEDIA

VR_FLAGS += $(ASSAN)

VR_FLAGS += $(DIRECTORIES)

VR_FLAGS += $(DOP)

#=============================================================================================================================================================================

VR_COMPILER = g++

VR_LIBS 	= -lsfml-graphics -lsfml-window -lsfml-system

VR_SIMD		= AVX2

VR_FUNCS	= AVX2_Mandelbrot

#=============================================================================================================================================================================

DO: FOLDERS DO_COMPILE

FOLDERS:
	mkdir -p OBJECTS $(VR_SIMD)_F

DO_COMPILE: OBJECTS/$(VR_SIMD).o OBJECTS/$(VR_FUNCS).o
	$(VR_COMPILER) OBJECTS/$(VR_SIMD).o OBJECTS/$(VR_FUNCS).o -o $(VR_SIMD) $(VR_LIBS) $(VR_FLAGS);


#=============================================================================================================================================================================

OBJECTS/$(VR_SIMD).o:  $(VR_SIMD)_F/$(VR_SIMD).cpp
	$(VR_COMPILER) -c -o OBJECTS/$(VR_SIMD).o  $(VR_SIMD)_F/$(VR_SIMD).cpp $(VR_FLAGS)

OBJECTS/$(VR_FUNCS).o:  $(VR_SIMD)_F/$(VR_FUNCS).cpp
	$(VR_COMPILER) -c -o OBJECTS/$(VR_FUNCS).o  $(VR_SIMD)_F/$(VR_FUNCS).cpp $(VR_FLAGS)

#=============================================================================================================================================================================

# vg:
# 	colour-valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(VR_SIMD)

#=============================================================================================================================================================================
