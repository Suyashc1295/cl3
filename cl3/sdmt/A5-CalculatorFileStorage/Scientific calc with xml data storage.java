// Title: Scientific calculator with XML data storage

// File: App.java
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.PrintWriter;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JTextField;

import org.w3c.dom.*;
import org.xml.sax.SAXException;

import javax.xml.parsers.*;

import java.io.*;


public class App implements ActionListener
{
	JFrame frame;
	JPanel panel;
	JTextField ansfield;
	JButton buttons[];
	String buttonsText[]={"C","MC","MR","M+","M-","sqrt","X^2","1/X","SIN","COS","TAN","+/-","0","1","2","3","4","5","6","7","8","9","+","-","*","/",".","=", "READ"};
int maxx=400,maxy=550;


static final String DIGITS = "0123456789.";
 Boolean userIsInTheMiddleOfTypingANumber = false;
 CalculatorBrain mCalculatorBrain=new CalculatorBrain();

	public App() {
		frame = new JFrame("Scientific Calculator");
		frame.setSize(maxx, maxy);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

		panel = new JPanel();
		panel.setLayout(null);
		
		ansfield = new JTextField();
		ansfield.setBounds(10,30,maxx-40,40);
		ansfield.setHorizontalAlignment(JTextField.RIGHT);


		buttons=new JButton[buttonsText.length];

		int currentx=0,currenty=0;
		
		for(int i=0;i<buttonsText.length;i++)
		{
			buttons[i]=new JButton(buttonsText[i]);
			buttons[i].addActionListener(this);
			if(currentx==0&&currenty==0)
			{
				buttons[i].setBounds(10,100,70,30);
				currentx=10;
				currenty=100;
			}
			else
			{
				if(currentx<maxx-100)
				{
				currentx+=100;
				buttons[i].setBounds(currentx,currenty,70,30);
				}
				else
				{
					currentx=10;
					currenty+=50;
					buttons[i].setBounds(currentx,currenty,70,30);	
				}
			
			}
		panel.add(buttons[i]);
		}
		
		panel.add(ansfield);
		frame.add(panel);
		frame.setVisible(true);
	}
	public static void main(String args[])
	{

		new App();	
	}
	@Override
	public void actionPerformed(ActionEvent ae) {
		// TODO Auto-generated method stub
		String buttonObj="";		

		for(int i=0;i<buttonsText.length;i++)
		{
			if(ae.getSource()==buttons[i])
			{
			buttonObj=buttons[i].getText().toString();
			
			break;
			}
		}
		
		if (buttonObj.equals("READ")) {
			readFromXML();
		}
		else {
			calc(buttonObj);			
		}
	}
	
	private void readFromXML() {
		String line, temp;
        BufferedReader in;

		DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
		try {
			DocumentBuilder builder = factory.newDocumentBuilder();
	        in = new BufferedReader(new FileReader("/home/sameer/Downloads/cyber security codes and writeups/EncryptPassword/encryptpassword/src/data.xml"));
	        temp = in.readLine();
	        line = "";
	        while (temp != null) {
	        	line += temp;
	        	temp = in.readLine();
	        }
	        
			ByteArrayInputStream input =  new ByteArrayInputStream(line.getBytes());
			Document doc = builder.parse(input);
			NodeList nList = doc.getElementsByTagName("op");
			
			for (int i = 0; i < nList.getLength(); ++ i) {
				Node op = nList.item(i);
				calc(op.getTextContent());
			}
		} catch (ParserConfigurationException e) {
			System.out.println("Problem with XML file.");
			e.printStackTrace();
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		} catch (SAXException e) {
			e.printStackTrace();
		}
	}
	
	private void calc(String buttonObj) {
		if (DIGITS.contains(buttonObj)) {

            
            if (userIsInTheMiddleOfTypingANumber) {

                if (buttonObj.equals(".") && ansfield.getText().toString().contains(".")) {
                    
                } else {
                    ansfield.setText(ansfield.getText()+buttonObj);
                }

            } else {

                if (buttonObj.equals(".")) {
                    
                   
                    ansfield.setText(0 + buttonObj);
                } else {
                    ansfield.setText(buttonObj);
                }

                userIsInTheMiddleOfTypingANumber = true;
            }

        } else {
            
            if (userIsInTheMiddleOfTypingANumber) {
                mCalculatorBrain.setOperand(Double.parseDouble(ansfield.getText().toString()));
                userIsInTheMiddleOfTypingANumber = false;
            }
            try {
            	mCalculatorBrain.performOperation(buttonObj);
            } catch(Exception e){}
            	ansfield.setText(""+mCalculatorBrain.getResult());
        }
	}

public class CalculatorBrain {
    private double mOperand;
    private double mWaitingOperand;
    private String mWaitingOperator;
    private double mCalculatorMemory;


