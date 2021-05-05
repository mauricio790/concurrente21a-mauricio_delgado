main:
	shared a1_ready := semaphore(0)
	create_thread(thread_a)
	create_thread(thread_b)

thread_a:
	statement a1
	signal(a1_ready)	
thread_b:
	wait(a1_ready)
	statement b1