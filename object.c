#include "object.h"

t_object *init()
{
  t_object *object;

  init_bdd();
  object = (t_object *)malloc(sizeof(t_object));
  object->infolog = init_client();
  object->request_server = ft_request;
  object->request_bdd = request_bdd;
  object->destruct = destruct;
  return (object);
}

void destruct(t_object *object)
{
  int *pid;

  //  ft_request(NULL, -1, NULL);
  //  dez_pipe(NULL, NULL);
  //  free(object);

  close_client(object->infolog);
  close(ft_write_req_pipe(-1));
  close(ft_read_req_pipe(-1));
  pid = ft_recup_pid(NULL);
  kill(pid[0], SIGINT);
  free(pid);
}

t_json *request_bdd_verif_extract(t_object *object, char *id)
{
  t_json *json = NULL;
  int fd;
  char *strproj;

  system("pwd;ls -l reg");
  if ((fd = open("reg/tab_id", O_RDONLY)) != -1)
    {
      strproj = ft_readfd(fd, size_fd(fd));
      ft_putendl_fd(strproj, 2);
      ft_putendl_fd(id, 2);
      if (ft_strstr(strproj, id))
	{
	  if ((json = m_object_request_bdd_extract(id)))//demande d'un fichier a la bdd
	    write_elem_json(json, 2);
	}
      else
	ft_putendl_fd("bha nan haha 1", 2);
      close(fd);
    }
  else
    ft_putendl_fd("bha nan haha 2", 2);
  return (json);
}

t_json *request_bdd_verif_del(t_object *object, char *id)
{
  t_json *json = NULL;
  int fd;
  char *strproj;

  if ((fd = open("reg/tab_id", O_RDONLY)) != -1)
    {
      strproj = ft_readfd(fd, size_fd(fd));
      if (ft_strstr(strproj, id))
	  m_object_request_bdd_del(id);//demande d'un fichier a la bdd
      else
	ft_putendl_fd("bha nan haha", 2);
      close(fd);
    }
  else
    ft_putendl_fd("bha nan haha", 2);
  return (json);
}

t_json *request_bdd_verif_id(t_object *object)
{
  //  int fd;

  
  return (m_object_request_bdd_id(" "));//demande d'un fichier a la bdd
  //  return (NULL);
}

//int object()
int main()
{
  t_object *object;
  char str[100];

  //lancement des processur initialisation de l'objet et de ses methodes/structures
  m_object_init;//constructeur object

  //extraction sans synchronisation
  ft_putendl_fd("extract OBJECT 1", 2);
  scanf("%s", str);
  object->json = m_request_bdd_verif_extract(object, str);



  //mise a jour de la bdd (connection serveur/requette http/post)
  ft_putendl_fd("MISE A JOUR DE LA BDD", 2);
  m_object_request_server(2);//requette au serveur (2)==(reception json)
  m_object_request_bdd_send(" ");//envoi a la bdd de json




  //mise a jour des tab_id
  ft_putendl_fd("id OBJECT", 2);

  //2 eme generation
  m_request_bdd_verif_id(object);

  //1 ere generation
  //object->id = m_object_request_bdd_id(" ");//demande a la bdd des ids client
  //  write_elem_json(object->id, 2);//ecriture sur la sortie d'erreur




  ft_putendl_fd("extract OBJECT 2", 2);
  scanf("%s", str);

  //2 eme generation
  m_request_bdd_verif_extract(object, str);

  //1 ere generation
  //  object->json = m_object_request_bdd_extract(str);//demande d'un fichier a la bdd
  //  write_elem_json(object->json, 2);




  ft_putendl_fd("del OBJECT", 2);
  scanf("%s", str);

  //2 eme generation
  m_request_bdd_verif_del(object, str);

  //1 ere generation
  //  m_object_request_bdd_del(str);//destruction d'un fichier dans la bdd

  ft_putendl_fd("end", 2);
  m_object_destruct;
  return (0);
}
