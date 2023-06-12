#ifndef ODYSSEY_LOG_H
#define ODYSSEY_LOG_H

#define LOG(s, ...) \
  terminal_printf("[%s:%llu]: " s "\n", __func__, __LINE__, ##__VA_ARGS__)

#endif