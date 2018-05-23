#include "PannelEffects.h"

/* Constuctor */
PannelEffects::PannelEffects()
{
	/* Led matrix initialisation */
	  LEDS.addLeds<CHIPSET, ENGR_PIN, COLOR_ORDER>(leds_engr,NUM_LEDS_ENGR).setCorrection(TypicalSMD5050);

  	LEDS.addLeds<CHIPSET, LOGO_PIN, COLOR_ORDER>(leds_logo,NUM_LEDS_LOGO).setCorrection(TypicalSMD5050);

  	LEDS.addLeds<CHIPSET, BAR_PIN, COLOR_ORDER>(leds_bar,NUM_LEDS_BAR).setCorrection(TypicalSMD5050);

  	LEDS.setBrightness(BRIGHTNESS);

    m_animations[DEFAULT_CODE] = fire_effect;
    m_animations[FIRE_CODE]    = fire_effect;
    m_animations[CYLON_CODE]   = cylon;
    m_animations[MATRIX_CODE]   = matrix;
}


/* public methodes */

void PannelEffects::set_bar_animation(int id)
{
    if(id < MAX_ANIMATION)
      m_id_bar = id;
}

void PannelEffects::set_engr_animation(int id)
{
    if(id < MAX_ANIMATION)
      m_id_engr = id;
}

void PannelEffects::set_logo_animation(int id)
{
    if(id < MAX_ANIMATION)
      m_id_logo = id;
}

int  PannelEffects::get_bar_animation()
{
    return m_id_bar;
}

int  PannelEffects::get_engr_animation()
{
    return m_id_engr;
}

int  PannelEffects::get_logo_animation()
{
    return m_id_logo;
}

void PannelEffects::execute()
{
    static int inc = 0;

    if(inc == 0)
    {
        m_animations[m_id_bar](leds_bar, NUM_LEDS_BAR);
        inc++;
    }
    else if(inc == 1)
    {
        m_animations[m_id_logo](leds_logo, NUM_LEDS_LOGO);
        inc++;
    }
    else if(inc == 2)
    {
        m_animations[m_id_engr](leds_engr, NUM_LEDS_ENGR);
        inc = 0;
    }
}
/* animattion func */

void fire_effect(CRGB * led_matrix, const int size)
{
	// Array of temperature readings at each simulation cell
   byte heat[size];

  // Step 1.  Cool down every cell a little
    for( int i = 0; i < size; i++) 
    {
      heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / size) + 2));
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= size - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < SPARKING ) 
    {
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(160,255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < size; j++) 
    {
      CRGB color = HeatColor( heat[j]);
      int pixelnumber;
      
      pixelnumber = (size-1) - j;

      led_matrix[pixelnumber] = color;
    }
    FastLED.show(); // display this frame
    FastLED.delay(1000 / FRAMES_PER_SECOND);
}

void fadeall(CRGB *led_matrix, const int size)
{ 
	for(int i = 0; i < size; i++) 
	{ 
		led_matrix[i].nscale8(250); 
	}
}

void cylon(CRGB * led_matrix, const int size)
{
	static uint8_t hue = 0;

  	// First slide the led in one direction
  	for(int i = 0; i < size; i++) 
  	{
    	// Set the i'th led to red 
    	led_matrix[i] = CHSV(hue++, 255, 255);
    	// Show the leds
    	FastLED.show(); 
    	// now that we've shown the leds, reset the i'th led to black
    	// leds[i] = CRGB::Black;
    	fadeall(led_matrix, size);
    	// Wait a little bit before we loop around and do it again
    	delay(10);
  	}

  	// Now go in the other direction.  
  	for(int i = (size)-1; i >= 0; i--) 
  	{
    	// Set the i'th led to red 
    	led_matrix[i] = CHSV(hue++, 255, 255);
    	// Show the leds
    	FastLED.show();
    	// now that we've shown the leds, reset the i'th led to black
    	// leds[i] = CRGB::Black;
    	fadeall(led_matrix, size);
    	// Wait a little bit before we loop around and do it again
    	delay(10);
  	}
}

/* MATRIX METHODS */

uint16_t XY(uint8_t x, uint8_t y)
{
	uint16_t i;

	if( y & 0x01) 
	{
		// Odd rows run backwards
		uint8_t reverseX = (KMATRIXWIDTH - 1) - x;
		i = (y * KMATRIXWIDTH) + reverseX;
	} 
	else 
	{
		// Even rows run forwards
		i = (y * KMATRIXWIDTH) + x;
	}
		  
	return i;
}

void matrix(CRGB * led_matrix, const int size)
{
    uint32_t ms = millis();
    int32_t yHueDelta32 = ((int32_t)cos16( ms * (27/1) ) * (350 / KMATRIXWIDTH));
    int32_t xHueDelta32 = ((int32_t)cos16( ms * (39/1) ) * (310 / KMATRIXHEIGHT));

    DrawOneFrame(led_matrix, ms / 65536, yHueDelta32 / 32768, xHueDelta32 / 32768);
    if( ms < 5000 ) {
      FastLED.setBrightness( scale8( BRIGHTNESS, (ms * 256) / 5000));
    } else {
      FastLED.setBrightness(BRIGHTNESS);
    }
    FastLED.show();
}
void DrawOneFrame(CRGB * led_matrix, byte startHue8, int8_t yHueDelta8, int8_t xHueDelta8)
{
  byte lineStartHue = startHue8;

  for( byte y = 0 ; y < KMATRIXHEIGHT; y++)
  {
      lineStartHue += yHueDelta8;
      byte pixelHue = lineStartHue; 

      for( byte x = 0; x < KMATRIXWIDTH; x++)
      {
          pixelHue += xHueDelta8;
          led_matrix[ XY(x, y)]  = CHSV( pixelHue, 255, 255);
      }
  }
}