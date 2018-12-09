#include <Arduino.h>
#include "../common/PeriodicProcess.h"
#include "configuration.h"
#include "eepromManagment.h"

#ifndef LEDMATRIX_H
#define LEDMATRIX_H

/** class Pattern
 *  \brief Gère les motifs à afficher
 *
 *  permet de gérer les messages / animations à afficher sur une LedMatrix. 
 * 
 */
class Pattern : public PeriodicProcess
{
	friend class LedMatrix;

public:

	void init();								// init the pattern
	void clearPatternToDisplay();				// Clear pattern to display array
	void clearPatterns();						// Clear _patterns array
	void setPattern();							// Set the current pattern into the patternToDisplay array
	void slidePattern();						// Slide the patternToDisplay array


private:

	byte _patternToDisplay[8];					// Pattern to display = {Row1,Row2,...,Row8}
	byte _patterns[NB_PATTERNS_MAX][8];			// Array of all the message to display
	byte _currentPattern;						// The position of the current pattern in the array _patterns
	byte _endOfPreviousPattern;					// Separation between 2 consecutive patterns being sliding
	byte _nbPatterns;							// Number of patterns having to be displayed
	byte _mode;									// mode of the matrix {SLIDE_MODE, ANIMATION_MODE}
	byte _patternWidth[NB_PATTERNS_MAX];		// The width of each pattern
	int _rotation;								// rotation of the pattern {0,90,180,270}
	int _defaultRotation;						// The default rotation of the pattern
	float _timeStep;

	virtual void process(float timestep);
	
};


class LedMatrix : public PeriodicProcess
{

  public:

    void attach(byte dataPin, byte clockPin, byte latchPin, int rotation, byte idMatrix);			// Attach a matrix to its pin
	void updateMatrix();																			// Send data to the registers
	void initMatrix();																				// Init the matrix data + update data into registers
	void computeBuffer(char buffer[]);																// Compute Serial input
	void enable();
	void disable();
	void update();
	void setMode(byte mode);		// Set mode of the matrix {SLIDE_MODE, ANIMATION_MODE}
	void changeTimeStep(float timeStep);

  private:

    byte _DATAPIN;					// Matrix register data pin
    byte _CLOCKPIN;					// Matrix register clock pin
    byte _LATCHPIN;					// Matrix register latch pin
	int _data;						// Data to send to the registers
	int _maskColumns;				// Mask to identify Columns into _data
	byte _actualColumn;				// Column to be updated
	Pattern _pattern;
	

    virtual void process(float timestep);
    void display();
};



#endif //LEDMATRIX_H