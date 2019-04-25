#ifndef __ARMMANAGER_H
#define __ARMMANAGER_H

#include <Arduino.h>
#include "Picker.h"
#include "Joint.h"
#include "MotorWrapper.h"
#include "../AX12/AX12.h"
#include "thread_tools.h"
#include "constants.h"
#include "datatype.h"
#include "MoveBatch.h"

namespace IK
{

class ArmManager : public Picker
{
    public:

        ArmManager() throw(){};

        void        set_workspace(Workspace ws_front, Workspace ws_back) throw();
        void        set_origin(Coords origin) throw();
        void        set_arm_link(float l1, float l2, float l3, int elbow_or);
        void        set_initial_joint_pos(Joints joints);

        float        get_link1() const {return _l1;}
        float        get_link2() const {return _l2;}
        float        get_link3() const {return _l3;}
        float        get_elbow() const {return _elbow;}

        Workspace   get_workspace_front() const {return _ws_front;}
        Workspace   get_workspace_back () const {return _ws_back;}

        Coords      get_origin() const {return _origin;}
        Joints      get_joints() const {return _joints;}

        void        init();

        Workspace clip_Workspaceo_constraints(Workspace workspace) throw();
        Coords    workspace_center(Workspace workspace) throw();
        MoveBatch   go_to(Coords start_pos, Coords target_pos);

        float      estimated_time_of_arrival(Coords start_pos, Coords start_vel, Coords target_pos, Coords target_vel);

        void load(int address);
        void save(int address) const;
        
    private:

        Workspace workspace_containing_position(Coords position) throw();
        bool        workspace_within_constraints(Workspace workspace) throw();
        bool        position_within_workspace(Coords position, Workspace workspace) throw();
        MoveBatch   goto_workspace(Coords start_pos, Coords target_pos, Workspace new_workspace);
        Joints    goto_position(Coords target_pos);

        Workspace _ws_front;
        Workspace _ws_back;
        Workspace _workspace;

        Coords _tool;
        Coords _origin;
        Joints _joints;

        float _l1;
        float _l2;
        float _l3;
        float _elbow;

        Mutex _mutex;
};

}

#endif /* __ARMMANAGER_H */