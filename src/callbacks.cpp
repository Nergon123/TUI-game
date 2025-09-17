#include "callbacks.h"

void message(void *param) {
	//horrible TODO Divide into functions
	if (param) {
		int a = 0;
		//int n = 0;
		for (char u = 255; u != '\0'; a++) u = ((const char *)(param))[a];//if (u == '\n')n++;
		system("clear");

		printf(WHITE "\n");

		for (int i = 0; i < (w.ws_col / 2 - a / 2)-1; i++) printf(" ");
		for (int i = 0; i < a+1; i++) printf("=");

		printf("\n");

		for (int i = 0; i < w.ws_col / 2 - a / 2; i++) printf(" ");

		printf("%s\n", (const char *)param);

		for (int i = 0; i < (w.ws_col / 2 - a / 2)-1; i++) printf(" ");
		for (int i = 0; i < a+1; i++) printf("=");

		//fgetc(stdin);
	}
}

void Death(void *a) {
	printf("gameover");
	exit(0);
}