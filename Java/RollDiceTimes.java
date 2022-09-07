/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
A program to roll a pair of standard die.
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

import java.util.Scanner;
import java.util.Random;

public class RollDiceTimes{
	public static void main(String[] args){
		char cont;
		int a,b;	
		Random rand = new Random();
		
		Scanner keyboard = new Scanner(System.in);

		do{
			for( int i=1; i<7; i++ ){
				System.out.println("   Rolling the dice..." + i);
				System.out.println("   Their values are:");
				a = rand.nextInt(6) +  1;;
				b = rand.nextInt(6) +  1;;
				System.out.println( "   " + a + " " +  b);
			}
			// Prompt the user for information
        		System.out.println(">> Roll them again (y = yes)?: ");
				cont = keyboard.next().toCharArray()[0];
				cont = Character.toLowerCase(cont);
		} while(cont=='y');    
    }
}
