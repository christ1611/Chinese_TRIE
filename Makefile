all: pinyin 
pinyin: main.o
	@echo 'Building target: $@'
	@echo 'Invoking: MinGW C++ Linker'
	g++ -std=c++11 main.o -o pinyin
	@echo 'Finished building target: $@'
	@echo ' '

main.o: main.cpp
	g++ -std=c++11 -c main.cpp

clean: 
	-$(RM) $(CC_DEPS)$(C++_DEPS)$(EXECUTABLES)$(OBJS)$(C_UPPER_DEPS)$(CXX_DEPS)$(CPP_DEPS)$(C_DEPS) pinyin
	-@echo ' '

