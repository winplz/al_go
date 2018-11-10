package test;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.locks.ReentrantLock;

import javax.print.attribute.standard.JobKOctetsSupported;

public class UnsafeIncrement implements Runnable {

	public static int total = 0;
	
	@Override
	public void run() {
		for(int i =0 ; i < 10000; i++) {
			total ++;
		}
	}
	
	public static String getResult() {
		return String.format("total = [%d]", total);
	}
	
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		Runnable runnable1 = new UnsafeIncrement();
		Runnable runnable2 = new UnsafeIncrement();
		
		ExecutorService executorService = Executors.newFixedThreadPool(8);
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
		
		int processors = Runtime.getRuntime().availableProcessors();
		int parallelProcess = processors * 2;
		int count = 0 ;
		for(int i=0; i < parallelProcess; i++) {
		 count ++ ;
		}
		
		System.out.println("CPU 갯수 : " + count);
	}

}
