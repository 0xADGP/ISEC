// E suposto guardarmos os topicos que o utlizador está inscrito quando ele da logout e da login novamente?
// R: NAO

// Quando um user é removido as mensagens dele e subscricoes sao removidas?
// R: NAO

// Quando o ficheiro não existe é suposto abortar ou criar um ficheiro temporario? E nao a variavel nao está setada?
// R: DEFINIR

// Quando chegamos ao limite de mensagens persistentes e um user envia uma mensagem "persistente" é suposto os users receberem à mesma ou descartar por completo?
// R: NAO RECEBEM

// Podemos usar arrays bidimensionais?
// R: SIM

// Quando um username sai do sistema a user cimando "fim" os outros recebem notificacao?
// R: NAO

// QUando o username passa o limite cortamos o resto e aceitamos ou nao deixamos ligar?
// R: DECISAO

// Na funcao update_topics() estamos a tirar os que nao tem nenhum subscritor, mas é suposto o topic continuar a existir caso exista mensagem nele?
// R:

// Funcao void delete_tmp(); apagar conteudos quando os pipes estao abertos
// R:

// O corpo da mensagem pode ter até 300 carateres, podendo, no entanto, ser significativamente menor. Uma solução que envie apenas a parte (os caracteres) efetivamente usada pela mensagem, evitando o envio de caracteres não
// usados, terá uma avaliação superior às soluções que simplesmente enviam mensagens sempre do tamanho máximo
// R: INTEIRO COM COMPRIMENTO E MENSAGEM

// No comando checkin e logout podemos assumir que o utilizador apenas executa 1 vez ou temos de adicionar verificacao para cada um dos comandos?
// R: SE VOLTAR A FAZER CHECKIN JA ESTA LOGADO, ADICIONAR PID

// When it comes to manipulating system resources, priority should be given to using calls to the operating system rather than library functions (for example, read() and write() should be used instead of fread() and fwrite())
// <->
// We suggest using C library functions (fread, fgets, etc.) rather than system functions (read, write) for the issue of saving and retrieving persistent messages as they simplify these tasks.
// R: PIPES -> READ/WRITE, FREAD PARA APENAS FICHEIRO

// É preciso usar threads? Caso seja, em que situaçoes é que temos de as usar?
// R: NAO É OBRIGATORIO

/*
  _   _  ____ _______        _____
 | \ | |/ __ \__   __|/\    / ____|
 |  \| | |  | | | |  /  \  | (___
 | . ` | |  | | | | / /\ \  \___ \
 | |\  | |__| | | |/ ____ \ ____) |
 |_| \_|\____/  |_/_/    \_\_____/

THREADS UPDATE -> SÓ SAIR DO PROGRAMA QUANDO AS THREADS SAIREM!!!!!!
Cada thread tem de ter o exit
phread_join, esperar pelo fim da thread
exit -> termina as threads?


QUANDO O USER USA MSG TBM SE SUBSCREVE AO TOPICO OU SO CRIA?
- DEIXADO EM ABERTO
*/

#include "manager.h"

/*int main(int argc, char *argv[])
{
    if (argc != 1)
    {
        fprintf(stderr, "[SERVER] Uso: %s\n", argv[0]);
        return 0;
    }

    ServerData sd;

    //setup_signals();
    start_server(&sd);
    return 0;
}*/

/* Main Server Functions */

/*void setup_signals()
{
    signal(SIGINT, handle_sigint);
    // signal(SIGALRM, handle_sigalrm);
}*/

/*void start_server(ServerData *sd)
{
    if (access(FIFO_SRV, F_OK) == 0)
    {
        printf("[SERVER] Ja existe um servidor a correr!\n");
        exit(3);
    }

    g_DEBUG ? printf("[SERVER] Servidor a iniciar\n") : (void)0;

    mkfifo(FIFO_SRV, 0600);
    sd->fd_srv = open(FIFO_SRV, O_RDWR);

    // create_tmp();

    load_messages_from_file(sd);

    // alarm(1);

    //mkfifo(FIFO_SRV, 0600);
    //fd_srv = open(FIFO_SRV, O_RDWR);

    fd_set read_fds;
    struct timeval timeout;

    pthread_t tid;
    TALARM ta;
    ta.ligado = 1;
    ta.sd = sd;
    pthread_create(&tid, NULL, doAlarm, (void *)&ta);

    Pedido p;

    while (1)
    {
        FD_ZERO(&read_fds);
        FD_SET(sd->fd_srv, &read_fds);
        FD_SET(STDIN_FILENO, &read_fds);

        timeout.tv_sec = 1;
        timeout.tv_usec = 0;

        int activity;
        do
        {
            activity = select(sd->fd_srv + 1, &read_fds, NULL, NULL, &timeout);
        } while (activity < 0 && errno == EINTR);

        if (FD_ISSET(sd->fd_srv, &read_fds))
        {
            int res = read(sd->fd_srv, &p, sizeof(Pedido));
            if (res == sizeof(Pedido))
            {
                process_client_request(&p, sd);
            }
        }

        if (FD_ISSET(STDIN_FILENO, &read_fds))
        {
            handle_manager_commands(sd);
        }
    }

    ta.ligado = 0;
    pthread_join(tid, NULL);
}*/

