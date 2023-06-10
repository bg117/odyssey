#ifndef ODYSSEY_LOG_H
#define ODYSSEY_LOG_H

#define LOG(s, ...) \
  terminal_printf("[%s:%d]: " s "\n", __FILE__, __LINE__, ##__VA_ARGS__)

#endif