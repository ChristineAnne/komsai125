import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.Scanner;
import java.util.StringTokenizer;

// created by Christine Anne Catubig

public class MemoryAllocation{

	static final String file1 = "C:/Users/apple/Documents/CaH2O/komsaiPaMore/Komsai 125/Lab/lab 3/jobs.txt";
	static final String file2 = "C:/Users/apple/Documents/CaH2O/komsaiPaMore/Komsai 125/Lab/lab 3/memory.txt";

	static Scanner sc = new Scanner(System.in);

	static ArrayList<Job> jobs = new ArrayList<>();
	static ArrayList<Memory> memory_blocks = new ArrayList<>();

	static void clearScreen() throws IOException, InterruptedException{
		new ProcessBuilder("cmd", "/c", "cls").inheritIO().start().waitFor();
	}

	static void pause(){
		System.out.print("\n\n\t\t\t\tPress Enter...");
		sc.nextLine();
	}

	static void sleep(int ms) {
        try {
            Thread.sleep(ms);
        } catch (InterruptedException ex) {
            Thread.currentThread().interrupt();
        }
    }

	static void loadData() throws Exception{
		BufferedReader br;
		try{
			br = new BufferedReader(new FileReader(file1));
			String line = br.readLine();
			while(line != null){
				StringTokenizer st = new StringTokenizer(line, ",");
				while(st.hasMoreElements()){
					jobs.add(new Job(Integer.parseInt((String) st.nextElement()), Integer.parseInt((String) st.nextElement()), Integer.parseInt((String) st.nextElement())));
				}
				line = br.readLine();
			}
			br.close();

			br = new BufferedReader(new FileReader(file2));
			line = br.readLine();
			while(line != null){
				StringTokenizer st = new StringTokenizer(line, ",");
				while(st.hasMoreElements()){
					memory_blocks.add(new Memory(Integer.parseInt((String) st.nextElement()), Integer.parseInt((String) st.nextElement())));
				}
				line = br.readLine();
			}
			br.close();
		}catch(Exception e){
			e.printStackTrace();
		}
	}

	static private void display(int condition){
		System.out.print("\n\n\t\t\t\t");
		switch(condition){
			case 1:
				System.out.println("First-Fit");
				break;
			case 2:
				System.out.println("Best-Fit");
				break;
			case 3:
				System.out.println("Worst-Fit");
				break;
		}
		System.out.println("\n\n\t\tJob\tTime\tMemory Block\tFragmentation");
	}

	static private void display(ArrayList<Job> copy){
		System.out.println("\n\n\t\t\tJob\tWaiting Time");
		for(Job j : copy){
			if(j.getWaitingTime() == -1){
				System.out.println("\t\t\t " + j.getNumber() + "\t     -");	
			}else{
				System.out.println("\t\t\t " + j.getNumber() + "\t     " + j.getWaitingTime());
			}
			
		}
	}

	static private void fit(int condition) throws IOException, InterruptedException{ // condition = 1 if first-fit, condition = 2 if best-fit, condition = 3 if worst-fit
		ArrayList<Job> copy = new ArrayList<>();
		for(Job j : jobs){
			copy.add(new Job(j.getNumber(), j.getTime(), j.getSize()));
		}		

		ArrayList<Job> running = new ArrayList<>();
		int occupied = 0;
		int waitingTime = 0;
		for(int i = 0; i < copy.size(); i++){
			for(int j = 0; j < memory_blocks.size(); j++){
				if(!memory_blocks.get(j).isUsed()){
					if(copy.get(i).getSize() <= memory_blocks.get(j).getSize()){
						copy.get(i).setWaitingTime(waitingTime);
						copy.get(i).setBlock(memory_blocks.get(j).getBlock());
						running.add(copy.get(i));
						memory_blocks.get(j).setUsedStatus(true);
						memory_blocks.get(j).incrementUsedCounter();
						memory_blocks.get(j).incrementFragmentation(memory_blocks.get(j).getSize() - copy.get(i).getSize());
						occupied++;
						break;
					}	
				}
			}
			if(occupied == memory_blocks.size()){
				break;
			}
		}

		int mana = 0;
		while(mana < copy.size()-1){
			clearScreen();
			waitingTime++;
			display(condition);
			for(int i = 0; i < running.size(); i++){
				Job j = running.get(i);
				if(j.getTime() > 0){
					System.out.println("\t\t " + j.getNumber() + "\t  " + j.getTime() + "\t     " + j.getBlock() + "\t\t    " + (getBlock(j.getBlock()).getSize() - j.getSize()));
					j.setTime(j.getTime() - 1);
					int time = j.getTime();
					if(time <= 0){
						j.setStatus(false);
						mana += 1;
						replace(running, i, copy, waitingTime);
					}	
				}
			}
			display(copy);
			sleep(50);
		}
		double awt = 0.0, throughput = 0;
		int waitingJob = 0;
		for(Job j : copy){
			if(j.getTime() > 0){
				waitingJob++;
			}else{
				awt += j.getWaitingTime();
				throughput++;
			}
		}
		System.out.println("\n\t\t\t\tAverage Waiting Time: " + awt/(jobs.size() - waitingJob));
		System.out.println("\n\t\t\t\tWaiting Queue Length: " + waitingJob);
		System.out.println("\n\t\t\t\tThroughput: " + throughput/waitingTime);

		double totalFragmentation = 0;
		for(Memory m : memory_blocks){
			totalFragmentation += m.getFragmentation();
			m.setFragmentation(0);
		}
		System.out.println("\n\t\t\t\tOverall Fragmentation: " + totalFragmentation);
	}

