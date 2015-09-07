/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clienttcp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalcim <jalcim@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/02/13 00:03:39 by jalcim            #+#    #+#             */
/*   Updated: 2014/06/13 21:12:57 by jalcim           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libsock/ft_inet.h"
#include "../libft/includes/libft.h"
#define PORT 25000
#define SIZE_FILENAME 256

int ft_cli_socktcp(t_sockaddr_in *sin, char *login);
void ft_commutateur(int sock, char **argv);

/*protocol :finaliter: eradication du protocol*/
//protocol :modif: envoi du nom d'utilisateur avant le mode
//protocol :c/x: envoi du mode, envoi du buffer
//protocol :f: envoi du mode, envoi du nom de fichier, envoi du buffer
//protocol :d: envoi du mode, envoi du nombre de fichier, (re:)envoi du nom de fichier, envoi du buffer(goto re;)

int main(int argc, char **argv)
{
	int sock;
	t_sockaddr_in *sin;
	char *user;

	if (argc != 4)
	{
		ft_putendl("error -> bad argument");
		exit(0);
	}
	sin = (t_sockaddr_in *)malloc(sizeof(t_sockaddr_in));
	ft_bzero(sin, sizeof(t_sockaddr_in));
	sock = ft_cli_socktcp(sin, argv[3]);

	errno = 0;
	if ((connect(sock, (t_sockaddr*)sin, sizeof(t_sockaddr))))
	{
		close(sock);
		error("connect -> ");
	}
	ft_putendl("connected");
	user = getenv("USER=");
	ft_putstr_fd(user, sock);
	write(sock, "\0", 1);
	ft_commutateur(sock, argv);
	ft_putendl("send");

	char buff;
	if (argv[1][0] == 'x')
	{
		while (!size_fd(sock))
			;
		while (read(sock, &buff, 1) && buff != '\0')
			write(1, &buff, 1);
	}
	close(sock);
	return (0);
}

void ft_commutateur(int sock, char **argv)
{
	char mode;
	char filename[SIZE_FILENAME] = {0};

	mode = argv[1][0];
	write(sock, &mode, 1);
	if (mode == 'f' || mode == 'd')
	{
		ft_strncpy(filename, argv[2], 255);
		if (mode == 'f')
			ft_send_file(sock, filename, 0, NULL);
		else
			ft_send_dir(sock, filename);
	}
	else if (mode == 'c' || mode == 'x')
	{
		ft_putstr_fd(argv[2], sock);
		write(sock, "\0", 1);
	}
	else
		printf("no mode %c bad argument\n", mode);
}

int ft_cli_socktcp(t_sockaddr_in *sin, char *login)
{
	int sock;
	char *ip;

	if ((sock = socket(PF_INET, SOCK_STREAM, 0)) == -1)
		error("socket -> ");
	if (!(ip = locate(login)))
	{
		printf("login invalide\n");
		exit(-1);
	}
	printf("%s : %s\n", login, ip);
	if (!(sin->sin_addr.s_addr = ft_inet_addr(ip)))
		error("inet error ->");
	sin->sin_family = PF_INET;
	sin->sin_port = ft_htons(PORT);

	ft_putendl("connect");
	return (sock);
}

void error(char *strerr)
{
	perror(strerr);
	exit(0);
}
