#include "libft.h"

#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <poll.h>
#include <dirent.h>
#include <signal.h>

typedef struct dirent t_dirent;
typedef struct sockaddr_in t_sockaddr_in;
typedef struct in_addr t_in_addr;
typedef struct pollfd t_pollfd;
typedef struct sockaddr t_sockaddr;
typedef struct s_aton t_aton;
typedef struct s_server t_server;
typedef struct s_client t_client;

struct s_server
{
  int pid;
  int connection;
  t_sockaddr_in *server;
  int sock;
  t_client *list;
  t_client *client;
};

struct s_client
{
  int port;
  t_sockaddr_in *info;
  t_pollfd *event;
  t_client *next;
};

struct	s_aton
{
  u_long		parts[4];
  in_addr_t	val;
  char		*c;
  char		*endptr;
  int			gotend;
  int			n;
};

int main()//void servershell(int pid)
{
	t_server *server;

	server = ft_serv_init();
	ft_socktcp(&server->sock, PORT, server->server);

	acceuil(server);
}

void acceuil(t_server *server)
{
	new_connect(server);
	ft_accept(server);
	server->connection++;
	//lecture sur socket de la requette http
	close(server->sock);
}


t_server *ft_serv_init()
{
  t_server *server;

  if (!(server = (t_server *)malloc(sizeof(t_server))))
    error("serv_init -> ");
  ft_bzero(server, sizeof(t_server));
  server->server = (t_sockaddr_in *)malloc(sizeof(t_sockaddr_in));
  ft_bzero(server->server, sizeof(t_sockaddr_in));
  server->list = NULL;
  server->connection = 0;

  return (server);
}

void ft_socktcp(int *sock, int port, t_sockaddr_in *serveur)
{
    if ((*sock = socket(PF_INET, SOCK_STREAM, 0)) == -1)
        error("socket -> ");
    serveur->sin_family = PF_INET;
    serveur->sin_port = htons(port);
	errno = 0;
    if ((bind(*sock, (t_sockaddr *)serveur, sizeof(t_sockaddr))) == -1)
	{ 
		if (EACCES == errno)
			ft_putendl("L'adresse est protégée et l'utilisateur n'est pas le superutilisateur");
		error("bind -> ");
	}
}

void new_connect(t_server *server)
{
	wait_connect(server);
	server->connection++;
}

void wait_connect(t_server *server)
{
	server->client = (t_client *)malloc(sizeof(t_client));
	bzero(server->client, sizeof(t_client));
	server->client->event = (t_pollfd *)malloc(sizeof(t_pollfd));
	ft_waitsocktcp(server->sock, server->client->event, 1, 1);
}

void ft_waitsocktcp(int sock, t_pollfd *event, int size_fille, int time)
{
    event->fd = sock;
    event->events = POLLIN;
    if ((poll(event, sizeof(t_pollfd), time)) == -1)
        error("poll -> ");
    if (listen(event[0].fd, size_fille) == -1)
        error("listen -> ");
}

void ft_accept(t_server *server)
{
	socklen_t size;

	size = sizeof(t_sockaddr);
	server->client->info = (t_sockaddr_in *)malloc(size);
	bzero(server->client->info, size);

	server->client->next = server->list;
	server->list = server->client;

	if ((server->sock = accept(server->client->event->fd, (t_sockaddr *)server->client->info, &size)) == -1)
		error("accept -> ");
}
