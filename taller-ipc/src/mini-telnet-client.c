#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "config.h"

int
main(int argc, char **argv)
{
	char *buf;
	struct sockaddr_in name, other;
	size_t bufsiz;
	ssize_t w;
	int s, c, rother;

	//chequea que la cantidad de argumentos sea la indicada
	if (argc != 2) {
		fprintf(stderr, "uso: %s ip\n", argv[0]);
		exit(1);
	}

	//crea el fucking socket
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	//setea la memoria del socket en cero
	memset(&name, 0, sizeof(name));

	//rutea el socket name a la ip pasada por parametro
	if (!inet_aton(argv[1], &name.sin_addr)) {
		fprintf(stderr, "%s: ip desconocida\n", argv[1]);
		exit(1);
	}

	//setea los demas valores del socket
	name.sin_family = AF_INET;
	name.sin_port = htons(PORT);

	if (connect(s, (struct sockaddr *)&name, sizeof(name)) == -1) {
		perror("connect");
		exit(1);
	}

	buf = NULL;
	bufsiz = 0;
	for (;;) {
		printf("[%s]> ", argv[1]);
		if ((w = getline(&buf, &bufsiz, stdin)) == -1) {
			if (!feof(stdin))
				perror("getline");
			break;
		}
		if (strncmp(buf, ENDMSG, w) == 0)
			break;
		if (send(s, buf, w, 0) == -1) {
			perror("send");
			exit(1);
		}

		char buffer[1024];
		int bytes;

		while ( bytes = recv(s, (void*) buffer, sizeof(buffer)-1, 0) ) {

				if(bytes == -1) {

					printf("What");
					perror("error on receiving");
					break;
				}

			if (strncmp(buffer, CMDSEP, 4) == 0)
			break;
			
			buffer[bytes-1] = '\0';
			
			fputs(buffer,stdout);

		} 

		printf("\n");
	}
	free(buf);
	close(s);

	return 0;
}
