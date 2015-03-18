CMP = g++
MAIN = mainLoadSave
FUNC1 = load_save_test
EXEC = $(MAIN)

$(EXEC): $(MAIN).o $(FUNC1).o
	$(CMP) -o $(EXEC)  $(MAIN).o $(FUNC1).o

$(MAIN).o: $(MAIN).cpp 
	$(CMP) -c $(MAIN).cpp

$(FUNC1).o: $(FUNC1).cpp $(FUNC1).h
	$(CMP) -c $(FUNC1).cpp

clean:
	rm $(EXEC) *.o
