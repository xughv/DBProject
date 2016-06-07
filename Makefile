SRCS		= src/util.cpp src/block_file.cpp src/b_node.cpp \
				src/b_tree.cpp src/medrank.cpp src/ann.cpp src/main.cpp
OBJS		= $(SRCS:.cpp=.o)
CXX			= g++ -std=c++11
CPPFLAGS	= -w -O3

.PHONY: clean

all: $(OBJS)
	$(CXX) -o medrank $(OBJS)

util.o: src/util.h

block_file.o: src/block_file.h

b_node.o: src/b_node.h

b_tree.o: src/b_tree.h

medrank.o: src/medrank.h

ann.o: src/ann.h

main.o:

clean:
	-rm $(OBJS) medrank
