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

namespace Zepco {
	class Blinker {
		struct Blink {
			int count, onTime, beetweenTime, delayTime;
		};

		enum Step {STOPPED, LED_ON, LED_OFF, DELAY};

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
			uint8_t _ledPin;
			bool _ledInversion = false;
			unsigned long _lastMillis = 0;
			int	_interval = 0;
			Blink* _currentBlink = NULL;
			Blink* _nextBlink = NULL;
			Step _step = STOPPED;
			int _stepCount = 0;

			bool _logActivity = false;
			Print* _logPrinter = NULL;

			void _log(String msg);
	};
}

#endif // _ZEPCO_BLINK_H
