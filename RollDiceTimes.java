/*------------------------------------------------------------------------
Name: Jennifer Olszyna     
Student ID: 006439880002    
Email: jolszyna@una.edu    
Course Information: CIS 315-03    
Program Source File Name: RollDiceTimes.java    
Programming Assignment: lab 5     
Program Description: A program to roll a pair of standard die.
References: PowerPoints
Due Date: 10/11/21 at 11:59pm   

In keeping with the honor code policies of the University of University of 
North Alabama, the School of Business, and the Department of Computer 
Science, I affirm that I have neither given nor received on this programming 
assignment. This assignmentrepresents my individual, original effort. 
... My Signature is on File.
------------------------------------------------------------------------*/

// Import statement for the Scanner class
import java.util.Scanner;
import java.util.Random;

public class RollDiceTimes{
	public static void main(String[] args){

		// Declare variable for the input
		char cont;
		int a,b;	
		Random rand = new Random();

	 	// Create a Scanner object to read input.	
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