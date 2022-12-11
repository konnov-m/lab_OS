#include <stdio.h>
#include <string.h>

int main() {
	const char *name_channel_sound="Exit\n";
	const char *name ="Exit";
	if (memcmp(name_channel_sound, name, 1) == 0) {
		printf("Equals\n");
	}
}
