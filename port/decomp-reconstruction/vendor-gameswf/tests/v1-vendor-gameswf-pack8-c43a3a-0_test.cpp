// Test for batch v1-vendor-gameswf-pack8-c43a3a-0: gameswf::stream constructor (DH2, two arguments) and
// destructor. Returns non-zero on failure.

#include <cstdio>

#include "gameswf/gameswf_stream.h"

namespace {

int failures = 0;

void check(bool ok, const char* what) {
	if (!ok) {
		std::printf("FAIL: %s\n", what);
		failures++;
	}
}

}  // namespace

int main() {
	// The stream stores its input pointer (+0x04 in the binary). Any address works; the stream never reads it here.
	int marker = 0;
	tu_file* input = reinterpret_cast<tu_file*>(&marker);

	{
		gameswf::stream s(input, true);
		check(s.get_underlying_stream() == input, "stream(input, true) stores the input pointer");
	}
	{
		gameswf::stream s(nullptr, false);
		check(s.get_underlying_stream() == nullptr, "stream(nullptr, false) stores a null input");
	}
	{
		// Construct and destroy several times: the DH2 array<char> (+0x1c, reserved to 0x100) and array<int>
		// (+0x0c) are released by the implicit member destructors.
		for (int i = 0; i < 4; i++) {
			gameswf::stream s(input, i & 1);
			check(s.get_underlying_stream() == input, "repeated construction keeps the input pointer");
		}
	}

	if (failures == 0) {
		std::printf("pack8-c43a3a-0 stream tests: all passed\n");
	}
	return failures == 0 ? 0 : 1;
}
