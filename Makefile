LIBAO_INCLUDE = /opt/homebrew/Cellar/libao/1.2.2/include
LIBAO_LIB     = /opt/homebrew/Cellar/libao/1.2.2/lib
all:
	gcc -o retro-synth.app *.cpp -I$(LIBAO_INCLUDE) -L$(LIBAO_LIB) -lao -ldl -lm
