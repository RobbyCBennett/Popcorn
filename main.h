#pragma once

#include <stdio.h>
#include <stdlib.h>

#if __has_include(<ncurses.h>)
	#include <ncurses.h>
#else
	#include "lib/ncurses.h"
#endif

#if __has_include(<dirent.h>)
	#include <dirent.h>
#else
	#include "lib/dirent.h"
#endif

#if __has_include(<unistd.h>)
	#include <unistd.h>
#else
	#include "lib/unistd.h"
#endif

