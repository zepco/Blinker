/*
 * Blinker.cpp
 *
 * @author	Zepco
 * @email	dev@zepco.pl
 * @version	0.1
 */
#include <Arduino.h>
#include "Blinker.h"

namespace Zepco {
	/**
	 * Konstruktor
	 *
	 * @param	int	pin		Numer pinu pod który podpięta jest dioda LED
	 */
	Blinker::Blinker(uint8_t pin) {
		setLedPin(pin);
	}

	/**
	 * Konstruktor
	 *
	 * @param	int		pin			Numer pinu pod który podpięta jest dioda LED
	 * @param	bool	inversion	Odwrócenie stanu diody
	 */
	Blinker::Blinker(uint8_t pin, bool inversion) {
		setLedPin(pin);
		setLedInversion(inversion);
	}

	/**
	 * Ustawia numer pinu pod którym jest wpięta dioda LED
	 *
	 * @param	int	pins
	 * @return	void
	 */
	void Blinker::setLedPin(uint8_t pin) {
		_ledPin = pin;
	}

	/**
	 * Ustawia flagę odwracającą działanie diody, przydatne w przypadku
	 * zapalania diody stanem niskim (LOW)
	 *
	 * @param	bool	inversion
	 * @return	void
	 */
	void Blinker::setLedInversion(bool inversion) {
		_ledInversion = inversion;
	}

	/**
	 * Dodaje jedno lub więce mrugnięć
	 *
	 * @param	int blinkCount		Ilość mrugnięć w serii
	 * @param	int	onTime			Czas załączenia diody
	 * @param	int	beetweenTime	Czas pomiędzy mrugnięciami
	 * @param	int	delayTime		Przerwa do kolejnej serii mrugnięć
	 * @return	void
	 */
	void Blinker::add(int blinkCount, int onTime,
						int beetweenTime, int delayTime) {
		if (_nextBlink != NULL) {
			delete _nextBlink;
			_nextBlink = NULL;
		}

		_nextBlink = new (Blink) {blinkCount, onTime, beetweenTime, delayTime};
	}

	/**
	 * Zapala diodę
	 *
	 * @return	void
	 */
	void Blinker::ledOn() {
		_log("[Zepco::Blinker] Led ON");

		if (_ledInversion) {
			digitalWrite(_ledPin, LOW);
		} else {
			digitalWrite(_ledPin, HIGH);
		}
	}

	/**
	 * Gasi diodę
	 *
	 * @return	void
	 */
	void Blinker::ledOff() {
		_log("[Zepco::Blinker] Led OFF");

		if (_ledInversion) {
			digitalWrite(_ledPin, HIGH);
		} else {
			digitalWrite(_ledPin, LOW);
		}
	}

	/**
	 * Włącza logowanie zdarzeń
	 *
	 * @return void
	 */
	void Blinker::enableLog() {
		_logActivity = true;
	}

	/**
	 * Ustawia obiekt drukujący logi
	 * @param	Print	p	Wskaźnik "drukarki"
	 */
	void Blinker::setLogPrinter(Print &p) {
		_logPrinter = &p;
	}

	/**
	 * Inicjuje wszystkie niezbędne rzeczy do działania obiektu
	 */
	void Blinker::init() {
		_log("[Zepco::Blinker] Init");
		pinMode(_ledPin, OUTPUT);
	}

	/**
	 * Aktualizuje stan diody LED
	 *
	 * @return	void
	 */
	void Blinker::update() {
		if (_currentBlink == NULL) {
			if (_nextBlink == NULL)
				return;

			_currentBlink = _nextBlink;
			_nextBlink = NULL;
		}

		unsigned long now = millis();

		if (_step == STOPPED)
		{
			_stepCount = (*_currentBlink).count;
			_interval = (*_currentBlink).onTime;
			_lastMillis = now;
			_step = LED_ON;
			ledOn();
		}

		if (now - _lastMillis > _interval) {
			switch (_step) {
				case LED_ON:
					ledOff();
					_stepCount--;
					if (_stepCount == 0) {
						_step = DELAY;
						_interval = (*_currentBlink).delayTime;
					} else {
						_step = LED_OFF;
						_interval = (*_currentBlink).beetweenTime;
					}

				break;

				case LED_OFF:
					ledOn();
					_interval = (*_currentBlink).onTime;
					_step = LED_OFF;
				break;

				case DELAY:
					_step = STOPPED;
					delete _currentBlink;
					_currentBlink = NULL;
				break;
			} // switch

			_lastMillis = now;
		} // if
	}

	/**
	 * Wysyła wiadomość logów do "drukarki"
	 * @param	String	msg		Wiadomość
	 */
	void Blinker::_log(String msg) {
		if (_logActivity && _logPrinter)
			_logPrinter->println(msg);
	}
} // namespace Zepco
