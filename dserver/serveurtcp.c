/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serveurtcp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalcim <jalcim@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/02/13 00:32:17 by jalcim            #+#    #+#             */
/*   Updated: 2014/06/11 14:16:56 by jalcim           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libsock/ft_inet.h"
#include "../libft/includes/libft.h"
#define PORT 25000
#define MAX_CLIENT 50
#define SIZE_FILENAME 256

/*
  protocol : http
  protocol : ftp
*/

t_server *recup(t_server *server);
void servershell(int pid)
{
	t_server *server;

	server = ft_serv_init();
	ft_socktcp(&server->sock, PORT, server->server);
	new_connect(server);
	recup(server);

	server->pid = 1;
	while (server->pid)
	  acceuil(server, pid);
}

void acceuil(t_server *server, int pid)
{
	char cmd;
	char *user;
	char *buffer;
	int nb_dir;

	ft_accept(server);
	server->connection++;
	user = ft_fd_in_str(server->sock);
	if (!(server->pid = fork()))
	{
		cmd = cmd_sock(server->sock);
		if (cmd == 'd' || cmd == 'f')
		{
			nb_dir = 0;
 			if (cmd == 'd')
				nb_dir = nb_dir_sock(server->sock);
			ft_recv_file(server->sock, nb_dir);
		}
		else if (cmd == 'c' || cmd == 'x')
		{
			buffer = ft_fd_in_str(server->sock);
			servcom(cmd, user, buffer, pid, server->sock);
		}//une suite de la connexion au lieu de la couper ?
		close(server->sock);
		exit(0);
 	}
}

char cmd_sock(int sock)
{
	char cmd;

	read(sock, &cmd, 1);
	return (cmd);
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

void ft_serv_end()
{
	int pid;

	pid = ft_pidsave(0);
	kill(pid, SIGINT);
    exit(0);
}

void error(char *strerr)
{
	perror(strerr);
	exit(0);
}

t_server *recup(t_server *server)
{
	static t_server *save = NULL;

	if (server)
		save = server;
	else
		return (save);
	return (NULL);
}
