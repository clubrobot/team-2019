OUTPUT_DIR = ../Module
CC=g++
EXT = $(SOURCES:.cpp=)
OBJ= $(SOURCES:.cpp=.o)
HEADERS =$(SOURCES:.cpp=.h)
PYTHON_LIB = $(shell find  /usr/include/*m  -name python?.?m)
PYTHON_VER2= $(shell find  /usr/include/*m  -name python?.?m | sed 's/\/usr\/include\/python//')
PYTHON_VER= $(shell find  /usr/include/*m  -name python?.?m | sed 's/\/usr\/include\/python//' | sed 's/m//' | sed 's/\.//' )
CFLAGS = -std=c++11 -ftemplate-depth-128 -O3 -fno-inline -Wall -g -fPIC -I$(PYTHON_LIB) -c -lpthread 

main.o : main.cpp
	$(CC) $(CFLAGS) -o $@ $<

%.o : %.cpp %.h
	$(CC) $(CFLAGS) -o $@ $<

module : main.o $(OBJ)
	$(CC) -o $(MODULE_NAME).so -Wl,-h -Wl,$(MODULE_NAME).so -shared -Wl,--start-group $< $(OBJ) -Wl,-Bstatic -Wl,-Bdynamic -lboost_python-py$(PYTHON_VER) -lpython$(PYTHON_VER2) $(LIBS) -ldl -lpthread -lutil -Wl,--end-group
	@cp $(MODULE_NAME).so $(OUTPUT_DIR)/$(MODULE_NAME).so 

clean:
	rm *.o *.so
	rm $(OUTPUT_DIR)/$(MODULE_NAME).so 

