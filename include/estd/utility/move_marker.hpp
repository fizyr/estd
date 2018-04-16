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
	move_marker & operator=(move_marker const & other) {
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