int main(int argc, char * argv[]) {
  if (argc != 1) {
    fprintf(stderr, "[SERVER] Uso: %s\n", argv[0]);
    return 0;
  }

  if (access(FIFO_SRV, F_OK) == 0) {
    printf("[SERVER] Já existe um servidor a correr!\n");
    return 3;
  }

  g_DEBUG ? printf("[SERVER] Servidor a iniciar\n") : (void) 0;

  int running = 1;
  ServerData sd;

  if (load_messages_from_file( & sd)) {
    pthread_t client_thread, alarm_thread;

    pthread_mutex_init( & sd.lock, NULL);

    signal(SIGINT, handle_sigint);

    mkfifo(FIFO_SRV, 0600);
    sd.fd_srv = open(FIFO_SRV, O_RDWR | O_NONBLOCK);

    ClientRequest client_data;
    client_data.running = & running;
    client_data.sd = & sd;
    pthread_create( & client_thread, NULL, getClientRequest, & client_data);

    TALARM alarm_data;
    alarm_data.running = & running;
    alarm_data.sd = & sd;
    pthread_create( & alarm_thread, NULL, doAlarm, & alarm_data);

    while (handle_manager_commands( & sd))
    ;

    running = 0;

    pthread_join(client_thread, NULL);
    pthread_join(alarm_thread, NULL);

    for (int i = sd.feed_count - 1; i >= 0; i--) {
      char username_copy[MAX_USERNAME_LENGHT + 1];
      strncpy(username_copy, sd.feed_usernames[i], MAX_USERNAME_LENGHT + 1);
      username_copy[MAX_USERNAME_LENGHT] = '\0';

      handle_user_removal(username_copy, 0, & sd);
    }

    save_messages_to_file( & sd);

    close(sd.fd_srv);
    unlink(FIFO_SRV);
    pthread_mutex_destroy( & sd.lock);
  }

  g_DEBUG ? printf("[SERVER] Servidor encerrado\n") : (void) 0;

  return 0;
}

void * doAlarm(void * pData) {
  TALARM * pTa = (TALARM * ) pData;
  int * running = pTa -> running;
  while ( * running) {
    pthread_mutex_lock( & pTa -> sd -> lock);
    update_message_duration(pTa -> sd);
    update_topics(pTa -> sd);
    pthread_mutex_unlock( & pTa -> sd -> lock);

    sleep(1);
  }
  g_DEBUG ? printf("[SERVER] Thread 'alarm' a sair\n") : (void) 0;
  pthread_exit(NULL);
}

void * getClientRequest(void * pData) {
  ClientRequest * pTa = (ClientRequest * ) pData;
  ServerData * sd = pTa -> sd;
  int * running = pTa -> running;

  Pedido p;

  while ( * running) {
    int res = read(sd -> fd_srv, & p, sizeof(Pedido));
    if (res == sizeof(Pedido)) {
      process_client_request( & p, sd);
    } else if (res == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
      usleep(10000);
    } else if (res < 0) {
      perror("[SERVER] Erro leitura");
    }
  }

  g_DEBUG ? printf("[SERVER] Thread 'clientRequest' a sair\n") : (void) 0;
  pthread_exit(NULL);
}

void handle_sigint(int sig) {
  g_DEBUG ? printf("\n[SERVER] Escreva 'close' para fechar o manager\n") : (void) 0;

  // cleanup_and_exit();
}

/*void handle_sigalrm(int sig)
{
    update_message_duration();
    update_topics();
    alarm(1);
}*/

