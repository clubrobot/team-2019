#ifndef DATASYNC_H
#define DATASYNC_H

typedef enum Color{GREEN = 0, ORANGE = 1};
typedef enum Channel { LONGDATA_RANGE_LOWPOWER = 1,
                       SHORTDATA_FAST_LOWPOWER = 2,
                       LONGDATA_FAST_LOWPOWER = 3,
                       SHORTDATA_FAST_ACCURACY = 4,
                       LONGDATA_FAST_ACCURACY = 5,
                       LONGDATA_RANGE_ACCURACY = 6 };

typedef struct DataSync
{
    Color color;
    Channel channel;
};

#endif