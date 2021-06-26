#pragma once
#include <cstdint>

extern uint64_t state[4];

extern uint64_t next(uint64_t s[4]);

extern void jump(uint64_t s[4]);

void long_jump(uint64_t s[4]);

extern void rand_init(uint64_t s[4], int ctr_no);