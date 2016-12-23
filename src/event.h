#ifndef EVENT_H_
#define EVENT_H_

enum event_kind {
    EVENT_GYRO_IDLE,
    EVENT_GYRO_MOVED,
    EVENT_SWITCH,
    EVENT_TICK,
    EVENT_BRIGHT,
    EVENT_DIM,
};

struct event {
    enum event_kind kind;
};

#endif /* !EVENT_H_ */
