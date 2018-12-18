#include "AX12Controller.h"



AX12Controller::AX12Controller(unsigned char ID)
{
    attach(ID);
}


int AX12Controller::integral_position_controller(int desired_pos, int current_pos)
{
    static double somme = 0;

    somme += (desired_pos - current_pos);

    int ret =(int) (0.01 * somme);

    return ret;
}