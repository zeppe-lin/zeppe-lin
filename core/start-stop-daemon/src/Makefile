all: start-stop-daemon start-stop-daemon.8

%: %.pod
	pod2man --nourls -r $(or $(VERSION),dbg) -c ' ' -n revdep -s 8 $^ > $@

.cpp.o:
	$(CXX) -c -Wall -Wextra -D$(or $(VERSION),dbg) $(CXXFLAGS) $< -o $@

start-stop-daemon: start-stop-daemon.o
	$(CXX) $^ $(LDFLAGS) -o $@

install: all
	install -m 755 -D -t $(DESTDIR)/sbin/               start-stop-daemon
	install -m 644 -D -t $(DESTDIR)/usr/share/man/man8/ start-stop-daemon.8

uninstall:
	rm -f $(DESTDIR)/sbin/start-stop-daemon
	rm -f $(DESTDIR)/usr/share/man/man8/start-stop-daemon.8

clean:
	rm -f $(OBJS) start-stop-daemon{,.8}

.PHONY: all install uninstall clean
