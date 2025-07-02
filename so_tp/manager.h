#ifndef MANAGER_H
#define MANAGER_H

// Function declarations

#include "utils.h"

/* Main Server Functions */
void * doAlarm(void * pData);
void * getClientRequest(void * pData);
// void setup_signals();
//  void start_server(ServerData *sd);
int handle_manager_commands(ServerData * sd);

/* User-Related Functions */
void process_client_request(Pedido * p, ServerData * sd);
void notify_all_users(const char * message, ServerData * sd);
int get_user_index(const char * username, ServerData * sd);
void handle_user_removal(const char * username, int notify_users, ServerData * sd);
int remove_username(const char * username, ServerData * sd);
int is_pid_duplicate(const int pid, ServerData * sd);
int is_username_duplicate(const char * username, ServerData * sd);
void display_connected_users(ServerData * sd);
int send_response_to_client(const char * fifo,
  const char * response);
int subscribe_to_topic(const char * username, Topic * topic, ServerData * sd);
int unsubscribe_from_topic(const char * username, Topic * topic, ServerData * sd);

/* Topic-Related Functions */
void list_topics(ServerData * sd);
void update_topics(ServerData * sd);
Topic * find_or_create_topic(const char * topic_name, int create_if_not_found, ServerData * sd);
int delete_topic(Topic * topic, ServerData * sd);
int lock_topic(Topic * topic);
int unlock_topic(Topic * topic);
void show_topic_messages(Topic * topic);
void add_message_to_topic(Topic * topic,
  const Message * new_msg);
void send_message_to_subscribers(Topic * topic,
  const Message * msg, ServerData * sd);
void update_message_duration(ServerData * sd);

/* Utility Functions */
/*void create_tmp();
void delete_tmp();*/
int load_messages_from_file(ServerData * sd);
void save_messages_to_file(ServerData * sd);

/* Cleanup Function */
// void cleanup_and_exit(ServerData *sd);

/* Signal Handling Functions */
void handle_sigint(int sig);
// void handle_sigalrm(int sig);

/* DEBUG FUNCTIONS DONT TOUCH */
void debug_server_state(ServerData * sd);

// Global variables

#endif