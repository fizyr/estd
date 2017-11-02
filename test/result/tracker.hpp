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
