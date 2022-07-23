EXE := server

# wildcardɨ��Դ�ļ�
sources := ${wildcard *.cpp}
headfile := ${wildcard *.h}
objects := ${sources:.cpp=.o}

CC := g++ -std=c++11 -g
RM := rm -rf
LIB := -lpthread

${EXE}: ${objects}
	${CC} -o $@ $^ ${LIB}

${objects}: %.o: %.cpp ${headfile}
	${CC} -o $@ -c $<
# CC
# αĿ�꣬��ζ��clean������һ���������ļ���
.PHONY: clean cleanall
cleanall:
	${RM} ${EXE} ${objects}
clean:
	${RM} ${objects}
