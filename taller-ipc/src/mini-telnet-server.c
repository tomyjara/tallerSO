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
	char buf[MSGLEN + 1];
	struct sockaddr_in local, remote;
	ssize_t r;
	int c, remotelen, s;

	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == -1) {//crea un socket el cual acepta direcciones afinet
		//y retorna un file_descriptor(s en nuestro caso)
		perror("socket");
		exit(1);
	}

	memset(&local, 0, sizeof(local));//setea en cero la memoria referenciada por local
	//inicializa el struct local
	local.sin_family = AF_INET;
	local.sin_addr.s_addr = INADDR_ANY;
	local.sin_port = htons(PORT);
	//bindea el socket s con el struct local
	if (bind(s, (struct sockaddr *)&local, sizeof(local)) == -1) {
		perror("bind");
		exit(1);
	}

	//pone el socket a escuchar... tanto les cuesta comentar el código?
	if (listen(s, 10) == -1) {
		perror("listen");
		exit(1);
	}

	remotelen = sizeof(remote);
	if ((c = accept(s, (struct sockaddr *)&remote, (socklen_t *)&remotelen))
	    == -1) {
		perror("accept");
		exit(1);
	}

	


	for (;;) {
		r = recv(c, buf, sizeof(buf) - 1, 0);
		if (r == -1) {
			perror("recv");
			continue;
		}
		buf[r] = '\0';
		printf("comando: %s", buf);
	
		dup2(c, STDOUT_FILENO); //Redirecciono stdout
		dup2(c, STDERR_FILENO); //Redirecciono stderr

		/* ejecuta el comando */
		(void)system(buf);

		printf(CMDSEP);

	}
	close(c);
	close(s);

	return 0;
}
