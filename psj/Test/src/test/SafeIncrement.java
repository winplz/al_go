package test;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicInteger;

public class SafeIncrement implements Runnable {

	public static AtomicInteger total = new AtomicInteger(0);

	@Override
	public void run() {
		for (int i = 0; i < 10000; i++) {
			total.incrementAndGet();
		}
	}

	public static String getResult() {
		return String.format("total = [%d]", total.get());
	}

	public static void main(String[] args) {
		Runnable runnable1 = new SafeIncrement();
		Runnable runnable2 = new SafeIncrement();

		ExecutorService executorService = Executors.newFixedThreadPool(2);
		executorService.execute(runnable1);
		executorService.execute(runnable2);

		executorService.shutdown();
		try {
			if(!executorService.awaitTermination(5, TimeUnit.MINUTES)) {
				executorService.shutdownNow();
			}
		} catch (InterruptedException e) {
			e.printStackTrace();
			executorService.shutdownNow();
		}
		System.out.println(getResult());
	}
}
