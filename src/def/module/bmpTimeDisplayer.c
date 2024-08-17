#include "module/bmpTimeDisplayer.h"

#include "painter.h"

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "decTypes.h"


struct bmpTimeDisplayer* request_bmpTimeLcd_direct()
{
    struct bmpTimeDisplayer* new_button =
        (struct bmpTimeDisplayer*)malloc(sizeof(struct bmpTimeDisplayer));
    init_bmpTimeDisplayer(new_button);

    return new_button;
}


void init_bmpTimeLcd(struct bmpTimeDisplayer* displayer, time_t initiate_time)
{
    displayer->center_cord.x = 0;
    displayer->center_cord.y = 0;
}
