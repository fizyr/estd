namespace estd {

/// Marker that you can embed in your own struct to detect if it has been moved from.
/**
 * The marker is explicitly convertible to bool.
 * If the marker has been moved from, the conversion results in true.
 * Otherwise, it results in false.
 *
 * If you move or copy *into* a marker it takes over the value of the other marker.
 */
class move_marker {
private:
	bool moved_from_;

public:
	/// Create a new move marker which indicates that it hasn't been moved yet.
	move_marker() {
		moved_from_ = false;
	}

	/// The copy constructor copies the value of the other marker.
	move_marker(move_marker const &) = default;

	/// The move constructor takes over the value of the other marker and then marks the *other* marker as moved.
	/**
	 * If other is the same marker, nothing is done.
	 */
	move_marker(move_marker && other) {
		if (&other == this) return;
		moved_from_ = other.moved_from_;
		other.moved_from_ = true;
	}

	/// THe copy assignment operator takes over the value of the other marker.
	move_marker & operator=(move_marker & other) {
		moved_from_ = other.moved_from_;
		return *this;
	}

	 /// The move assignment operator takes over the value of the other marker and then marks the *other* marker as moved.
	/**
	 * If other is the same marker, nothing is done.
	 */
	move_marker & operator=(move_marker && other) {
		if (&other == this) return *this;
		moved_from_ = other.moved_from_;
		other.moved_from_ = true;
		return *this;
	}

	/// Check if this marker has been moved from.
	/**
	 * True if this marker has been moved from. False otherwise.
	 */
	explicit operator bool() const {
		return moved_from_;
	}
};

}
