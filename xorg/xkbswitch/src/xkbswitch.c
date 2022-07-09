#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <err.h>
#include <X11/Xlib.h>
#include <X11/XKBlib.h>

int main(int argc, char **argv)
{
	int xkbEventType, xkbError, xkbReason;
	int mjr = XkbMajorVersion, mnr = XkbMinorVersion;
	Display *display = NULL;
	XkbStateRec state;
	Bool status = True;

	display = XkbOpenDisplay(NULL, &xkbEventType, &xkbError, &mjr, &mnr,
				 &xkbReason);
	if (NULL == display) {
		warnx("Cannot open X display %s", XDisplayName(NULL));
		switch (xkbReason) {
		case XkbOD_BadServerVersion:
			warnx("Bad X11 server version");
			break;
		case XkbOD_BadLibraryVersion:
			warnx("Bad XKB library version");
			break;
		case XkbOD_ConnectionRefused:
			warnx("Connection to X server refused");
			break;
		case XkbOD_NonXkbServer:
			warnx("XKB extension is not present");
			break;
		default:
			warnx("Unknown error from XkbOpenDisplay: %d",
			      xkbReason);
			break;
		}
		exit(1);
	}

	if (argc == 1) {
		/* Get layout group index in [0..3] */
		status = XkbGetState(display, XkbUseCoreKbd, &state);

		printf("%d\n", state.group);
	} else if (argc == 2) {
		/* Set layout or print usage msg if arg is not in [0..3] */
		if (strlen(argv[1]) > 1 || argv[1][0] < 48 || argv[1][0] > 51)
			goto usage;

		status = XkbLockGroup(display, XkbUseCoreKbd, atoi(argv[1]));
	} else {
 usage:
		puts("xkbswitch - set/get current layout group index in [0..3]\n"
		     "usage: xkbswitch [0..3]\n"
		     "If no args then just prints current layout group index\n");
	}

	XCloseDisplay(display);

	return status;
}