int handle_manager_commands(ServerData * sd) {
  char cmd[256];

  // pthread_mutex_lock(&sd->lock);
  if (fgets(cmd, sizeof(cmd), stdin) != NULL) {
    cmd[strcspn(cmd, "\n")] = 0;

    if (strlen(cmd) == 0) {
      g_DEBUG ? printf("[MANAGER] Comando vazio ignorado\n") : (void) 0;
      return 1;
    }

    if (strcmp(cmd, "users") == 0) {
      pthread_mutex_lock( & sd -> lock);
      display_connected_users(sd);
      pthread_mutex_unlock( & sd -> lock);
    } else if (strncmp(cmd, "remove ", 7) == 0) {
      char username[MAX_USERNAME_LENGHT];
      sscanf(cmd + 7, "%s", username);

      pthread_mutex_lock( & sd -> lock);
      handle_user_removal(username, 1, sd);
      pthread_mutex_unlock( & sd -> lock);
    } else if (strcmp(cmd, "topics") == 0) {
      pthread_mutex_lock( & sd -> lock);
      list_topics(sd);
      pthread_mutex_unlock( & sd -> lock);
    } else if (strncmp(cmd, "show ", 5) == 0) {
      char topic_name[20];
      sscanf(cmd + 5, "%s", topic_name);

      pthread_mutex_lock( & sd -> lock);
      Topic * topic = find_or_create_topic(topic_name, 0, sd);
      show_topic_messages(topic);
      pthread_mutex_unlock( & sd -> lock);
    } else if (strncmp(cmd, "lock ", 5) == 0) {
      char topic_name[20];
      sscanf(cmd + 5, "%s", topic_name);

      pthread_mutex_lock( & sd -> lock);
      Topic * topic = find_or_create_topic(topic_name, 0, sd);
      lock_topic(topic);
      pthread_mutex_unlock( & sd -> lock);
    } else if (strncmp(cmd, "unlock ", 7) == 0) {
      char topic_name[20];
      sscanf(cmd + 7, "%s", topic_name);

      pthread_mutex_lock( & sd -> lock);
      Topic * topic = find_or_create_topic(topic_name, 0, sd);
      unlock_topic(topic);
      pthread_mutex_unlock( & sd -> lock);
    } else if (strcmp(cmd, "close") == 0) {
      g_DEBUG ? printf("[SERVER] Manager a encerrar\n") : (void) 0;
      // pthread_mutex_unlock(&sd->lock);
      return 0;
      // cleanup_and_exit(sd);
    } else if (strcmp(cmd, "debug") == 0 && g_DEBUG) {
      pthread_mutex_lock( & sd -> lock);
      debug_server_state(sd);
      pthread_mutex_unlock( & sd -> lock);
    } else {
      printf("[SERVER] Comando nao reconhecido: '%s'\n", cmd);
    }
  }

  // pthread_mutex_unlock(&sd->lock);
  return 1;
}

/* User-Related Functions */

int is_pid_duplicate(const int pid, ServerData * sd) {
  for (int i = 0; i < sd -> feed_count; i++) {
    if (sd -> feed_pids[i] == pid) {
      return 1;
    }
  }
  return 0;
}

int is_username_duplicate(const char * username, ServerData * sd) {
  for (int i = 0; i < sd -> feed_count; i++) {
    if (strcmp(sd -> feed_usernames[i], username) == 0) {
      return 1;
    }
  }
  return 0;
}

int remove_username(const char * username, ServerData * sd) {
  int pid_to_remove = 0;
  int user_index_to_remove = -1;

  for (int i = 0; i < sd -> feed_count; i++) {
    if (strcmp(sd -> feed_usernames[i], username) == 0) {
      pid_to_remove = sd -> feed_pids[i];
      user_index_to_remove = i;

      char remove[MAX_USERNAME_LENGHT];
      sprintf(remove, FIFO_CLI, pid_to_remove);
      send_response_to_client(remove, "[SERVER] Voce foi removido do servidor");

      for (int j = i; j < sd -> feed_count - 1; j++) {
        strcpy(sd -> feed_usernames[j], sd -> feed_usernames[j + 1]);
        sd -> feed_pids[j] = sd -> feed_pids[j + 1];
      }

      memset(sd -> feed_usernames[sd -> feed_count - 1], 0, sizeof(sd -> feed_usernames[sd -> feed_count - 1]));
      sd -> feed_pids[sd -> feed_count - 1] = 0;
      sd -> feed_count--;
      break;
    }
  }

  if (user_index_to_remove != -1) {
    for (int i = 0; i < sd -> topic_count; i++) {
      for (int j = 0; j < sd -> topics[i].subscriber_count; j++) {
        if (sd -> topics[i].subscribers[j] == user_index_to_remove) {
          for (int k = j; k < sd -> topics[i].subscriber_count - 1; k++) {
            sd -> topics[i].subscribers[k] = sd -> topics[i].subscribers[k + 1];
          }

          sd -> topics[i].subscriber_count--;
          printf("[SERVER] Utilizador '%s' removido do topico '%s'\n", username, sd -> topics[i].topic_name);
          break;
        }
      }
    }
  }

  return pid_to_remove;
}

int get_user_index(const char * username, ServerData * sd) {
  for (int i = 0; i < sd -> feed_count; i++) {
    if (strcmp(sd -> feed_usernames[i], username) == 0) {
      return i;
    }
  }
  return -1;
}

void handle_user_removal(const char * username, int notify_users, ServerData * sd) {
  int removed_pid = remove_username(username, sd);

  if (removed_pid > 0) {
    char fifo[30];
    sprintf(fifo, FIFO_CLI, removed_pid);
    send_response_to_client(fifo, FEED_EXIT_CODE);
    printf("[SERVER] Utilizador '%s' removido. PID: %d\n", username, removed_pid);

    if (notify_users == 1) {
      char message[256];
      snprintf(message, sizeof(message), "[SERVER] Utilizador '%s' removido", username);
      notify_all_users(message, sd);
    }
  } else {
    printf("[SERVER] Utilizador '%s' nao encontrado\n", username);
  }
}

