SDL2_INCLUDE_DIR       = /Library/Frameworks/SDL2.framework/Headers
SDL2_IMAGE_INCLUDE_DIR = /Library/Frameworks/SDL2_image.framework/Headers
FRAMEWORKS_DIR         = /Library/Frameworks
SRC_DIR                = ./src/
INCLUDE_DIR            = ./include/

all:
	clang++ -o retro-synth.app $(SRC_DIR)*.cpp --std=c++17 -I$(SDL2_INCLUDE_DIR) -I$(SDL2_IMAGE_INCLUDE_DIR) -I$(INCLUDE_DIR) -F$(FRAMEWORKS_DIR) -framework SDL2 -framework SDL2_image
