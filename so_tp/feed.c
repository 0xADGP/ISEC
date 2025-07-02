#include "feed.h"

int main(int argc, char * argv[]) {
  if (argc != 2) {
    fprintf(stderr, "[FEED] Uso: %s <username>\n", argv[0]);
    return 0;
  }

  if (strlen(argv[1]) > MAX_USERNAME_LENGHT) {
    // printf("%ld\n",strlen(argv[1]));
    fprintf(stderr, "[FEED] O nome de utilizador nao pode exceder 20 caracteres\n");
    return 0;
  }

  FeedData fd;
  strncpy(fd.username, argv[1], sizeof(fd.username) - 1);
  fd.username[sizeof(fd.username) - 1] = '\0';

  init_feed( & fd, fd.username);

  return 0;
}

/* Main Initialization Functions */

void init_feed(FeedData * fd,
  const char * username) {
  int running = 1;

  if (access(FIFO_SRV, F_OK) != 0) {
    printf("[FEED - %s (%d)] O manager nao esta a correr\n", username, getpid());
    // cleanup_and_exit(fd);
    running = 0;
  }

  if (!running) {
    return;
  }
  g_DEBUG ? printf("[FEED - %s (%d)] A iniciar feed para utilizador %s\n", username, getpid(), username) : (void) 0;

  struct sigaction sa;
  sa.sa_flags = 0;
  sa.sa_handler = handle_signal;
  sigaction(SIGUSR1, & sa, NULL);
  sigaction(SIGINT, & sa, NULL);

  create_and_open_fifos(fd);

  send_checkin(fd, & running);

  if (!running) {
    return;
  }

  pthread_t server_thread;

  ServerResponse server_response;
  server_response.running = & running;
  server_response.fd = fd;

  server_response.tid = pthread_self();

  pthread_create( & server_thread, NULL, getServerResponse, & server_response);

  while (handle_feed_commands(fd, & server_response))
  ;

  running = 0;

  pthread_join(server_thread, NULL);

  cleanup_and_exit(fd);
}

/* Communication Functions */

void create_and_open_fifos(FeedData * fd) {
  sprintf(fd -> fifo, FIFO_CLI, getpid());
  mkfifo(fd -> fifo, 0600);

  fd -> fd_cli = open(fd -> fifo, O_RDWR);
  fd -> fd_srv = open(FIFO_SRV, O_WRONLY);
  if (fd -> fd_srv < 0) {
    perror("[FEED] Error a abrir FIFO_SRV");
    cleanup_and_exit(fd);
  }
}

void send_checkin(FeedData * fd, int * running) {
  fd -> p.pid = getpid();
  strcpy(fd -> p.username, fd -> username);
  strcpy(fd -> p.str, "CHECKIN");

  if (write(fd -> fd_srv, & fd -> p, sizeof(Pedido)) != sizeof(Pedido)) {
    g_DEBUG ? printf("[FEED - %s (%d)] Erro a enviar checkin\n", fd -> username, getpid()) : (void) 0;
    * running = 0;
    cleanup_and_exit(fd);
  }

  if (read(fd -> fd_cli, & fd -> r, sizeof(Resposta)) <= 0) {
    g_DEBUG ? printf("[FEED - %s (%d)] Erro ao ler do servidor. Conexao rejeitada\n", fd -> username, getpid()) : (void) 0;
    * running = 0;
    cleanup_and_exit(fd);
  }

  if (strcmp(fd -> r.str, ERROR_CODE_USER_EXISTS) == 0) {
    printf("[FEED - %s (%d)] Nome '%s' ja esta em uso. Conexao rejeitada\n", fd -> username, getpid(), fd -> username);
    * running = 0;
    cleanup_and_exit(fd);
  }

  if (strcmp(fd -> r.str, ERROR_CODE_SERVER_FULL) == 0) {
    printf("[FEED - %s (%d)] O servidor esta cheio. Conexao rejeitada\n", fd -> username, getpid());
    * running = 0;
    cleanup_and_exit(fd);
  }
}

/* Thread Functions */

void * getServerResponse(void * pData) {
  ServerResponse * pTa = (ServerResponse * ) pData;
  FeedData * fd = pTa -> fd;
  int * running = pTa -> running;

  Resposta r;

  while ( * running) {
    int res = read(fd -> fd_cli, & r, sizeof(Resposta));
    if (res == sizeof(Resposta)) {
      process_manager_request( & r, fd, pTa);
    } else if (res == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
      usleep(10000);
    } else if (res < 0) {
      perror("[FEED] Erro a ler do servidor");
    }
  }

  g_DEBUG ? printf("[FEED - %s] Thread 'serverResponse' a sair\n", fd -> username) : (void) 0;

  pthread_exit(NULL);
}

int handle_feed_commands(FeedData * fd, ServerResponse * sr) {
  char str[BUFFER_SIZE];

  int * running = sr -> running;

  if (! * running) {
    return 0;
  }

  if (fgets(str, sizeof(str), stdin) != NULL && * running) {
    str[strcspn(str, "\n")] = 0;

    if (strlen(str) == 0) {
      g_DEBUG ? printf("[FEED] Comando vazio ignorado\n") : (void) 0;
      return * running;
    }

    strcpy(fd -> p.str, str);
    fd -> p.pid = getpid();
    strcpy(fd -> p.username, fd -> username);

    if (write(fd -> fd_srv, & fd -> p, sizeof(Pedido)) == sizeof(Pedido)) {
      if (strcmp(str, "exit") == 0) {
        g_DEBUG ? printf("[FEED - %s (%d)] Desligando feed\n", fd -> username, getpid()) : (void) 0;
        return 1;
      }
    }
  }
  return * running;
}

void process_manager_request(Resposta * r, FeedData * fd, ServerResponse * sr) {
  if (strcmp(r -> str, FEED_EXIT_CODE) == 0) {
    * sr -> running = 0;
    pthread_kill(sr -> tid, SIGUSR1);
  } else {
    printf("[FEED - %s (%d)] %s\n", fd -> username, getpid(), r -> str);
  }
  fflush(stdout);
}

/* Cleanup Functions */

void cleanup_and_exit(FeedData * fd) {
  close(fd -> fd_srv);
  close(fd -> fd_cli);
  unlink(fd -> fifo);
  g_DEBUG ? printf("[FEED - %s (%d)] FIM\n", fd -> username, getpid()) : (void) 0;
  fflush(stdout);
}

/* Signal Handling Functions */

void handle_signal(int sig) {
  if (sig == 2) {
    g_DEBUG ? printf("\n[SERVER] Escreva 'exit' para sair\n") : (void) 0;
  }

  // cleanup_and_exit();
}