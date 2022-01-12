#include <C2nxt/StdIO.h>
#include <C2nxt/StdPlatform.h>
#include <C2nxt/StdVector.h>
#include <C2nxt/time/StdClock.h>
#include <time.h>

i32 main(i32 argc, char** argv) {

	ignore(argc, argv);

	println("beginning println vs printf benchmark");
	let num_runs = 1000000;
	let unum = 1024U;
	let snum = -1024;
	let fnum = -1024.1024;
	let_mut std_string_scoped string = std_string_from("This is a string");

	let_mut average_println = 0.0;
	let_mut average_printf = 0.0;

	for(let_mut i = 0; i < num_runs; ++i) {
		// NOLINTNEXTLINE(readability-identifier-length)
		let u = unum * static_cast(u32)(i);
		// NOLINTNEXTLINE(readability-identifier-length)
		let s = snum * i;
		// NOLINTNEXTLINE(readability-identifier-length)
		let f = fnum * i;
		let start = std_clock_now(&std_steady_clock);
		println("{}, {}, {d3}, {}", u, s, f, string);
		let end = std_clock_now(&std_steady_clock);
		average_println += static_cast(f64)(
			std_duration_subtract(end.time_since_epoch, start.time_since_epoch).count);
	}
	average_println = average_println / static_cast(f64)(num_runs);

	for(let_mut i = 0; i < num_runs; ++i) {
		// NOLINTNEXTLINE(readability-identifier-length)
		let u = unum * static_cast(u32)(i);
		// NOLINTNEXTLINE(readability-identifier-length)
		let s = snum * i;
		// NOLINTNEXTLINE(readability-identifier-length)
		let f = fnum * i;
		let start = std_clock_now(&std_steady_clock);
		printf("%u, %d, %.3f, %s\n", u, s, f, std_string_into_cstring(string));
		let end = std_clock_now(&std_steady_clock);
		average_printf += static_cast(f64)(
			std_duration_subtract(end.time_since_epoch, start.time_since_epoch).count);
	}
	average_printf = average_printf / static_cast(f64)(num_runs);

	let diff = average_println - average_printf;
	let relative_perf = average_printf / average_println;
	println("Run time for println (ns): {d}", average_println);
	println("Run time for printf (ns): {d}", average_printf);
	println("println - printf (ns): {d}", diff);
	println("Relative performance: {d}", relative_perf);

	return 0;
}
