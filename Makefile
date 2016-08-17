# Makefile for the socket programming example
#

train_model_objects = train_model_main.o svm.o
cal_result_objects = cal_result_main.o svm.o

train_model: $(train_model_objects)
	g++ -o train_model $(train_model_objects)
cal_result: $(cal_result_objects)
	g++ -o cal_result $(cal_result_objects)

train_model_main: train_model_main.cpp
cal_result_main: cal_result_main.cpp
svm = svm.cpp

clean:
	rm -f *.o train_model cal_result
