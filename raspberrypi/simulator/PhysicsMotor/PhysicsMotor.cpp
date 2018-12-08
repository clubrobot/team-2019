#include "PhysicsMotor.h"
#include <cmath>
#include <limits>
Vector genVec(Point p1,Point p2){
    Vector vec= {p2.x-p1.x,p2.y-p1.y};
    return vec;
}

void PhysicsMotor::process(){
    std::map<std::string,PhysicsObject>::iterator it;
    for (it=m_objects.begin(); it!=m_objects.end(); ++it){
        PhysicsMotor::move((it->second));
    }
    bool collided = true;
    while(collided){
        collided = false;
        for (it=m_objects.begin(); it!=m_objects.end(); ++it){
            if((it->second).m_enable==false) continue;
            std::vector<std::string> ban_list;
            ban_list.push_back((it->first));
            collided = collide((it->first),&(it->second),ban_list) || collided ;

        }
    }

    
}

bool member(std::vector<std::string>* ban_list,std::string name ){
           for(std::string element : *ban_list ){
            if(element ==name) return true;
        } 
        return false;
}

bool PhysicsMotor::collide(std::string name,PhysicsObject * main,std::vector<std::string> ban_list){

    bool collided = false;
    Polygon current = main->m_shape_temp;
    std::map<std::string,PhysicsObject>::iterator it;
    for (it=m_objects.begin(); it!=m_objects.end(); ++it){
        if(it->first == name) continue;
        if(member(&ban_list,it->first)) continue;
        if((it->second).m_enable==false ) continue;
        int index = -1;
        Point impact  = {0};
        float distance=std::numeric_limits<float>::max();
        Vector opositForcetoMain = {0};
        Vector opositForcetoSecond = {0};
        Polygon second = (it->second).m_shape_temp;
        int i;
        for(i=0;i<second.length;i++){
            int k;
            distance =std::numeric_limits<float>::max();
            for(k=0;k<current.length;k++){
                Vector v1 = genVec(current.list[k],second.list[i]);
                Vector v2 = genVec(current.list[k],current.list[(k+1)%current.length]);
                //float norm1 = v1.norm();
                float norm2 = v2.norm();
                float proj  = v1*v2/norm2;
                Vector vult = v1^v2;
                if((vult.z)>0){
                    impact = second.list[i];
                    Point inter = {current.list[k].x,current.list[k].y};
                    inter.x += v2.x*proj/norm2;
                    inter.y += v2.y*proj/norm2;
                    //looking for the shortest distance between the point and a 
                    float distance_temp = vult.norm()/v2.norm();
                    if(distance_temp<distance){
                        index = k;
                        distance = distance_temp;
                        opositForcetoMain = genVec(inter,second.list[i]);
                        opositForcetoSecond = genVec(inter,second.list[i]);
                        opositForcetoSecond.x *=-1;
                        opositForcetoSecond.y *=-1;
                    }
                }else{
                    index = -1;
                    break;
                    }
                }


            //If we find a point into main polygon we can stop this loop
            if(index!=-1) break;
        }
        if(index!=-1){  
            //main->stop();
            //(it->second).stop();
            collided = true;


            if(main->m_weight>=(it->second).m_weight){
                // Si main est le plus gros
                (it->second).m_center+= opositForcetoSecond;
                (it->second).genShape();
                Velocity common_velocity = {main->m_velocity.linear*main->m_weight +(it->second).m_velocity.linear*(it->second).m_weight,main->m_velocity.angular};
                common_velocity.linear = common_velocity.linear/(main->m_weight+(it->second).m_weight);
                main->m_velocity = common_velocity; 
                // On fait un return vrai et on refait les boucle tant que c'est vrai
                collide(it->first,&(it->second),ban_list);
            }else{
                main->m_center-= opositForcetoSecond;
                main->genShape();
                current = main->m_shape_temp;
                // Si main est le plus leger
                Velocity common_velocity = {main->m_velocity.linear*main->m_weight +(it->second).m_velocity.linear*(it->second).m_weight,main->m_velocity.angular};
                common_velocity.linear = common_velocity.linear/(main->m_weight+(it->second).m_weight);
                (it->second).m_velocity = common_velocity; 
                // On fait un return vrai et on refait les boucle tant que c'est vrai
                collide(it->first,&(it->second),ban_list);


            }
        }
    }

        // Map coliision part 1
    int i;
    for(i=0;i<current.length;i++){
        int k;
            
        for(k=0;k<m_map.length;k++){

            Vector v1 = genVec(m_map.list[k],m_map.list[ (k+1)%m_map.length]);
            Vector v2 = genVec(m_map.list[k],current.list[i]);

            if( (v1^v2).z>0){
                float norm1 = v1.norm();
                float proj  =(v1*v2)/norm1;

                if(0<proj && proj<norm1  ){
                    
                    Point inter = m_map.list[k];
                    inter.x+=v1.x *proj/norm1;
                    inter.y+=v1.y * proj/norm1;
                    Vector translation= genVec(current.list[i],inter);
                    if(translation.norm()<25){
                        main->m_center += translation;
                        main->setVelocity(0,0);
                        main->m_setvel.linear = 0;
                        main->m_setvel.angular = 0;
                        collided = true;

                        }

                }
            }
                
        }

    }

    // On vérifie que un point de la map n'est pas dans l'objet

    for(i=0;i<m_map.length;i++){
        int k;
        float distance = std::numeric_limits<float>::max();
        float distance_temp;
        Vector v1;
        Vector v2;
        float proj=0;
        float norm1=1;
        float norm1_final = 1;
        int index;
        Vector vult;

        for(k=0;k<current.length;k++){
            v1 = genVec(current.list[k],current.list[(k+1)% current.length]);
            v2 = genVec(current.list[k],m_map.list[i]);
            vult = v1^v2;
            if(vult.z<0){
                norm1 = v1.norm();
                distance_temp = vult.norm()/norm1;
                if(distance>distance_temp){
                    proj = v1*v2/norm1;
                    norm1_final =norm1;
                    index = k;
                    distance = distance_temp;
                }


            }else{
                index=-1;
                break;
                
            }
        
        }


        if(index!=-1){
            collided =true;
            Point inter = current.list[index];
            v1 = genVec(current.list[index],current.list[(index+1)% current.length]);
            inter.x += v1.x*proj/norm1_final;
            inter.y += v1.y*proj/norm1_final;
            Vector translation = genVec(inter,m_map.list[i]);
            main->m_center += translation;
            main->setVelocity(0,0);
            main->m_setvel.linear = 0;
            main->m_setvel.angular = 0;
            break;

        }

    }
    /*if(collided){
        std::cout<<"aie"<<std::endl;
    }*/
    // Si la vitesse est trop faible il n'y a pas de pattinage
    if(main->m_setvel.linear<5) collided = false;
    main->m_collided = collided;

    return false;

    //teleporter les objects et determiner les vitesses
}



