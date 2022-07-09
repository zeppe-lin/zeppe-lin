/* Modify this file to change what commands output to your statusbar, and recompile using the make command. */
static const Block blocks[] = {
	/* Icon */	/* Command */				/* Update Interval */	/* Update Signal */
	{" ",		"/usr/share/dwmblocks/batt.sh",				2,			0},
	{"",		"/usr/share/dwmblocks/audiovol.sh",			2,			0},
	{"\uf85a ",	"/usr/share/dwmblocks/cpumem.sh",			2,			0},
	{"\uf7c9 ",	"/usr/share/dwmblocks/diskfree.sh",			15,			0},
	{"\uf5ef ",	"date '+%b %d (%a) %H:%M'",				5,			0},
};

/* Sets delimeter between status commands. NULL character ('\0') means no delimeter. */
static char delim[] = " | ";
static unsigned int delimLen = 5;

/* End of file. */