void display_connected_users(ServerData * sd) {
  if (sd -> feed_count == 0) {
    printf("[SERVER] Nenhum utilizador conectado\n");
  } else {
    printf("[SERVER] Utilizadores Conectados (%d/%d):\n", sd -> feed_count, MAX_USERS);
    for (int i = 0; i < sd -> feed_count; i++) {
      printf(" - Username: %s | PID: %d\n", sd -> feed_usernames[i], sd -> feed_pids[i]);
    }
  }
}

void process_client_request(Pedido * p, ServerData * sd) {
  char fifo[30];
  sprintf(fifo, FIFO_CLI, p -> pid);

  // pthread_mutex_lock(&sd->lock);
  if (strcmp(p -> str, "CHECKIN") == 0) {
    g_DEBUG ? printf("[SERVER] A fazer CHECKIN para '%s' (PID: %d)\n", p -> username, p -> pid) : (void) 0;

    pthread_mutex_lock( & sd -> lock);
    if (is_username_duplicate(p -> username, sd)) {
      g_DEBUG ? printf("[SERVER] Username '%s' ja esta em uso\n", p -> username) : (void) 0;
      send_response_to_client(fifo, ERROR_CODE_USER_EXISTS);
    } else if (sd -> feed_count >= MAX_USERS) {
      g_DEBUG ? printf("[SERVER] Servidor esta cheio. Nao e possivel adicionar '%s'.\n", p -> username) : (void) 0;
      send_response_to_client(fifo, ERROR_CODE_SERVER_FULL);
    } else {
      strcpy(sd -> feed_usernames[sd -> feed_count], p -> username);
      sd -> feed_pids[sd -> feed_count++] = p -> pid;

      g_DEBUG ? printf("[SERVER] User '%s' (PID: %d) adicionado\n", p -> username, p -> pid) : (void) 0;
      send_response_to_client(fifo, "OK");
    }
    pthread_mutex_unlock( & sd -> lock);
  } else if (strcmp(p -> str, "LOGOUT") == 0) {
    pthread_mutex_lock( & sd -> lock);
    handle_user_removal(p -> username, 0, sd);
    pthread_mutex_unlock( & sd -> lock);
  } else if (strcmp(p -> str, "topics") == 0) {
    pthread_mutex_lock( & sd -> lock);
    if (sd -> topic_count == 0) {
      send_response_to_client(fifo, "[SERVER] Nenhum topico criado");
    } else {
      char response[BUFFER_SIZE];
      snprintf(response, sizeof(response), "[SERVER] Lista de Topicos:\n");

      for (int i = 0; i < sd -> topic_count; i++) {
        snprintf(response + strlen(response), sizeof(response) - strlen(response), "Topico: %s | Mensagens: %d/%d | Estado: %s\n", sd -> topics[i].topic_name, sd -> topics[i].message_count, MAX_PERSISTENT_MESSAGES, sd -> topics[i].locked ? "Bloqueado" : "Desbloqueado");
      }

      send_response_to_client(fifo, response);
    }
    pthread_mutex_unlock( & sd -> lock);
  } else if (strncmp(p -> str, "msg ", 4) == 0) {
    char topic_name[20];
    int duration;
    char message_body[MAX_MSG_LENGTH + 1];

    if (sscanf(p -> str + 4, "%s %d %[^\n]", topic_name, & duration, message_body) != 3) {
      send_response_to_client(fifo, "[SERVER] Comando 'msg' invalido. Uso: msg <nome_topico> <duracao> <corpo>");
      return;
    }

    if (strlen(message_body) > MAX_MSG_LENGTH) {
      send_response_to_client(fifo, "[SERVER] Mensagem muito longa. O corpo da mensagem deve ter no maximo 300 caracteres");
      return;
    }

    Message new_msg;
    strncpy(new_msg.username, p -> username, MAX_USERNAME_LENGHT + 1);
    new_msg.username[MAX_USERNAME_LENGHT] = '\0';
    new_msg.duration = duration;
    strncpy(new_msg.body, message_body, MAX_MSG_LENGTH - 1);
    new_msg.body[MAX_MSG_LENGTH - 1] = '\0';

    pthread_mutex_lock( & sd -> lock);
    Topic * topic = find_or_create_topic(topic_name, 1, sd);
    if (!topic) {
      send_response_to_client(fifo, "[SERVER] Erro ao encontrar ou criar o topico");
      pthread_mutex_unlock( & sd -> lock);
      return;
    }

    if (topic -> locked) {
      send_response_to_client(fifo, "[SERVER] O topico esta bloqueado. Nao e possível adicionar mensagens");
      pthread_mutex_unlock( & sd -> lock);
      return;
    }

    send_message_to_subscribers(topic, & new_msg, sd);

    if (duration > 0) {
      add_message_to_topic(topic, & new_msg);
    }

    send_response_to_client(fifo, "Mensagem enviada com sucesso");
    pthread_mutex_unlock( & sd -> lock);

    g_DEBUG ? printf("[SERVER] Mensagem adicionada ao topico '%s' por '%s' (Duração: %d, Corpo: '%s')\n", topic_name, new_msg.username, new_msg.duration, new_msg.body) : (void) 0;
  } else if (strncmp(p -> str, "subscribe ", 10) == 0) {
    char topic_name[20];
    sscanf(p -> str + 10, "%s", topic_name);

    pthread_mutex_lock( & sd -> lock);
    Topic * topic = find_or_create_topic(topic_name, 1, sd);
    if (!topic) {
      send_response_to_client(fifo, "[SERVER] Erro ao encontrar ou criar o topico");
      pthread_mutex_unlock( & sd -> lock);
      return;
    }
    subscribe_to_topic(p -> username, topic, sd);
    pthread_mutex_unlock( & sd -> lock);
  } else if (strncmp(p -> str, "unsubscribe ", 12) == 0) {
    char topic_name[20];
    sscanf(p -> str + 12, "%s", topic_name);

    pthread_mutex_lock( & sd -> lock);
    Topic * topic = find_or_create_topic(topic_name, 0, sd);
    if (!topic) {
      send_response_to_client(fifo, "[SERVER] Erro ao encontrar o topico");
      pthread_mutex_unlock( & sd -> lock);
      return;
    }
    unsubscribe_from_topic(p -> username, topic, sd);
    pthread_mutex_unlock( & sd -> lock);
  } else if (strcmp(p -> str, "exit") == 0) {
    pthread_mutex_lock( & sd -> lock);
    handle_user_removal(p -> username, 0, sd);
    pthread_mutex_unlock( & sd -> lock);
  } else {
    g_DEBUG ? printf("[SERVER] Recebido: '%s' de PID: %d\n", p -> str, p -> pid) : (void) 0;
    send_response_to_client(fifo, "[SERVER] Comando desconhecido");
  }
  // pthread_mutex_unlock(&sd->lock);
}

