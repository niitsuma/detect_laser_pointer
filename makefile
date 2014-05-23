CC		= g++
INCDIR		= /opt/local/include
CFLAGS		= -DBOOST_UBLAS_NDEBUG -Wall -I/$(INCDIR) -I. 
LIBS		= -lboost_unit_test_framework
SRCS		= hotelling_t2_test.cpp ublas-io-test.cpp
OBJS		= $(SRCS:.c=.o)
PROGRAM	=  hotelling_t2_test.exe

all:		$(PROGRAM)  test

%.exe: %.cpp
	$(CC) $(CFLAGS) $(LDFLAGS) $(LIBS)  -o $@ $<  

test : hotelling_t2_test.exe
	./hotelling_t2_test.exe


clean:		rm -f *.o *~ $(PROGRAM)
