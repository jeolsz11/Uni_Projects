/*------------------------------------------------------------------------
Name: Jennifer Olszyna     
Student ID: 006439880002    
Email: jolszyna@una.edu    
Course Information: CIS 315-03    
Program Source File Name: CustomerLabTesting.java    
Programming Assignment: Project 3    
Program Description:  Program to manage a clinic's customers 
and their payments. 
References: PowerPoints
Due Date: 12/7/21 at 11:59pm   

In keeping with the honor code policies of the University of University of 
North Alabama, the School of Business, and the Department of Computer 
Science, I affirm that I have neither given nor received on this programming 
assignment. This assignmentrepresents my individual, original effort. 
... My Signature is on File.
------------------------------------------------------------------------*/

// Import statement for the Scanner class
import java.util.Scanner;
import java.util.ArrayList;

public class CustomerLabTesting {
    //Globel Variables (ones that can be used by all methods)
    //Create a Scanner object to read input.
    public static Scanner keyboard = new Scanner(System.in); 

// -------------------- Main --------------------   
	public static void main(String[] args){
        int num_add;
        
        System.out.println("\n - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -");
        System.out.println("                        Pillbox Clinic Patient Registry");
        System.out.println(" - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -");
        
        System.out.print("\n>> How many customers needs to be added?: ");
		num_add = keyboard.nextInt();
		
		for (int i = 0; i < num_add; i++) {
            //object "customer" of the Customer class
		    Customer customer = new Customer();
		    
		    customer.addLabTest();
		    customer.payBalance();
		    customer.displayEntry();
        }
    }
}

class Customer {
    //Globel Variables (ones that can be used by all methods)
    //Create a Scanner object to read input.
    public static Scanner keyboard = new Scanner(System.in); 

// Private fields
	private String first_name = " ";
	private String last_name = " ";
	private int depart_num = 0;
	ArrayList<String> test_des = new ArrayList<String>();
	private int balance = 0;
	private int cost_test = 120;
	private int ID = 10000;
	private String customer_ID = " ";
// Constructor 
    public Customer() {
		System.out.print("\n>> Customer first name: ");
		first_name = keyboard.nextLine();
		System.out.print(">> Customer last name: ");
		last_name = keyboard.nextLine();
		System.out.println("    Departments:   1. Microbiology  2. Virology  3. Hematology  4. Parasitology");
		System.out.print(">> Department number: ");
		depart_num = keyboard.nextInt();
		customer_ID = setCustomerID();
	    
	}
    
// -------------------- Methods --------------------
//method to Generate Customer ID
    private String setCustomerID() {
        //concatenate ints by having compiler convert them to strings
        customer_ID = depart_num + "" + ID;
        return customer_ID;
    }
    
//method to Add the lab tests and finding the amount to be paid by the customer
    public void addLabTest() {
        String test = " ";
        // Loop to add the lab tests  
        while (!test.toUpperCase().equals("Q"))
        {
            System.out.print(">> Test type (q if none): ");
            test = keyboard.nextLine();
            
            if (!test.toUpperCase().equals("Q")) {
                test_des.add(test);
                // Every time a lab test is added the balance to be paid by the customer must increase.   
                balance = balance + cost_test;
            }    
        }
    }
    
//method to View the balance
    public void viewBalance() {
        System.out.println("    Current balance: $" + balance);
    }  
    
//method to Pay the balance
    public void payBalance() {
        int payment;
        
        // view the balance to be paid 
        viewBalance();
        
        // prompt and get user input for the payment. 
        System.out.print(">> Payment amount: ");
		payment = keyboard.nextInt();
        
        balance = balance - payment;
        
        //Display the amount paid by the customer.
        System.out.println("    Payment of $" + payment + " accepted.");
    
        //Call the viewBalance method to show the remaining amount to be paid (if any) by the customer. 
        viewBalance();
    } 
    
//method to Show the information
    public void displayEntry() {
        System.out.println("\n - - - - - - - - - - - - - - - - - - - -");
        System.out.println("    Customer info: " + first_name + " " + last_name);
        System.out.println("    Customer ID: " + customer_ID);
        switch (depart_num) {
            case 1:
                System.out.println("    Department of Microbiology");
                break;
            case 2:
                System.out.println("    Department of Virology");
                break;
            case 3:
                System.out.println("    Department of Hematology");
                break;
            case 4:
                System.out.println("    Department of Parasitology");
                break;
            default:
                break;
        }
        for(int i = 0; i < test_des.size(); i++) {
            System.out.println("    " + test_des.get(i));    
        } 
        viewBalance();
    } 
}