/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 A program to calculate the area of differnt shapes.
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

import java.util.Scanner;
import java.util.Random;

public class GeometryCal{
    public static Scanner keyboard = new Scanner(System.in); 

// -------------------- Main --------------------   
	public static void main(String[] args){
		int c;//the user's choice from the menu
		
		//loop that calls the getChoice and processChoice methods
		do{
		    c = getChoice();
		    processChoice(c);
		} while(c != 4);    
    }

	// -------------------- Methods --------------------
	//method to display menu and prompt user for choice  
	public static int getChoice(){
		System.out.println("\nThis is a Geometry calculator. Choose what you would like to calculate.");
		System.out.println("1. Find the area of a circle");
		System.out.println("2. Find the area of a rectangle");
		System.out.println("3. Find the area of a triangle");
		System.out.print(">>(type 1, 2, 3, or 4): ");
		return keyboard.nextInt(); //return the user's choice
    }
    
	//method to take in and process user's choice  
    public static void processChoice(int c){
        double area = 0;  //the value returned from the methods
        double r;  //the radius of the circle
        double l;  //the length of the rectangle
        double w;  //the width of the rectangle
        double h;  //the height of the triangle
        double b;  //the base of the triangle
        
        switch (c){
            case 1:
                System.out.print(">>Enter the radius of the circle: ");
                r = keyboard.nextDouble();
                area = circleArea(r);
                System.out.println("The area of the circle is: " + Math.round(area * 100.0) / 100.0);
                break;
            case 2:
                System.out.print(">>Enter the length of the rectangle: ");
                l = keyboard.nextDouble();
                System.out.print(">>Enter the width of the rectangle: ");
                w = keyboard.nextDouble();
                area = rectangleArea(l,w);
                System.out.println("The area of the rectangle is: " + Math.round(area * 100.0) / 100.0); 
                break;
            case 3:
                System.out.print(">>Enter the height of the triangle: ");
                h = keyboard.nextDouble();
                System.out.print(">>Enter the base of the triangle: "); 
                b = keyboard.nextDouble();
                area = triangleArea(h,b);
                System.out.println("The area of the triangle is: " + Math.round(area * 100.0) / 100.0);
                break;
            case 4:
                System.out.println("Bye");
                break;
            default:
                System.out.println( c + " is not an option, please choose 1, 2, 3, or 4.");
                break;
        }
    }
    
	//method to calculate the area of a circle    
    public static double circleArea(double r){
        return Math.PI * (r * r);
    }
   

	//method to calculate the area of a rectangle    
    public static double rectangleArea(double l, double w){
        return l * w;
    }
    

	//method to calculate the area of a triangle    
    public static double triangleArea(double h, double b){
        return 0.5 * b * h;
    }
}
