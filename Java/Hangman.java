/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
Simple text based game of hangman 
Note: you need to have the text document with the word bank in 
the same folder
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

import java.io.File;
import java.io.FileNotFoundException; 
import java.util.Scanner;
import java.util.ArrayList;
import java.util.List;

public class Main {
    
    //Constants
    public static int MAX_WORD_SIZE = 15;
    public static int MAX_GUESSES = 6;
    public static int NUM_LETTERS = 26;
    public static int MAX_WORDS = 10;

    /******************************************************************
                          FUNCTION DEFINITIONS
    *****************************************************************/
    /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    Function:	  read_puzzles
    Description:  This function will read puzzeles in from a file
    			  and keep up with how many there are.
    Incoming:	  Array string words && wordCount(refrence)
    Outgoing:     N/A
    Return:       N/A
    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
    static void read_puzzles (String[] words, int &count)
    {
    	count = 0;
        try {
            File myObj = new File("puzzles.txt");
            Scanner myReader = new Scanner(myObj);
            while (myReader.hasNextLine()) {
                words[] = myReader.nextLine();
                count++;
            }
            myReader.close();
        } catch (FileNotFoundException e) {
            System.out.println("An error occurred while attmepting to open file");
            e.printStackTrace();
        }
    }
    
    /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    Function:	  display_puzzle
    Description:  This function displays a '_' for unguessed letters or 
    			  displays the letters if guessed.
    Incoming:	  string word && array of guessed(boolean)
    Outgoing:     N/A
    Return:       N/A
    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
    static void display_puzzle (String word, boolean[] guessed)
    {
    	int i;
    	for (i=0; i<word.length(); i++)
    		if (guessed[i]==false)
    			System.out.println("_ ");
    		else
    			System.out.println("word[i]");
    		System.out.println("");
    }
    
    /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    Function:	  found
    Description:  This function will find if a guess is in the word 
    Incoming:	  string word && guessed array(boolean) && guess(char)
    Outgoing:     N/A
    Return:       return find (true or false)
     - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
    static boolean found(String word, boolean[] guessed, char guess)
    {
    	boolean find = false;
    	int i;
    	for (i=0; i<word.length(); i++)
    			if (word[i]==guess)
    			{
    				find = true;
    				guessed[i] = true;				
    			}
    
    return find;
    }
    
    /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    Function:	  winner
    Description:  This function will see if all the guessed array is 
    			  true to determine if the word has been guessed.
    Incoming:	  guessed array(boolean) && letters(int)
    Outgoing:     N/A
    Return:       winner (true or false)
    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
    static boolean winner(boolean[] guessed, int letters)
    {
    	for (int i=0; i<letters; i++)
    		if (guessed[i] == false)
    			return false;
    	
        return true; //outside for loop
    			
    }
    
    
    /******************************************************************
                             MAIN FUNCTION
    *****************************************************************/
    public static void main(String[] args) {
        String alpha = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"; 
        String[] words; 
        int wordNum = 0, letters = 0, misses = 0, numWords = 0;
        char guess;
        boolean[] guessed;
        
        read_puzzles(words, numWords);
        
        //Random Puzzle
        srand(time(0));
        wordNum = rand() % numWords;
        letters = words[wordNum].length();
        
        for (int i=0; i<letters; i++){
            guessed[i] = false;
        }
        
        
        do
        {
            // Display puzzle, alpha, & get guesses
            display_puzzle(words[wordNum], guessed);
            System.out.println("Available:" + alpha);
            System.out.println("Guess: ");
            char ws = (char)System.in.read();
            ws.get(guess);
            guess = toupper(guess);
            
            // Take away guessed letters
            for (int i=0; i<NUM_LETTERS; i++)
                if (alpha[i] == guess)
                    alpha[i] = ' ';
            
            if (!(found(words[wordNum], guessed, guess)))
                misses++;
            System.out.println("Misses: " + misses);;
        
        } while (!winner(guessed,letters) && (misses < MAX_GUESSES));
        
        System.out.println("The word was: " + words[wordNum]);
        if (misses == MAX_GUESSES) {
            System.out.println("You lose!");
        }
        else {
            System.out.println("You win. Nice job!");
        }
    }
}