int send_response_to_client(const char * fifo,
  const char * response) {
  Resposta r;
  strncpy(r.str, response, sizeof(r.str) - 1);
  r.str[sizeof(r.str) - 1] = '\0';

  int fd_cli = open(fifo, O_WRONLY);
  if (fd_cli > 0) {
    write(fd_cli, & r, sizeof(Resposta));
    close(fd_cli);
    return 1;
  }
  return 0;
}

int subscribe_to_topic(const char * username, Topic * topic, ServerData * sd) {
  if (!username || !topic) {
    printf("[SERVER] Username ou topico invalidos\n");
    return -1;
  }

  int user_index = get_user_index(username, sd);
  if (user_index == -1) {
    printf("[SERVER] Utilizador '%s' nao encontrado\n", username);
    return -1;
  }

  char fifo[30];
  sprintf(fifo, FIFO_CLI, sd -> feed_pids[user_index]);

  for (int j = 0; j < topic -> subscriber_count; j++) {
    if (strcmp(sd -> feed_usernames[topic -> subscribers[j]], username) == 0) {
      send_response_to_client(fifo, "[SERVER] Voce ja esta subscrito no topico");
      return -1;
    }
  }

  if (topic -> subscriber_count < MAX_USERS) {
    topic -> subscribers[topic -> subscriber_count++] = user_index;

    send_response_to_client(fifo, "[SERVER] Subscricao no topico realizada com sucesso");

    for (int k = 0; k < topic -> message_count; k++) {
      Message * msg = & topic -> messages[k];
      char message_str[MAX_MSG_LENGTH + 100];
      snprintf(message_str, sizeof(message_str), "[%s] De: %s\nCorpo: %s\n", topic -> topic_name, msg -> username, msg -> body);

      send_response_to_client(fifo, message_str);
    }

    return 0;
  } else {
    send_response_to_client(fifo, "[SERVER] Topico chegou ao maximo de subscritores");
    return -1;
  }
}

int unsubscribe_from_topic(const char * username, Topic * topic, ServerData * sd) {
  if (!username || !topic) {
    printf("[SERVER] Username ou topico invalidos\n");
    return -1;
  }

  int user_index = get_user_index(username, sd);
  if (user_index == -1) {
    printf("[SERVER] Utilizador '%s' nao encontrado\n", username);
    return -1;
  }

  char fifo[30];
  sprintf(fifo, FIFO_CLI, sd -> feed_pids[user_index]);

  for (int j = 0; j < topic -> subscriber_count; j++) {
    if (topic -> subscribers[j] == user_index) {
      for (int k = j; k < topic -> subscriber_count - 1; k++) {
        topic -> subscribers[k] = topic -> subscribers[k + 1];
      }
      topic -> subscriber_count--;

      send_response_to_client(fifo, "[SERVER] Unsubscricao do topico realizada com sucesso");
      return 0;
    }
  }

  send_response_to_client(fifo, "[SERVER] Voce nao esta subscrito no topico");
  return -1;
}

