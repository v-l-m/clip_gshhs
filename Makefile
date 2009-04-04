CC = gcc
CFLAGS = -Wall -O3
LDFLAGS = -lm
LIBS =  -lgd -lpng

EXEC=readgshhs read_bd map_g

all: $(EXEC)

# ReadGSHHS	
readgshhs: readgshhs.o read_gshhs.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LIBS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS) $(LIBS)


# Read_BD	
read_bd: read_bd.o gpc.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LIBS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS) $(LIBS)


# Map_G		
map_g: map.o map_functions.o map_projection.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LIBS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS) $(LIBS)


#Cleaner!
clean:
	rm -f *.o
	rm -f *~        

