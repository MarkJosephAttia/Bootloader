#ifndef STD_TYPES_H
#define STD_TYPES_H

#define NULL                            ((void*)0)

typedef void (*callback_t)(void);

typedef uint8_t Std_ReturnType;

#define E_OK                            (0)
#define E_NOT_OK                        (1)

#define STD_LOW                         (0)
#define STD_HIGH                        (1)

#define STD_IDLE                        (0)
#define STD_ACTIVE                      (1)

#define STD_OFF                         (0)
#define STD_ON                          (1)

#endif
