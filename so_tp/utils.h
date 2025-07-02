#ifndef UTILS_H
#define UTILS_H

#define true 1
#define false 0
#define bool int

#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <unistd.h>

#include <fcntl.h>

#include <signal.h>

#include <sys/select.h>

#include <sys/stat.h>

#include <errno.h>

#include <pthread.h>

#define FIFO_SRV "/tmp/f_manager"
#define FIFO_CLI "/tmp/f_%d"
#define BUFFER_SIZE 350
#define MAX_USERS 10
#define MAX_TOPICS 20
#define MAX_TOPIC_LENGTH 20
#define MAX_MSG_LENGTH 300
#define MAX_USERNAME_LENGHT 20
#define MAX_PERSISTENT_MESSAGES 5

#define ERROR_CODE_USER_EXISTS "#EXISTS"
#define ERROR_CODE_SERVER_FULL "#FULL"
#define FEED_EXIT_CODE "#EXITGET"

bool g_DEBUG = true;

typedef struct {
  char str[BUFFER_SIZE];
  char username[MAX_USERNAME_LENGHT + 1];
  int pid;
}
Pedido;

typedef struct {
  char str[BUFFER_SIZE];
}
Resposta;

typedef struct {
  char username[MAX_USERNAME_LENGHT + 1];
  int duration;
  char body[MAX_MSG_LENGTH + 1];
}
Message;

typedef struct {
  char topic_name[MAX_TOPIC_LENGTH + 1];
  int locked;
  int subscriber_count;
  int subscribers[MAX_USERS];
  Message messages[MAX_PERSISTENT_MESSAGES];
  int message_count;
}
Topic;

typedef struct {
  int fd_srv;
  pid_t feed_pids[MAX_USERS];
  char feed_usernames[MAX_USERS][MAX_USERNAME_LENGHT + 1];
  int feed_count;
  Topic topics[MAX_TOPICS];
  int topic_count;
  pthread_mutex_t lock;
}
ServerData;

typedef struct {
  ServerData * sd;
  int * running;
}
TALARM;

typedef struct {
  ServerData * sd;
  int * running;
}
ClientRequest;

typedef struct {
  int fd_cli, fd_srv;
  char fifo[30];
  char username[MAX_USERNAME_LENGHT + 1];
  Pedido p;
  Resposta r;
}
FeedData;

typedef struct {
  FeedData * fd;
  int * running;
  pthread_t tid;
}
ServerResponse;

#endif