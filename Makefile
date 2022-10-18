target := test 
library := 
objects += $(patsubst %.c,%.o,$(wildcard src/*.c))
objects += $(patsubst %.cpp,%.o,$(wildcard src/*.cpp))

.PHONY:all
all:$(target) 

%.o : %.c
	gcc -Wp  -c -o $@ $<

%.o : %.cpp
	g++ -Wp, -c -o $@ $<


$(target) : $(objects) 
	g++ $^ $(library)  -o $@

clean:
	rm -rf libs *.o $(target)
