CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
SRC      = main.cpp Utils.cpp Person.cpp Customer.cpp Courier.cpp Parcel.cpp Delivery.cpp FileManager.cpp CourierSystem.cpp
TARGET   = CourierSystem

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET) $(TARGET).exe
