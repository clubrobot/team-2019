#ifndef __STEPBYSTEPMOTOR_H__
#define __STEPBYSTEPMOTOR_H__

#define STEP_BY_REV (200*16)

#define P_MM 80L

#define ACC  150000L	/* 1500 p/s² */
#define PLAT 9000L	/* 1500 p/s  */
#define DECC 150000L	/* 1700 p/s² */

class StepByStepMotor
{
	public:
		StepByStepMotor();

		void attach(int step, int dir, int enable, int rst, int sleep);

		void begin();

		void step();

		void update();

		void set_position(double position){m_pos = position; update();}

		void set_speed(unsigned long speed){m_speed = speed;}

		float get_position(){return m_pos;}

		float get_speed(){return m_speed;}

		void enable();
		void disable();

	private:

		int m_step;
		int m_sleep;
		int m_dir;
		int m_rst;
		int m_enable;

		float m_current_pos;
		float m_last_pos;

		float m_pos;
		unsigned long m_speed;
		int   m_direction;
		bool m_en;
};


#endif // __STEPBYSTEPMOTOR_H__