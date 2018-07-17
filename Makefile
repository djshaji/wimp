GTK_CFLAGS=`pkg-config gtk+-2.0 --cflags`
XINE_CFLAGS=`xine-config --cflags`

GTK_LIBS=`pkg-config gtk+-2.0 --libs`
XINE_LIBS=`xine-config --libs`

wimp1:
	cc -c *.c $(GTK_CFLAGS) $(XINE_CFLAGS) -w
	cc *.o $(GTK_LIBS) $(XINE_LIBS) -o wimp -w
	
debug:
	cc -g -c *.c $(GTK_CFLAGS) $(XINE_CFLAGS) -w
	cc *.o $(GTK_LIBS) $(XINE_LIBS) -o wimp -w

install:
	if ! test -d ~/bin ; then mkdir -v ~/bin ; fi
	cp -v wimp ~/bin

clean:
	rm -v *.o
