#include <C2nxt/StdIO.h>
#include <C2nxt/StdPlatform.h>
#include <C2nxt/StdVector.h>
#include <time.h>

i32 main(i32 argc, char** argv) {

	ignore(argc, argv);

	println("beginning println vs printf benchmark");
	let num_runs = 100000;
	let_mut average_println_times = 0.0;
	let_mut average_printf_times = 0.0;
	let unum = 1024U;
	let snum = -1024;
	let fnum = -1024.1024;
	let secs_to_nanos = static_cast(f64)(10e9F);
	let_mut std_string_scoped string = std_string_from("This is a string");

	let_mut start_time = (struct timespec){};
	let_mut end_time = (struct timespec){};

	for(let_mut i = 0; i < num_runs; ++i) {
		let u = unum * static_cast(u32)(i);
		let s = snum * i;
		let f = fnum * i;
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_time);
		println("{}, {}, {d3}, {}", u, s, f, string);
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_time);
		average_println_times
			+= static_cast(f64)(end_time.tv_sec - start_time.tv_sec) * secs_to_nanos
			   + static_cast(f64)(end_time.tv_nsec - start_time.tv_nsec);
	}
	average_println_times /= static_cast(f64)(num_runs);

	for(let_mut i = 0; i < num_runs; ++i) {
		let u = unum * static_cast(u32)(i);
		let s = snum * i;
		let f = fnum * i;
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_time);
		printf("%u, %d, %.3f, %s\n", u, s, f, std_string_into_cstring(string));
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_time);
		average_printf_times
			+= static_cast(f64)(end_time.tv_sec - start_time.tv_sec) * secs_to_nanos
			   + static_cast(f64)(end_time.tv_nsec - start_time.tv_nsec);
	}
	average_printf_times /= static_cast(f64)(num_runs);

	let diff = average_println_times - average_printf_times;
	let relative_perf = average_printf_times / average_println_times;
	println("Run time for println: {d}", average_println_times);
	println("Run time for printf: {d}", average_printf_times);
	println("Println - printf: {d}", diff);
	println("Relative performance: {d}", relative_perf);

	return 0;
}
