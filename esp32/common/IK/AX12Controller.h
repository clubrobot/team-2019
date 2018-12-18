#ifndef __AX12CONTROLLER_H
#define __AX12CONTROLLER_H

#include <Arduino.h>
#include "AX12.h"

class AX12Controller: private AX12
{
    public:
        AX12Controller(unsigned char ID);

    private:

        int integral_position_controller(int desired_pos, int current_pos);

};

#endif /* __AX12CONTROLLER_H */