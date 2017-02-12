/**
 * Blinker.h
 *
 * @author	Zepco
 * @email	dev@zepco.pl
 * @version	0.1
 */
#ifndef _ZEPCO_BLINK_H
#define _ZEPCO_BLINK_H

#include <Arduino.h>

#define	STEP_NOTHING	0
#define	STEP_LED_ON		1
#define STEP_LED_OFF	2
#define STEP_DELAY		3

namespace Zepco {
	class Blinker {
		struct Blink {
			int count, onTime, beetweenTime, delayTime;
		};

		public:
			Blinker(uint8_t pin);
			Blinker(uint8_t pin, bool inversion);

			void setLedPin(uint8_t pin);
			void setLedInversion(bool inversion);
			void add(int blinkCount, int onTime,
						int beetweenTime, int delayTime);
			void ledOn();
			void ledOff();
			void enableLog();
			void setLogPrinter(Print &p);
			void init();
			void update();

		private:
			uint8_t _ledPin = 2;
			bool _ledInversion = false;
			unsigned long _lastMillis = 0;
			int	_interval = 0;
			Blink* _currentBlink = NULL;
			Blink* _nextBlink = NULL;
			int _step = STEP_NOTHING;
			int _stepCount = 0;

			bool _logActivity = false;
			Print* _logPrinter = NULL;

			void _log(String msg);
	};
}

#endif // _ZEPCO_BLINK_H