/* Topic-Related Functions */

void list_topics(ServerData * sd) {
  if (sd -> topic_count == 0) {
    printf("[SERVER] Nenhum topico criado\n");
  } else {
    printf("[SERVER] Lista de Topicos:\n");
    for (int i = 0; i < sd -> topic_count; i++) {
      printf("Topico: %s | Mensagens: %d/%d | Estado: %s\n", sd -> topics[i].topic_name, sd -> topics[i].message_count, MAX_PERSISTENT_MESSAGES, sd -> topics[i].locked ? "Bloqueado" : "Desbloqueado");
    }
  }
  fflush(stdout);
}

void update_topics(ServerData * sd) {
  for (int i = 0; i < sd -> topic_count; i++) {
    if (sd -> topics[i].subscriber_count == 0 && sd -> topics[i].message_count == 0) {
      g_DEBUG ? printf("[SERVER] A apagar topico: %s\n", sd -> topics[i].topic_name) : (void) 0;

      for (int j = i; j < sd -> topic_count - 1; j++) {
        sd -> topics[j] = sd -> topics[j + 1];
      }

      sd -> topic_count--;

      i--;
    }
  }
}

Topic * find_or_create_topic(const char * topic_name, int create_if_not_found, ServerData * sd) {
  if (!topic_name) {
    g_DEBUG ? printf("[SERVER] Nome invalido na funcao do topic\n") : (void) 0;
    return NULL;
  }

  for (int i = 0; i < sd -> topic_count; i++) {
    if (strcmp(sd -> topics[i].topic_name, topic_name) == 0) {
      return & sd -> topics[i];
    }
  }

  if (create_if_not_found && sd -> topic_count < MAX_TOPICS) {
    if (strlen(topic_name) > MAX_TOPIC_LENGTH) {
      g_DEBUG ? printf("[SERVER] Falha ao criar topico: o nome '%s' excede o limite de 20 caracteres\n", topic_name) : (void) 0;
      return NULL;
    }
    Topic * new_topic = & sd -> topics[sd -> topic_count++];
    strncpy(new_topic -> topic_name, topic_name, sizeof(new_topic -> topic_name));
    new_topic -> topic_name[sizeof(new_topic -> topic_name) - 1] = '\0';
    new_topic -> locked = 0;
    new_topic -> subscriber_count = 0;
    new_topic -> message_count = 0;

    g_DEBUG ? printf("[SERVER] Topico '%s' criado com sucesso\n", topic_name) : (void) 0;
    return new_topic;
  }

  if (!create_if_not_found) {
    g_DEBUG ? printf("[SERVER] Topico '%s' nao encontrado\n", topic_name) : (void) 0;
  }

  return NULL;
}

int delete_topic(Topic * topic, ServerData * sd) {
  if (!topic) {
    g_DEBUG ? printf("[SERVER] Topico invalido\n") : (void) 0;
    return -1;
  }

  int topic_index = -1;
  for (int i = 0; i < sd -> topic_count; i++) {
    if ( & sd -> topics[i] == topic) {
      topic_index = i;
      break;
    }
  }

  if (topic_index == -1) {
    g_DEBUG ? printf("[SERVER] Topico nao encontrado\n") : (void) 0;
    return -1;
  }

  sd -> topics[topic_index] = sd -> topics[sd -> topic_count - 1];
  sd -> topic_count--;

  printf("[SERVER] Topico '%s' apagado com sucesso\n", topic -> topic_name);
  return 0;
}

int lock_topic(Topic * topic) {
  if (!topic) {
    g_DEBUG ? printf("[SERVER] Topico invalido\n") : (void) 0;
    return -1;
  }

  topic -> locked = 1;
  printf("[SERVER] Topico '%s' bloqueado\n", topic -> topic_name);
  return 0;
}

int unlock_topic(Topic * topic) {
  if (!topic) {
    g_DEBUG ? printf("[SERVER] Topico invalido\n") : (void) 0;
    return -1;
  }

  topic -> locked = 0;
  printf("[SERVER] Topico '%s' desbloqueado\n", topic -> topic_name);
  return 0;
}

void show_topic_messages(Topic * topic) {
  if (!topic) {
    g_DEBUG ? printf("[SERVER] Topico invalido\n") : (void) 0;
    return;
  }

  printf("[SERVER] Mensagens no topico '%s':\n", topic -> topic_name);

  if (topic -> message_count == 0) {
    printf("Nenhuma\n");
  } else {
    for (int i = 0; i < topic -> message_count; i++) {
      Message * msg = & topic -> messages[i];
      printf("De: %s | Duracao: %d segundos\nCorpo: %s\n", msg -> username, msg -> duration, msg -> body);
    }
  }
}

