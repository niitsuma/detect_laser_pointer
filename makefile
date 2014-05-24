CC		= g++
INCDIR		= /opt/local/include
CFLAGS		= -DBOOST_UBLAS_NDEBUG -Wall -I/$(INCDIR) -I. 
LIBS		= -lboost_unit_test_framework
SRCS		= hotelling_t2_test.cpp ublas-io-test.cpp
OBJS		= $(SRCS:.c=.o)
PROGRAM	=  ublas_hotelling_t2_test.exe cv_hotelling_t2_test.exe

all:		$(PROGRAM)  test

cv_hotelling_t2_test.exe : cv_hotelling_t2_test.cpp cv_hotelling_t2.hpp
	g++ -lopencv_core -lboost_unit_test_framework -o $@ $<  

ublas_hotelling_t2_test.exe : ublas_hotelling_t2_test.cpp ublas_hotelling_t2.hpp 
	g++ -DBOOST_UBLAS_NDEBUG -lboost_unit_test_framework -o ublas_hotelling_t2_test.exe ublas_hotelling_t2_test.cpp

#%.exe: %.cpp
#	$(CC) $(CFLAGS) $(LDFLAGS) $(LIBS)  -o $@ $<  


test : $(PROGRAM) 
	./ublas_hotelling_t2_test.exe
	./cv_hotelling_t2_test.exe


clean:		
	rm -f *.o *~  $(PROGRAM)
