#include "main.h"

void draw(DIR *dir, size_t focus) {
	// Clear screen
	#if _WIN32
		system("cls");
	#else
		system("clear");
	#endif

	// Print files
	size_t i = 0;
	struct dirent *file;
	rewinddir(dir);
	while ((file = readdir(dir)) != NULL) {
		printf("%c %s\n", (i == focus) ? '>' : ' ', file->d_name);
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
			// Enter
			case 13:
				newdir = cd(dir, focus);
				if (newdir != NULL) {
					dir = newdir;
					files = countdir(dir);
					focus = 0;
					draw(dir, focus);
				}
				break;
			// Up
			case 72:
				if (prev == 0 && focus > 0) {
					focus--;
					draw(dir, focus);
				}
				break;
			// Down
			case 80:
				if (prev == 0 && focus < files - 1) {
					focus++;
					draw(dir, focus);
				}
				break;
		}

		prev = curr;
	}
}