void PhysicsMotor::move(PhysicsObject& p){
    float time_step = m_timestep*1e-3+((  (float)  (clock()-p.getLastCall())  )/  CLOCKS_PER_SEC)  ;

    if(p.m_setvel.angular>p.m_velocity.angular){

        if( p.m_setvel.angular-p.m_velocity.angular< time_step*p.m_maxacc.angular){
            p.m_velocity.angular = p.m_setvel.angular;
        }else{

             p.m_velocity.angular += time_step*p.m_maxacc.angular;
        }

    }else if(p.m_setvel.angular<p.m_velocity.angular){

        if( p.m_velocity.angular-p.m_setvel.angular< time_step*p.m_minacc.angular){
            p.m_velocity.angular = p.m_setvel.angular;
        }else{
             p.m_velocity.angular -= time_step*p.m_minacc.angular;
        }
    }

    float theta_temp = p.m_theta + p.m_velocity.angular*time_step/2.0;
    p.m_theta += p.m_velocity.angular*time_step;

    if(p.m_setvel.linear>p.m_velocity.linear){

        if( p.m_setvel.linear-p.m_velocity.linear< time_step*p.m_maxacc.linear){
            p.m_velocity.linear = p.m_setvel.linear;
        }else{

             p.m_velocity.linear += time_step*p.m_maxacc.linear;
        }

    }else if(p.m_setvel.linear<p.m_velocity.linear){

        if( p.m_velocity.linear-p.m_setvel.linear< time_step*p.m_minacc.linear){
            p.m_velocity.linear = p.m_setvel.linear;
        }else{
             p.m_velocity.linear -= time_step*p.m_minacc.linear;
        }

    }
    //std::cout<< p.m_velocity.linear*time_step*cosf(theta_temp)<<std::endl;
    p.m_center.x += p.m_velocity.linear*time_step*cosf(theta_temp);
    p.m_center.y += p.m_velocity.linear*time_step*sinf(theta_temp);



    p.setTime(clock());
    p.genShape();
}


void PhysicsMotor::addObject(std::string name){
    m_mtx.lock();
    m_objects[name] = PhysicsObject();
    m_objects[name].setTime(clock());
    m_mtx.unlock();
}




void PhysicsMotor::removeObject(std::string name){
    m_mtx.lock();
    m_objects.erase(name);
    m_mtx.unlock();
}

std::string PhysicsMotor::getObject(float x,float y,float threshold){
    m_mtx.lock();
    std::string result = "None";
    std::map<std::string,PhysicsObject>::iterator it;
    for (it=m_objects.begin(); it!=m_objects.end(); ++it){
        if( genVec((it->second).m_center,{x,y}).norm()<threshold){
            result = it->first;
            break;
        }
    }
    m_mtx.unlock();
    return result;
}

void PhysicsMotor::enableObject(std::string object){
    m_objects[object].enable();
}

void PhysicsMotor::disableObject(std::string object){
    m_objects[object].disable();
}


