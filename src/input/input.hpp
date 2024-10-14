#pragma once

#include <hirie.hpp>
#include <stdint.h>

extern const uint8_t *keys;
int hirie_take_input(hirie_object* model);
int hirie_poll_exit(hirie_window* window);