void add_message_to_topic(Topic * topic,
  const Message * new_msg) {
  if (!topic || !new_msg) {
    g_DEBUG ? printf("[SERVER] Topico ou mensagem invalidos\n") : (void) 0;
    return;
  }

  if (topic -> message_count >= MAX_PERSISTENT_MESSAGES) {
    printf("[SERVER] Numero maximo de mensagens atingidas no topico '%s'\n", topic -> topic_name);
    return;
  }

  topic -> messages[topic -> message_count++] = * new_msg;

  g_DEBUG ? printf("[SERVER] Mensagem persistente adicionada ao topico '%s'.\n", topic -> topic_name) : (void) 0;
}

/* Utility Functions */

/*void create_tmp()
{
    if (access("./tmp", F_OK) != 0)
    {
        if (mkdir("./tmp", 0700) == -1)
        {
            perror("[SERVER] Erro a criar pasta tmp");
            exit(1);
        }
        else
        {
            printf("[SERVER] Pasta tmp criada\n");
        }
    }
}

void delete_tmp()
{
    if (access("./tmp", F_OK) == 0)
    {
        if (rmdir("./tmp") == -1)
        {
            perror("[SERVER] Erro a remover pasta tmp");
            exit(1);
        }
        else
        {
            printf("[SERVER] Pasta tmp removida\n");
        }
    }
}*/

void notify_all_users(const char * message, ServerData * sd) {
  for (int i = 0; i < sd -> feed_count; i++) {
    char user_fifo[30];
    sprintf(user_fifo, FIFO_CLI, sd -> feed_pids[i]);

    send_response_to_client(user_fifo, message);

    g_DEBUG ? printf("[SERVER] Notificado '%s' (PID %d): %s\n", sd -> feed_usernames[i], sd -> feed_pids[i], message) : (void) 0;
  }
}

int load_messages_from_file(ServerData * sd) {
  const char * filename = getenv("MSG_FICH");
  if (!filename) {
    g_DEBUG ? fprintf(stderr, "[SERVER] Erro: MSG_FICH nao esta definida\n") : (void) 0;
    return 0;
  }

  FILE * file = fopen(filename, "r");
  if (!file) {
    g_DEBUG ? perror("[SERVER] Erro a abrir ficheiro para leitura") : (void) 0;
    return 0;
  }

  char line[400];
  while (fgets(line, sizeof(line), file)) {
    char * newline = strchr(line, '\n');
    if (newline) {
      * newline = '\0';
    }

    Message message;
    char topic_name[MAX_TOPIC_LENGTH + 2];
    char username[MAX_USERNAME_LENGHT + 2];
    char body[MAX_MSG_LENGTH + 2];

    int fields_parsed = sscanf(line, "%21s %21s %d %301[^\n]", topic_name, username, & message.duration, body);
    if (fields_parsed != 4) {
      g_DEBUG ? fprintf(stderr, "[SERVER] Erro: Syntax invalida na linha %s\n", line) : (void) 0;
      continue;
    }

    if (strlen(topic_name) > MAX_TOPIC_LENGTH ||
      strlen(username) > MAX_USERNAME_LENGHT ||
      strlen(body) > MAX_MSG_LENGTH) {
      g_DEBUG ? fprintf(stderr, "[SERVER] Erro: Limites nao cumpridos na linha %s\n", line) : (void) 0;
      continue;
    }

    strncpy(message.username, username, MAX_USERNAME_LENGHT);
    message.username[MAX_USERNAME_LENGHT] = '\0';
    strncpy(message.body, body, MAX_MSG_LENGTH);
    message.body[MAX_MSG_LENGTH] = '\0';

    if (message.duration <= 0) {
      g_DEBUG ? fprintf(stderr, "[SERVER] Erro: Duracao invalida na linha: %s\n", line) : (void) 0;
      continue;
    }

    Topic * topic = find_or_create_topic(topic_name, 1, sd);
    if (!topic) {
      g_DEBUG ? fprintf(stderr, "[SERVER] Erro: Erro a encontrar/criar topico na linha: %s\n", line) : (void) 0;
      continue;
    }

    add_message_to_topic(topic, & message);
  }

  fclose(file);
  return 1;
}