	static Memory getBlock(int block){
		for(Memory m : memory_blocks){
			if(m.getBlock() == block){
				return m;
			}
		}
		return null;
	}

	static void replace(ArrayList<Job> running, int pos, ArrayList<Job> copy, int waitingTime){
		for(int l = 0; l < memory_blocks.size(); l++){
			if(running.get(pos).getBlock() == memory_blocks.get(l).getBlock()){
				memory_blocks.get(l).setUsedStatus(false);
			}
		}

		for(int i = 0; i < copy.size(); i++){
			Boolean okay = false;
			if(copy.get(i).getTime() > 0){
				Boolean found = false;
				for(int l = 0; l < running.size(); l++){
					if(copy.get(i).getNumber() == running.get(l).getNumber()){
						found = true;
						break;
					}
				}

				if(!found){
					for(int j = 0; j < memory_blocks.size(); j++){
						if(memory_blocks.get(j).isUsed() == false){
							if(copy.get(i).getSize() <= memory_blocks.get(j).getSize()){
								copy.get(i).setWaitingTime(waitingTime);
								copy.get(i).setBlock(memory_blocks.get(j).getBlock());
								running.set(pos, copy.get(i));
								memory_blocks.get(j).setUsedStatus(true);
								memory_blocks.get(j).incrementUsedCounter();
								okay = true;
							}
						}
						if(okay){
							break;
						}
					}
				}
			}
			if(okay){
				break;
			}
		}
	}
	
	static void setUsedCounterToZero(){
		for(Memory m : memory_blocks){
			m.setUsedCounter(0);
		}
	}

	static void firstFit() throws IOException, InterruptedException{
		fit(1);
		pause();
	}

	static void bestFit() throws IOException, InterruptedException{
		setUsedCounterToZero();
		Collections.sort(memory_blocks, Memory.sortAscending);
		fit(2);
		pause();
	}

	static void worstFit() throws IOException, InterruptedException{
		setUsedCounterToZero();
		Collections.sort(memory_blocks, Memory.sortDescending);
		fit(3);
	}

	public static void main(String[] args) throws Exception, IOException, InterruptedException{
		loadData();
		firstFit();
		bestFit();
		worstFit();
		sc.close();
	}
}

class Job{
	private int number, time, size, block, waitingTime = -1;
	private Boolean status = false;

	public Job(int number, int time, int size){
		this.number = number;
		this.time = time;
		this.size = size;
	}

	public int getWaitingTime(){
		return waitingTime;
	}

	public void setWaitingTime(int time){
		waitingTime = time;
	}

	public Boolean getStatus(){
		return status;
	}

	public void setStatus(Boolean status){
		this.status = status;
	}

	public int getNumber(){
		return number;
	}

	public int getTime(){
		return time;
	}

	public int getSize(){
		return size;
	}

	public void setTime(int time){
		this.time = time;
	}

	public void setBlock(int block){
		this.block = block;
	}

	public int getBlock(){
		return block;
	}
}

class Memory{
	private int block_number, size, usedCounter, fragmentation = 0;
	private Boolean used;
	
	public Memory(int block_number, int size){
		this.block_number = block_number;
		this.size = size;
		used = false;
	}

	public int getFragmentation(){
		return fragmentation;
	}

	public void incrementFragmentation(int fragment){
		fragmentation += fragment;
	}

	public void setFragmentation(int f){
		fragmentation = f;
	}

	public int getUsedCounter(){
		return usedCounter;
	}

	public void incrementUsedCounter(){
		usedCounter++;
	}

	public void setUsedCounter(int counter){
		usedCounter = counter;
	}

	public int getBlock(){
		return block_number;
	}

	public int getSize(){
		return size;
	}

	public Boolean isUsed(){
		return used;
	}

	public void setUsedStatus(Boolean status){
		used = status;
	}

	public static Comparator<Memory> sortAscending = new Comparator<Memory>(){
		public int compare(Memory m1, Memory m2){
			int arr1 = m1.getSize();
			int arr2 = m2.getSize();

			return arr1-arr2;
	}};

	public static Comparator<Memory> sortDescending = new Comparator<Memory>(){
		public int compare(Memory m1, Memory m2){
			int arr1 = m1.getSize();
			int arr2 = m2.getSize();

			return arr2-arr1;
	}};
}