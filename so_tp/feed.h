#ifndef FEED_H
#define FEED_H

// Function declarations

#include "utils.h"

/* Main Initialization Functions */
void init_feed(FeedData * fd,
  const char * username);

/* Communication Functions */
void create_and_open_fifos(FeedData * fd);
void send_checkin(FeedData * fd, int * running);
int handle_feed_commands(FeedData * fd, ServerResponse * sr);
void process_manager_request(Resposta * r, FeedData * fd, ServerResponse * sr);
void * getServerResponse(void * pData);

/* Misc Functions */
void cleanup_and_exit(FeedData * fd);

/* Signal Handling Functions */
void handle_signal(int sig);

#endif