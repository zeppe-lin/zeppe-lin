include config.mk

OBJSRC := $(wildcard *.cpp)
MANSRC := $(wildcard *.pod)

OBJS   := $(OBJSRC:.cpp=.o)
MANS   := $(MANSRC:.pod=)

all: pkgman $(MANS)

%: %.pod
	pod2man --nourls -r $(VERSION) -c ' ' -n $(basename $@) \
		-s $(subst .,,$(suffix $@)) $< > $@

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) $< -o $@

pkgman: $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

install: all
	install -m 755 -D -t $(DESTDIR)$(BINDIR)/       pkgman
	install -m 644 -D -t $(DESTDIR)$(MANDIR)/man1/  *.1
	install -m 644 -D -t $(DESTDIR)$(MANDIR)/man5/  *.5
	install -m 644 -D -t $(DESTDIR)$(MANDIR)/man8/  *.8
	ln -sf pkgman-install.8 $(DESTDIR)$(MANDIR)/man8/pkgman-update.8
	ln -sf pkgman-dep.1     $(DESTDIR)$(MANDIR)/man1/pkgman-rdep.1

uninstall:
	rm -f $(DESTDIR)$(BINDIR)/pkgman
	for d in 1 5 8; do \
		cd $(DESTDIR)$(MANDIR)/man$$d && \
		rm -f $(MANS) pkgman-update.8 pkgman-rdep.1; \
	done

clean:
	rm -f pkgman $(MANS) $(OBJS)

.PHONY: all install uninstall clean
