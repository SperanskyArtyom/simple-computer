#pragma once

enum keys
{
  KEY_UNKNOWN,

  KEY_UP,
  KEY_DOWN,
  KEY_RIGHT,
  KEY_LEFT,

  KEY_F5,
  KEY_F6,

  KEY_ENTER,
  KEY_ESC,

  KEY_PLUS = 0x2b,
  KEY_MINUS = 0x2d,

  KEY_0 = 0x30,
  KEY_1,
  KEY_2,
  KEY_3,
  KEY_4,
  KEY_5,
  KEY_6,
  KEY_7,
  KEY_8,
  KEY_9,

  KEY_A = 0x61,
  KEY_B,
  KEY_C,
  KEY_D,
  KEY_E,
  KEY_F,
  KEY_G,
  KEY_H,
  KEY_I,
  KEY_J,
  KEY_K,
  KEY_L,
  KEY_M,
  KEY_N,
  KEY_O,
  KEY_P,
  KEY_Q,
  KEY_R,
  KEY_S,
  KEY_T,
  KEY_U,
  KEY_V,
  KEY_W,
  KEY_X,
  KEY_Y,
  KEY_Z,
};

int rk_readkey (enum keys *key);
int rk_mytermsave (void);
int rk_mytermrestore (void);
