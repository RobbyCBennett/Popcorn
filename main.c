#include "main.h"

void draw(DIR *dir, size_t focus) {
	// Clear screen
	#if _WIN32
		system("cls");
	#else
		erase();
	#endif

	// Print files
	size_t i = 0;
	struct dirent *file;
	rewinddir(dir);
	while ((file = readdir(dir)) != NULL) {
		printw("%c %s\n", (i == focus) ? '>' : ' ', file->d_name);
		i++;
	}
}

size_t countdir(DIR *dir) {
	// Get the file count
	size_t i = 0;
	struct dirent *file;
	rewinddir(dir);
	while ((file = readdir(dir)) != NULL)
		i++;
	return i;
}

DIR *cd(DIR *dir, size_t focus) {
	// Get the new directory
	size_t i = 0;
	struct dirent *file = NULL;
	rewinddir(dir);
	while ((file = readdir(dir)) != NULL && i != focus)
		i++;

	// Error: no new directory
	DIR *newdir;
	if (file == NULL || (newdir = opendir(file->d_name)) == NULL)
		return NULL;

	// Change the directory
	closedir(dir);
	chdir(file->d_name);
	return newdir;
}

void open(DIR *dir, size_t focus) {
	// Get the file
	size_t i = 0;
	struct dirent *file = NULL;
	rewinddir(dir);
	while ((file = readdir(dir)) != NULL && i != focus)
		i++;

	// Get the file name in quotes, with ./ on Linux
	char command[256];
	#if !(_WIN32)
		strcat(command, "./");
	#endif
	strcat(command, "\"");
	strcat(command, file->d_name);
	strcat(command, "\"");
	printw("%s\n", command);

	// Open the file
	system(command);
}

int main(int argc, char **argv) {
	// Error: Expected program and one argument
	if (argc != 2) {
		printf("Expected 1 folder name argument\n");
		return -1;
	}

	// Open root directory of media
	DIR *dir = opendir(argv[1]);
	chdir(argv[1]);
	size_t files = countdir(dir);

	// Initialize screen for UNIX
	#if !(_WIN32)
		initscr();
		noecho();
		raw();
		curs_set(0);
	#endif

	// Initial draw
	size_t focus = 0;
	draw(dir, focus);

	// Get user input and redraw
	char prev = -1;
	DIR *newdir;
	while (1) {
		const char curr = getch();
		switch (curr) {
			// Control C
			case 3:
				return 0;
			// Enter or space
			case 10:
			case 13:
			case 32:
				// Change directory
				newdir = cd(dir, focus);
				if (newdir != NULL) {
					dir = newdir;
					files = countdir(dir);
					focus = 0;
					draw(dir, focus);
				}
				// Open file
				else
					open(dir, focus);
				break;
			// Up
			case 65:
			case 72:
				if ((prev == 0 || prev == 91) && focus > 0) {
					focus--;
					draw(dir, focus);
				}
				break;
			// Down
			case 66:
			case 80:
				if ((prev == 0 || prev == 91) && focus < files - 1) {
					focus++;
					draw(dir, focus);
				}
				break;
		}

		prev = curr;
	}
}
