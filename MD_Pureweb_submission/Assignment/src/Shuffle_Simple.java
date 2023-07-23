    import java.util.*;
public class Shuffle_Simple {
    public static void main(String[] args) {

// Create a Scanner object to read input from the keyboard

        Scanner scanner = new Scanner(System.in);
        System.out.print("Enter your Number: ");
        int N = scanner.nextInt();
        scanner.close();

//start of timer

        long starting = System.currentTimeMillis();
        List<Integer> shuffledList = generateRandomList(N);
        long end = System.currentTimeMillis();

//Print statements : the array , time taken , The input given by user

        System.out.println("Shuffled List:");
        for (int num : shuffledList) {
            System.out.print(num + " ");
        }
        long totalTime = end - starting;
        System.out.println("\nTime taken: " + totalTime + " milliseconds");
         System.out.println("Hello, " + "You Number is " + N);

    }

// Aim: Genrates a list of N integers and then randomly swaps the index places
// Input: 5
// Output: 2,3,5,4,1


    public static List<Integer> generateRandomList(int N) {

        List<Integer> list = new ArrayList<>();
        for (int i = 1; i <= N; i++) {
            list.add(i);
        }
      
        Random random = new Random();
        //Map<Integer, Integer> relationMap = new HashMap<>(); // the indexs where swaps happened 
        for (int i = 0; i < N; i++) {
            
            int randIndex = random.nextInt(N - i) + i;
            Collections.swap(list, i, randIndex);

// Here I have attended to write code to track the index swap but this was out of the requirements and increases the time taken. 
// int num_randIndex = relationMap.getOrDefault(randIndex, randIndex);
// int num_CurrentIndex = relationMap.getOrDefault(i, i);
// // this condition is used to speed up and avoid the condition where the current index and random index is the same. it just  
// if(!(randIndex==num_randIndex && num_randIndex==num_CurrentIndex)){
// relationMap.put(i, num_randIndex);
// relationMap.put(randIndex, num_CurrentIndex);  
// Collections.swap(list, i, randIndex);
//}

        }

        return list;
}
}

// Complextity 
// Speed O(N) = As generateRandomList() has two for loops which are not nested. 
//Space O(N) = As I am making an array list of N size