CC		= g++
INCDIR		= /opt/local/include
CFLAGS		= -Wall \
		-I/$(INCDIR) \
		-I. -I./hotelling_t_square_test

LIBS		=  -lopencv_core -lopencv_highgui -lopencv_objdetect -lopencv_imgproc -lopencv_video -lopencv_contrib -lopencv_nonfree -lopencv_features2d 
#LIBS		= -lboost_unit_test_framework

OBJS		= $(SRCS:.c=.o) 

PROGRAM		=  cv_detect_laser.exe

#all:		$(PROGRAM) test
all:		$(PROGRAM)

%.exe: %.cpp
	$(CC) $(CFLAGS) $(LDFLAGS) $(LIBS) -o $@ $<


%.exe: %.c
	gcc $(CFLAGS) $(LDFLAGS) $(LIBS) -o $@ $<



clean:		
	rm -f *.o *~  $(PROGRAM)


test : $(PROGRAM) 
	./cv_detect_laser.exe




