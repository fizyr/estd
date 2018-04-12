/* Copyright 2017-2018 Fizyr B.V. - https://fizyr.com
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors
 *    may be used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once
#include <vector>
#include <iostream>

namespace estd {

enum class Event {
	value_initialized = 0,
	default_constructed,
	copy_constructed,
	move_constructed,
	copy_assigned_to,
	move_assigned_to,
	moved_from,
};


inline std::ostream & operator<< (std::ostream & stream, Event event) {
	switch (event) {
		case Event::value_initialized:   return stream << "value_initialized";
		case Event::default_constructed: return stream << "default_constructed";
		case Event::copy_constructed:    return stream << "copy_constructed";
		case Event::move_constructed:    return stream << "move_constructed";
		case Event::copy_assigned_to:    return stream << "copy_assigned_to";
		case Event::move_assigned_to:    return stream << "move_assigned_to";
		case Event::moved_from:          return stream << "moved_from";
	}
	return stream << "unknown_event_" << int(event);
}

using History = std::vector<Event>;

struct Tracker {
	std::vector<Event> history;

	Tracker() : history{Event::default_constructed} {
#ifdef DEBUG
		std::cerr << "Default constructed Tracker *" << this << "{}\n";
#endif
	};

	Tracker(Tracker const & other) : history{Event::copy_constructed} {
		(void) other;
#ifdef DEBUG
		std::cerr << "Copy-constructed Tracker *" << this << "{*" << &other << "}\n";
#endif
	};

	Tracker(Tracker && other) : history{Event::move_constructed} {
		other.history.push_back(Event::moved_from);
#ifdef DEBUG
		std::cerr << "Move-constructed Tracker *" << this << "{std::move(*" << &other << ")}\n";
#endif
	}

	void operator=(Tracker const & other)  {
		(void) other;
		history.push_back(Event::copy_assigned_to);
#ifdef DEBUG
		std::cerr << "Copy-assigned Tracker *" << this << " = *" << &other << "\n";
#endif
	};
	void operator=(Tracker && other) {
		history.push_back(Event::move_assigned_to); other.history.push_back(Event::moved_from);
#ifdef DEBUG
		std::cerr << "Move-assigned Tracker *" << this << " = std::move(*" << &other << ")\n";
#endif
	};

	bool operator==(Tracker const & other) const { return history == other.history; }
};

}