    public static final String ADD = "+";
    public static final String SUBTRACT = "-";
    public static final String MULTIPLY = "*";
    public static final String DIVIDE = "/";

    public static final String CLEAR = "C" ;
    public static final String CLEARMEMORY = "MC";
    public static final String ADDTOMEMORY = "M+";
    public static final String SUBTRACTFROMMEMORY = "M-";
    public static final String RECALLMEMORY = "MR";
    public static final String SQUAREROOT = "sqrt";
    public static final String SQUARED = "X^2";
    public static final String INVERT = "1/X";
    public static final String TOGGLESIGN = "+/-";
    public static final String SINE = "SIN";
    public static final String COSINE = "COS";
    public static final String TANGENT = "TAN";
    
    PrintWriter writer;
    public CalculatorBrain() {
        mOperand = 0;
        mWaitingOperand = 0;
        mWaitingOperator = "";
        mCalculatorMemory = 0;
    }

    public void setOperand(double operand) {
        mOperand = operand;
    }

   public double getResult() {
        return mOperand;
    }
   
    protected double performOperation(String operator) throws Exception {

        if (operator.equals(CLEAR)) {
            mOperand = 0;
            mWaitingOperator = "";
            mWaitingOperand = 0;
            // mCalculatorMemory = 0;
        } else if (operator.equals(CLEARMEMORY)) {
            mCalculatorMemory = 0;
            
            writer = new PrintWriter("memoryFile.txt", "UTF-8");
            writer.println(""+mCalculatorMemory);
            
            writer.close();
        } else if (operator.equals(ADDTOMEMORY)) {
            mCalculatorMemory = mCalculatorMemory + mOperand;
            
        
        } else if (operator.equals(SUBTRACTFROMMEMORY)) {
            mCalculatorMemory = mCalculatorMemory - mOperand;
            
        
        } else if (operator.equals(RECALLMEMORY)) {
            mOperand = mCalculatorMemory;
        } else if (operator.equals(SQUAREROOT)) {
            mOperand = Math.sqrt(mOperand);

        } else if (operator.equals(SQUARED)) {
            mOperand = mOperand * mOperand;

        } else if (operator.equals(INVERT)) {
            if (mOperand != 0) {
                mOperand = 1 / mOperand;
            }
        } else if (operator.equals(TOGGLESIGN)) {
            mOperand = -mOperand;
        } else if (operator.equals(SINE)) {
            mOperand = Math.sin(Math.toRadians(mOperand)); 
            
        } else if (operator.equals(COSINE)) {
            mOperand = Math.cos(Math.toRadians(mOperand)); 
        } else if (operator.equals(TANGENT)) {
            mOperand = Math.tan(Math.toRadians(mOperand)); 
        } else {
            performWaitingOperation();
            mWaitingOperator = operator;
            mWaitingOperand = mOperand;
        }
        
        return mOperand;
    }

    protected void performWaitingOperation() {

        if (mWaitingOperator.equals(ADD)) {
            mOperand = mWaitingOperand + mOperand;
        } else if (mWaitingOperator.equals(SUBTRACT)) {
            mOperand = mWaitingOperand - mOperand;
        } else if (mWaitingOperator.equals(MULTIPLY)) {
            mOperand = mWaitingOperand * mOperand;
        } else if (mWaitingOperator.equals(DIVIDE)) {
            if (mOperand != 0) {
                mOperand = mWaitingOperand / mOperand;
            }
        }

    }
}

}

// File: data.xml
<?xml version="1.0"?>

<data>
	<op>1</op>
	<op>2</op>
	<op>3</op>
	<op>+</op>
	<op>1</op>
	<op>=</op>
</data>

// Output
6
