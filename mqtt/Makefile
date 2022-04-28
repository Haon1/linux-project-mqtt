SRC_DIR = ./src
OUT_DIR = ./out
OBJ_DIR = $(OUT_DIR)/obj
BIN_DIR = $(OUT_DIR)/bin

SOURCE = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(patsubst %.c,$(OBJ_DIR)/%.o,$(notdir $(SOURCE)))
DEST = $(BIN_DIR)/mqtt_demo

LIB = lpthread
INC = ./include

$(DEST):$(OBJ)
#	if [ ! -e $(OUT_DIR) ];then
#	mkdir $(OUT_DIR)
#	mkdir $(OBJ_DIR)
#	mkdir $(BIN_DIR)
#	fi
	$(CC) $(SOURCE) -o $(DEST) -I$(INC) -$(LIB)

clean:     
	if [ -e $(OUT_DIR) ];then
	$(RM) $(OUT_DIR) -rf
	fi
	echo "clean OK"
	