void PhysicsMotor::setShape(std::string name,boost::python::tuple poly){
    std::map<std::string,PhysicsObject>::iterator it;
    it = m_objects.find(name);
    if (it == m_objects.end()) return;

    m_mtx.lock();
    (it->second).setPolygon(poly);
    m_mtx.unlock();
}

void PhysicsMotor::setPosition(std::string name,float x,float y){
    std::map<std::string,PhysicsObject>::iterator it;
    it = m_objects.find(name);
    if (it == m_objects.end()) return;

    m_mtx.lock();
    (it->second).setPosition(x,y);
    m_mtx.unlock();
}

void PhysicsMotor::setWeight(std::string name,float weight){
    std::map<std::string,PhysicsObject>::iterator it;
    it = m_objects.find(name);
    if (it == m_objects.end()) return;

    m_mtx.lock();
    (it->second).setWeight(weight);
    m_mtx.unlock();

}

void PhysicsMotor::setTheta(std::string name,float t){
    std::map<std::string,PhysicsObject>::iterator it;
    it = m_objects.find(name);
    if (it == m_objects.end()) return;

    m_mtx.lock();
    (it->second).setTheta(t);
    m_mtx.unlock();
}

void PhysicsMotor::setVelocity(std::string name,float lin,float ang,bool forced){
    std::map<std::string,PhysicsObject>::iterator it;
    it = m_objects.find(name);
    if (it == m_objects.end()) return;

    m_mtx.lock();


    if (forced==true){

        (it->second).setVelocity(lin,ang);
        (it->second).setVelSet(lin,ang);
    }else{
        (it->second).setVelSet(lin,ang);
    }
    m_mtx.unlock();


}


void PhysicsMotor::setMaxAcceleration(std::string name,float lin,float vel){
    std::map<std::string,PhysicsObject>::iterator it;
    it = m_objects.find(name);
    if (it == m_objects.end()) return;

    m_mtx.lock();
    (it->second).setMaxAcceleration(lin,vel);
    m_mtx.unlock();

}

void PhysicsMotor::setMinAcceleration(std::string name,float lin,float vel){
    std::map<std::string,PhysicsObject>::iterator it;
    it = m_objects.find(name);
    if (it == m_objects.end()) return;

    m_mtx.lock();
    (it->second).setMinAcceleration(lin,vel);
    m_mtx.unlock();

}


boost::python::tuple PhysicsMotor::getShape(std::string name){
    std::map<std::string,PhysicsObject>::iterator it;
    it = m_objects.find(name);
    if (it == m_objects.end()) return boost::python::make_tuple();

    m_mtx.lock();
    boost::python::tuple result = (it->second).getPolygon();
    m_mtx.unlock();

    return result;
}





boost::python::tuple PhysicsMotor::getPosition(std::string name){
    std::map<std::string,PhysicsObject>::iterator it;
    it = m_objects.find(name);
    if (it == m_objects.end()) return boost::python::make_tuple();

    m_mtx.lock();
    boost::python::tuple result = (it->second).getPosition();
    m_mtx.unlock();

    return result;

}

bool PhysicsMotor::isCollided(std::string name){
    std::map<std::string,PhysicsObject>::iterator it;
    it = m_objects.find(name);
    if (it == m_objects.end()) return false;
    m_mtx.lock();
    bool result = (it->second).m_collided;
    m_mtx.unlock();
    return result;
}

float PhysicsMotor::getTheta(std::string name){
    std::map<std::string,PhysicsObject>::iterator it;
    it = m_objects.find(name);
    if (it == m_objects.end()) return 0.0;

    m_mtx.lock();
    float result = (it->second).getTheta();
    m_mtx.unlock();

    return result;

}

float PhysicsMotor::getWeight(std::string name){
    std::map<std::string,PhysicsObject>::iterator it;
    it = m_objects.find(name);
    if (it == m_objects.end()) return 0.0;

    m_mtx.lock();
    float result = (it->second).getWeight();
    m_mtx.unlock();

    return result;

}

boost::python::tuple PhysicsMotor::getVelocity(std::string name){
    std::map<std::string,PhysicsObject>::iterator it;
    it = m_objects.find(name);
    if (it == m_objects.end()) return boost::python::make_tuple();

    m_mtx.lock();
    boost::python::tuple result = (it->second).getVelocity();
    m_mtx.unlock();

    return result;

}


boost::python::tuple PhysicsMotor::getMaxAcceleration(std::string name){
    std::map<std::string,PhysicsObject>::iterator it;
    it = m_objects.find(name);
    if (it == m_objects.end()) return boost::python::make_tuple();

    return (it->second).getMaxAcceleration();

}

boost::python::tuple PhysicsMotor::getMinAcceleration(std::string name){
    std::map<std::string,PhysicsObject>::iterator it;
    it = m_objects.find(name);
    if (it == m_objects.end()) return boost::python::make_tuple();

    return (it->second).getMinAcceleration();

}



