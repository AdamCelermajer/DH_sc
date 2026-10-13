// Test for v1-vendor-gameswf-pack16-133bd3-0: gameswf::profiler. The four binary bodies are bx lr, so the
// test checks that each call links, runs and returns without side effects on the object.
#include "vendor-gameswf/profiler.hpp"

int main() {
	gameswf::profiler p;
	p.set_enabled(true);
	p.begin_event("frame");
	p.end_event();
	p.dump();
	p.set_enabled(false);
	return 0;
}
