# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall

# Directories
SRC_DIR = src
BUILD_DIR = build
POCO_DIR = /opt/iot/poco

# Include paths
INCLUDES = -I. -I$(POCO_DIR)/include -Isrc -Isrc/web

# Libraries and linker flags
LIBS = -L$(POCO_DIR)/lib -lPocoNet -lPocoUtil -lPocoFoundation -lPocoJSON -lpthread
LDFLAGS = -Wl,-rpath,$(POCO_DIR)/lib

# Target executables
SERVER_TARGET = ebikeGateway
CLIENT_TARGET = ebikeClient
GENERATOR_TARGET = generateEBikeFile
MANAGEMENT_TARGET = managementClient

# Source files
SERVER_SRC = $(SRC_DIR)/ebikeGateway.cpp
CLIENT_SRC = $(SRC_DIR)/ebikeClient.cpp
GENERATOR_SRC = $(SRC_DIR)/util/generateEBikeFile.cpp
WEBSERVER_SRC = $(SRC_DIR)/web/WebServer.cpp
EBIKEHANDLER_SRC = $(SRC_DIR)/web/EbikeHandler.cpp
MANAGEMENT_SRC = $(SRC_DIR)/managementClient.cpp

# Object files
SERVER_OBJ = $(BUILD_DIR)/ebikeGateway.o
CLIENT_OBJ = $(BUILD_DIR)/ebikeClient.o
GENERATOR_OBJ = $(BUILD_DIR)/generateEBikeFile.o
WEBSERVER_OBJ = $(BUILD_DIR)/WebServer.o
EBIKEHANDLER_OBJ = $(BUILD_DIR)/EbikeHandler.o
MANAGEMENT_OBJ = $(BUILD_DIR)/managementClient.o

all: $(BUILD_DIR) $(SERVER_TARGET) $(CLIENT_TARGET) $(GENERATOR_TARGET) $(MANAGEMENT_TARGET)

$(BUILD_DIR):
		mkdir -p $(BUILD_DIR)

$(SERVER_TARGET): $(SERVER_OBJ) $(WEBSERVER_OBJ) $(EBIKEHANDLER_OBJ)
		$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^ $(LIBS) $(LDFLAGS)

$(CLIENT_TARGET): $(CLIENT_OBJ)
		$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^ $(LIBS) $(LDFLAGS)

$(GENERATOR_TARGET): $(GENERATOR_OBJ)
		$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^ $(LIBS) $(LDFLAGS)

$(MANAGEMENT_TARGET): $(MANAGEMENT_OBJ)
		$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^ $(LIBS) $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
		$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(GENERATOR_OBJ): $(GENERATOR_SRC)
		$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(WEBSERVER_OBJ): $(WEBSERVER_SRC)
		$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(EBIKEHANDLER_OBJ): $(EBIKEHANDLER_SRC)
		$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(MANAGEMENT_OBJ): $(MANAGEMENT_SRC)
		$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
		rm -rf $(BUILD_DIR) $(SERVER_TARGET) $(CLIENT_TARGET) $(GENERATOR_TARGET) $(MANAGEMENT_TARGET)
