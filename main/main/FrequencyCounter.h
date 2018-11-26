#pragma once
#include <avr/interrupt.h>
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
namespace lyncs
{

class FrequencyCounter
{
  private:
	unsigned long FreqCounter::f_freq;

	unsigned char f_ready;
	unsigned char f_mlt;
	unsigned int f_tics;
	unsigned int f_period;
	unsigned int f_comp;

  public:
	FrequencyCounter();
	~FrequencyCounter();
	void Init(int ms);
};

FrequencyCounter::FrequencyCounter(/* args */)
{
}

FrequencyCounter::~FrequencyCounter()
{
}

void Init(int ms)
{
}

} // namespace lyncs