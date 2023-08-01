FRAMEWORKS_DIR         = /Library/Frameworks/

SDL2_INCLUDE_DIR       = /Library/Frameworks/SDL2.framework/Headers/
SDL2_IMAGE_INCLUDE_DIR = /Library/Frameworks/SDL2_image.framework/Headers/
SDL2_TTF_INCLUDE_DIR   = /Library/Frameworks/SDL2_ttf.framework/Headers/

GLEW_INCLUDE_DIR       = /opt/homebrew/Cellar/glew/2.2.0_1/include/
GLEW_LIB_DIR           = /opt/homebrew/Cellar/glew/2.2.0_1/lib/

GLU_INCLUDE_DIR        = /opt/homebrew/Cellar/mesa-glu/9.0.2/include/
GLU_LIB_DIR            = /opt/homebrew/Cellar/mesa-glu/9.0.2/lib/

INCLUDE_DIR            = ./include/
SRC_DIR                = ./src/

INCLUDE                = -I$(SDL2_INCLUDE_DIR) -I$(SDL2_IMAGE_INCLUDE_DIR) -I$(SDL2_TTF_INCLUDE_DIR)-I$(GLEW_INCLUDE_DIR) -I$(GLU_INCLUDE_DIR) -I$(INCLUDE_DIR)
FRAMEWORKS             = -F$(FRAMEWORKS_DIR) -framework OpenGL -framework SDL2 -framework SDL2_image -framework SDL2_ttf -framework CoreMIDI -framework CoreAudio -framework CoreFoundation 
LIBRARIES              = -L$(GLEW_LIB_DIR) -L$(GLU_LIB_DIR) -lGLEW -lglu

PREPROCESSOR_DIRECTIVES = -D__MACOSX_CORE__ --std=c++20

NAME                   = retro-synth.app

all:
	clang++ $(PREPROCESSOR_DIRECTIVES) $(SRC_DIR)*.cpp $(INCLUDE) $(FRAMEWORKS) $(LIBRARIES) -o $(NAME)