void send_message_to_subscribers(Topic * topic,
  const Message * msg, ServerData * sd) {
  if (!topic || !msg) {
    g_DEBUG ? printf("[SERVER] Topico ou mensagem invalidos\n") : (void) 0;
    return;
  }

  char response[BUFFER_SIZE];
  snprintf(response, sizeof(response), "[%s] %s: %s", topic -> topic_name, msg -> username, msg -> body);

  for (int i = 0; i < topic -> subscriber_count; i++) {
    int user_index = topic -> subscribers[i];
    if (user_index < 0 || user_index >= MAX_USERS) {
      g_DEBUG ? printf("[SERVER] Index %d invalido na lista de subscritores do topico '%s'\n", user_index, topic -> topic_name) : (void) 0;
      continue;
    }

    int subscriber_pid = sd -> feed_pids[user_index];

    char fifo[30];
    sprintf(fifo, FIFO_CLI, subscriber_pid);

    if (!send_response_to_client(fifo, response)) {
      g_DEBUG ? printf("[SERVER] Erro a enviar mensagem ao subscritor com PID %d via FIFO '%s'.\n", subscriber_pid, fifo) : (void) 0;
    } else {
      g_DEBUG ? printf("[SERVER] Mensagem enviada ao subscritor com PID %d via FIFO '%s'.\n", subscriber_pid, fifo) : (void) 0;
    }
  }
}

void update_message_duration(ServerData * sd) {
  for (int i = 0; i < sd -> topic_count; i++) {
    for (int j = 0; j < sd -> topics[i].message_count; j++) {
      if (sd -> topics[i].messages[j].duration > 0) {
        sd -> topics[i].messages[j].duration--;
      }
    }
  }

  for (int i = 0; i < sd -> topic_count; i++) {
    for (int j = 0; j < sd -> topics[i].message_count; j++) {
      if (sd -> topics[i].messages[j].duration == 0) {
        for (int k = j; k < sd -> topics[i].message_count - 1; k++) {
          sd -> topics[i].messages[k] = sd -> topics[i].messages[k + 1];
        }
        memset( & sd -> topics[i].messages[sd -> topics[i].message_count - 1], 0, sizeof(Message));
        sd -> topics[i].message_count--;
        j--;
      }
    }
  }
}

void save_messages_to_file(ServerData * sd) {
  const char * filename = getenv("MSG_FICH");
  if (!filename) {
    g_DEBUG ? fprintf(stderr, "[SERVER] Erro: MSG_FICH nao esta definida\n") : (void) 0;
    return;
  }

  FILE * file = fopen(filename, "w");
  if (!file) {
    g_DEBUG ? perror("[SERVER] Erro a abrir ficheiro para escrita") : (void) 0;
    return;
  }

  for (int i = 0; i < sd -> topic_count; i++) {
    Topic * topic = & sd -> topics[i];
    for (int j = 0; j < topic -> message_count; j++) {
      Message * msg = & topic -> messages[j];
      msg -> body[MAX_MSG_LENGTH] = '\0';
      msg -> username[MAX_USERNAME_LENGHT] = '\0';
      fprintf(file, "%s %s %d %s\n", topic -> topic_name, msg -> username, msg -> duration, msg -> body);
    }
  }

  fclose(file);
}

/*void cleanup_and_exit(ServerData *sd)
{
    for (int i = 0; i < sd->feed_count; i++)
    {
        kill(sd->feed_pids[i], SIGTERM);
    }

    save_messages_to_file(sd);

    close(sd->fd_srv);
    unlink(FIFO_SRV);

    // delete_tmp();

    g_DEBUG ? printf("[SERVER] FIM\n") : (void)0;
    fflush(stdout);

    exit(0);
}*/

/* DEBUG FUNCTIONS DONT TOUCH*/
void debug_server_state(ServerData * sd) {
  printf("\n============= SERVER DEBUG =============\n");

  printf("Utilizadores Conectados (%d/%d):\n", sd -> feed_count, MAX_USERS);
  for (int i = 0; i < sd -> feed_count; i++) {
    printf(" - Username: %s | PID: %d\n", sd -> feed_usernames[i], sd -> feed_pids[i]);
  }

  printf("\nTopicos (%d/%d):\n", sd -> topic_count, MAX_TOPICS);
  for (int i = 0; i < sd -> topic_count; i++) {
    Topic * topic = & sd -> topics[i];
    printf(" - Topico: %s | Estado: %s | Subscritores: %d | Mensagens: %d/%d\n", topic -> topic_name, topic -> locked ? "Bloqueado" : "Desbloqueado", topic -> subscriber_count, topic -> message_count, MAX_PERSISTENT_MESSAGES);

    printf("   Subscritores: ");
    for (int j = 0; j < topic -> subscriber_count; j++) {
      int user_index = topic -> subscribers[j];

      if (user_index >= 0 && user_index < sd -> feed_count) {
        printf("%s (PID: %d)", sd -> feed_usernames[user_index], sd -> feed_pids[user_index]);
      } else {
        printf("[Index do subscritor invalido]");
      }

      if (j < topic -> subscriber_count - 1) {
        printf(", ");
      }
    }
    printf("\n");

    printf("   Mensagens:\n");
    for (int k = 0; k < topic -> message_count; k++) {
      Message * msg = & topic -> messages[k];
      printf("     [%s] De: %s | Duracao: %d\n       Corpo: %s\n", topic -> topic_name, msg -> username, msg -> duration, msg -> body);
    }
  }
  printf("========================================\n");
}