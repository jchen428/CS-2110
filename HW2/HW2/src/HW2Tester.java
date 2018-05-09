import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.lang.reflect.Field;

import javax.swing.Timer;

/**
 * Tester for CS 2110 Homework 2
 * Fall 2014
 * 
 * @author Andrew Wilder
 * @version 3.0
 */
public class HW2Tester {

	/** The test cases - weight, name, and data */
	private static TestCase testCases[] = {
		new TestCase("strdtoi",        654,        "654",
		                        2147483647, "2147483647",
		                                 0,          "0",
		                         555555555,  "555555555"),
		                           
		new TestCase("strbtoi",          0,                               "0",
		                                11,                            "1011",
		                                72,                         "1001000",
		                        2147483647, "1111111111111111111111111111111"),
		                           
		new TestCase("strxtoi",          0,        "0",
		                                50,       "32",
		                          0xFEDCBA,   "FEDCBA",
		                        0x7FFFFFFF, "7FFFFFFF"),
		                           
		new TestCase("itostrb",                                "0",          0,
		                                                    "1110",         14,
		                                                  "101010",         42,
		                         "1111111111111111111111111111111", 2147483647),
		                           
		new TestCase("itostrx",        "0",          0,
		                              "10",         16,
		                            "FEED",      65261,
		                        "7FFFFFFF", 2147483647),
		                           
		new TestCase("setByte", 0x00000001, 0x00000000, 0x01, 0,
		                        0x33914480, 0x33912280, 0x44, 1,
		                        0x12CF5678, 0x12345678, 0xCF, 2,
		                        0xBEEFCAFE, 0x56EFCAFE, 0xBE, 3),
		                             
		new TestCase("getNibble", 0x9, 0x10100292, 1,
		                          0xA, 0xDEADBEEF, 5,
		                          0xF, 0xDEADBEEF, 0,
		                          0x1, 0x12345678, 7),
		                            
		new TestCase("pack", 0x12345678, 0x12, 0x34, 0x56, 0x78,
		                     0xBA5EBA11, 0xBA, 0x5E, 0xBA, 0x11,
		                     0xCAFEBABE, 0xCA, 0xFE, 0xBA, 0xBE,
		                     0xAAAACCCC, 0xAA, 0xAA, 0xCC, 0xCC),
		                        
		new TestCase("abs", 0x00002110, 0x00002110, 17,
		                    0x00000DCA, 0x00001236, 13,
		                    0x3FFFFFFF, 0x3FFFFFFF, 31,
		                    0x00000001, 0x7FFFFFFF, 31),
		                       
		new TestCase("xor", 0xFFFFFFFF, 0xFF00FF00, 0x00FF00FF,
		                    0x95511559, 0x12345678, 0x87654321,
		                    0xFFFFDDDD, 0x55555555, 0xAAAA8888,
		                    0x66666666, 0x55555555, 0x33333333),
		                       
		new TestCase("powerOf2", true,  0x10000000,
		                         false, 0x000000A0,
		                         true,  0x00000008,
		                         false, 0x80000000, // This number is negative!
		                         true,  0x00000001,
		                         false, 0x12345678,
		                         false, 0x00000000),
		                            
		new TestCase("set", 0x00000000, 0x00000001,  0,
		                    0x00000001, 0x00000003,  1,
		                    0x00000003, 0x00000007,  2,
		                    0x00000007, 0x0000000F,  3,
		                    0x0000000F, 0x0000001F,  4,
		                    0x0000001F, 0x0000003F,  5,
		                    0x0000003F, 0x0000007F,  6,
		                    0x0000007F, 0x000000FF,  7,
		                    0x000000FF, 0x000001FF,  8,
		                    0x000001FF, 0x000003FF,  9,
		                    0x000003FF, 0x000007FF, 10,
		                    0x000007FF, 0x00000FFF, 11,
		                    0x00000FFF, 0x00001FFF, 12,
		                    0x00001FFF, 0x00003FFF, 13,
		                    0x00003FFF, 0x00007FFF, 14,
		                    0x00007FFF, 0x0000FFFF, 15,
		                    0x0000FFFF, 0x0001FFFF, 16,
		                    0x0001FFFF, 0x0003FFFF, 17,
		                    0x0003FFFF, 0x0007FFFF, 18,
		                    0x0007FFFF, 0x000FFFFF, 19,
		                    0x000FFFFF, 0x001FFFFF, 20,
		                    0x001FFFFF, 0x003FFFFF, 21,
		                    0x003FFFFF, 0x007FFFFF, 22,
		                    0x007FFFFF, 0x00FFFFFF, 23,
		                    0x00FFFFFF, 0x01FFFFFF, 24,
		                    0x01FFFFFF, 0x03FFFFFF, 25,
		                    0x03FFFFFF, 0x07FFFFFF, 26,
		                    0x07FFFFFF, 0x0FFFFFFF, 27,
		                    0x0FFFFFFF, 0x1FFFFFFF, 28,
		                    0x1FFFFFFF, 0x3FFFFFFF, 29,
		                    0x3FFFFFFF, 0x7FFFFFFF, 30,
		                    0x7FFFFFFF, 0xFFFFFFFF, 31),
		                     
		new TestCase("clear", 0xFFFFFFFF, 0xFFF7FFFF, 19,
		                      0xFFF7FFFF, 0xFFD7FFFF, 21),
		                         
		new TestCase("toggle", 0xFFFFFFFF, 0xFFFFFFFE,  0,
		                       0xFFFFFFFE, 0xFFFFFFFF,  0,
		                       0xABCDEFFF, 0xABCDFFFF, 12,
		                       0xABCDEFFF, 0xEBCDEFFF, 30),
		                          
		new TestCase("isSet", 0x00100000, true,  20,
		                      0x55555555, true,   0,
		                      0x753A96CB, false, 31,
		                      0xFFFFFFFF, false, 32),
		                         
		new TestCase("isClear", 0x00100000, false, 20,
		                        0x55555555, false,  0,
		                        0x753A96CB, true,  31,
		                        0xFFFFFFFF, true,  32),
		                           
		new TestCase("toString", 0x00000000,
		                         "00000000000000000000000000000000",
		                         0xDEADBEEF,
		                         "11011110101011011011111011101111",
		                         0x0000F569,
		                         "00000000000000001111010101101001",
		                         0xFFFFFF0F,
		                         "11111111111111111111111100001111",
		                         0xFFAF7FFF,
		                         "11111111101011110111111111111111"),
		                            
		new TestCase("onesCount", 0xFFFFFFFF, 32,
		                          0x00000000,  0,
		                          0x5555FFFF, 24,
		                          0x87654321, 13),
		                             
		new TestCase("zerosCount", 0xFFFFFFFF,  0,
		                           0x00000000, 32,
		                           0x5555FFFF,  8,
		                           0x87654321, 19),
		                              
		new TestCase("size", 0xFFFFFFF0, 32,
		                     0x1F4CAB04, 29,
		                     0x00000000,  1,
		                     0x00007569, 15)
	};
	
	/** The instance of HW2BitVector which is used by this tester */
	private static HW2BitVector vec = new HW2BitVector();

	/** This Field allows unconditional access to bits in vec */
	private static Field field = null;
	static {
		try {
			field = vec.getClass().getDeclaredField("bits");
		} catch(NoSuchFieldException e) {
			e.printStackTrace();
		}
		field.setAccessible(true);
	}
	
	/** This PrintWriter will write detailed output to a log file */
	private static PrintWriter pw = null;
	static {
		try {
			pw = new PrintWriter(new File("Results.log"));
		} catch(FileNotFoundException e) {
			e.printStackTrace();
		}
	}
	
	/** This global flag is used to break out of infinite loops */
	// NOTE: The JDK optimizes this variable out if it's not declared "volatile"
	private static volatile boolean timeout;
	
	/**
	 * Run the test cases.
	 */
	public static void main(String[] args) {
		
		// Run each test in testCases
		for(int i = 0; i < testCases.length; ++i) {
			
			/* Spawn new threads to allow the test cases to continue if an
			 * infinite loop is encountered in the code. The Thread is left to
			 * hang until the tester completes execution.
			 */
			Thread thread = new Thread(new TestThread(i));
			Timer timer = new Timer(1000, new ActionListener() {
				public void actionPerformed(ActionEvent e) {
					timeout = true;
				}
			});
			thread.start();
			timer.start();
			timeout = false;
			
			// Quit busy-blocking if the operation completes or times out
			while(!testCases[i].completed && !timeout);
			timer.stop();
			if(!testCases[i].completed) {
				testCases[i].score = -1;
			}
		}
		pw.close();
		
		// For each test case, print the result
		int grade = 0;
		for(TestCase t : testCases) {
			if(t.score == 0) {
				System.out.printf("%s did not work\n", t.name);
			} else if(t.score == -1) {
				System.out.printf("%s looped infinitely\n", t.name);
			} else if(t.score > 0 && t.score < 1) {
				System.out.printf("%s worked sometimes\n", t.name);
			} else {
				System.out.printf("%s passed every test!\n", t.name);
				++grade;
			}
		}
		if(grade == testCases.length) {
			System.out.println("Perfect!");
		}
		
		// Manually cancel all running threads (infinite loops)
		System.exit(0);
	}
	
	/**
	 * This class represents a test case which is run by the tester.
	 * @param name The name of the function being tested by this test case.
	 * @param tests The test data for this test case, specialized for each test.
	 * @param score The score earned on the test, from 0.0 - 1.0
	 *              A value of -1.0 indicates an infinite loop.
	 */
	private static class TestCase {
		
		/** The name of the function this test case is testing */
		public String name;
		
		/**
		 * An array of test data for this test case; the data is casted
		 * appropriately in the individual test functions, which are specialized
		 * for each test.
		 */
		public Object[] tests;
		
		/** The score from 0.0 - 1.0 that the student received on this test */
		public double score = 0;
		
		/** Test completion flag checked by main's busy-blocking */
		// NOTE: The JDK optimizes this variable out if it's not volatile
		public volatile boolean completed = false;
		
		/**
		 * Create a new TestCase object with a weight, name, and data.
		 * @param n The weight of this test in raw points.
		 * @param s The name of the function being tested.
		 * @param o The data used for testing this function.
		 */
		public TestCase(String s, Object ... o) {
			name = s;
			tests = o;
		}
	}
	
	/**
	 * This class is used to run test cases in a separate Thread so that main
	 * can busy block for a set period on each test and avoid infinite loops.
	 * If the code infinite loops, this will hang until System.exit(0) is
	 * reached in the end of main.
	 */
	private static class TestThread implements Runnable {

		/** An ID for which test case to run */
		private int which;
		
		/**
		 * This is the constructor for the TestThread object.
		 * @param id The ID for which test this thread represents.
		 */
		public TestThread(int id) {
			which = id;
		}
		
		/**
		 * Run a test function specified by the index this TestThread was
		 * created with.
		 */
		@Override
		public void run() {
			TestCase t = testCases[which];
			switch(which) {
				case 0:
					pw.print("================\n" +
					         "Testing HW2Bases\n" +
					         "================\n");
					test_strdtoi(t);
					break;
				case 1:
					test_strbtoi(t);
					break;
				case 2:
					test_strxtoi(t);
					break;
				case 3:
					test_itostrb(t);
					break;
				case 4:
					test_itostrx(t);
					break;
				case 5:
					pw.print("\n=====================\n" +
					           "Testing HW2Operations\n" +
					           "=====================\n");
					test_setByte(t);
					break;
				case 6:
					test_getNibble(t);
					break;
				case 7:
					test_pack(t);
					break;
				case 8:
					test_abs(t);
					break;
				case 9:
					test_xor(t);
					break;
				case 10:
					test_powerOf2(t);
					break;
				case 11:
					pw.print("\n====================\n" +
					           "Testing HW2BitVector\n" +
					           "====================\n");
					test_set(t);
					break;
				case 12:
					test_clear(t);
					break;
				case 13:
					test_toggle(t);
					break;
				case 14:
					test_isSet(t);
					break;
				case 15:
					test_isClear(t);
					break;
				case 16:
					test_toString(t);
					break;
				case 17:
					test_onesCount(t);
					break;
				case 18:
					test_zerosCount(t);
					break;
				case 19:
					test_size(t);
					break;
			}
			
			// Once the test is complete, signal main to quit busy-blocking
			t.completed = true;
		}
	}
	
	/**
	 * Test the strdtoi function.
	 * @param pw A PrintWriter which will write a detailed log to an output file.
	 * @param t A TestCase whose data will be used in this test.
	 */
	private static void test_strdtoi(TestCase t) {
		
		// Print the name of the function to the log
		pw.printf("\nTesting %s:\n", t.name);
		
		// Run each test for this test case
		int score = 0;
		for(int i = 0; i < t.tests.length; i += 2) {
			int exp = (Integer) t.tests[i];
			String arg1 = (String) t.tests[i + 1];
			int res = HW2Bases.strdtoi(arg1);
			boolean cor = exp == res;
			if(cor) {
				++score;
			}
			pw.printf("%s(%10s): Expected: %8X Result: %8X Correct: %5b\n", t.name, arg1, exp, res, cor);
			pw.flush();
		}
		
		// Set the score for this test case
		t.score = (double) score / (t.tests.length / 2);
	}

	/**
	 * Test the strbtoi function.
	 * @param pw A PrintWriter which will write a detailed log to an output file
	 * @param t A TestCase whose data will be used in this test.
	 */
	private static void test_strbtoi(TestCase t) {
		
		// Print the name of the function to the log
		pw.printf("\nTesting %s:\n", t.name);
		
		// Run each test for this test case
		int score = 0;
		for(int i = 0; i < t.tests.length; i += 2) {
			int exp = (Integer) t.tests[i];
			String arg1 = (String) t.tests[i + 1];
			int res = HW2Bases.strbtoi(arg1);
			boolean cor = exp == res;
			if(cor) {
				++score;
			}
			pw.printf("%s(%31s): Expected: %8X Result: %8X Correct: %5b\n", t.name, arg1, exp, res, cor);
			pw.flush();
		}
		
		// Set the score for this test case
		t.score = (double) score / (t.tests.length / 2);
	}

	/**
	 * Test the strxtoi function.
	 * @param pw A PrintWriter which will write a detailed log to an output file
	 * @param t A TestCase whose data will be used in this test.
	 */
	private static void test_strxtoi(TestCase t) {
		
		// Print the name of the function to the log
		pw.printf("\nTesting %s:\n", t.name);
		
		// Run each test for this test case
		int score = 0;
		for(int i = 0; i < t.tests.length; i += 2) {
			int exp = (Integer) t.tests[i];
			String arg1 = (String) t.tests[i + 1];
			int res = HW2Bases.strxtoi(arg1);
			boolean cor = exp == res;
			if(cor) {
				++score;
			}
			pw.printf("%s(%8s): Expected: %8X Result: %8X Correct: %5b\n", t.name, arg1, exp, res, cor);
			pw.flush();
		}
		
		// Set the score for this test case
		t.score = (double) score / (t.tests.length / 2);
	}

	/**
	 * Test the itostrb function.
	 * @param pw A PrintWriter which will write a detailed log to an output file
	 * @param t A TestCase whose data will be used in this test.
	 */
	private static void test_itostrb(TestCase t) {
		
		// Print the name of the function to the log
		pw.printf("\nTesting %s:\n", t.name);
		
		// Run each test for this test case
		int score = 0;
		for(int i = 0; i < t.tests.length; i += 2) {
			String exp = (String) t.tests[i];
			int arg1 = (Integer) t.tests[i + 1];
			String res = HW2Bases.itostrb(arg1);
			boolean cor = exp.equals(res);
			if(cor) {
				++score;
			}
			pw.printf("%s(%8X): Expected: %31s Result: %31s Correct: %5b\n", t.name, arg1, exp, res, cor);
			pw.flush();
		}
		
		// Set the score for this test case
		t.score = (double) score / (t.tests.length / 2);
	}

	/**
	 * Test the itostrx function.
	 * @param pw A PrintWriter which will write a detailed log to an output file
	 * @param t A TestCase whose data will be used in this test.
	 */
	private static void test_itostrx(TestCase t) {
		
		// Print the name of the function to the log
		pw.printf("\nTesting %s:\n", t.name);
		
		// Run each test for this test case
		int score = 0;
		for(int i = 0; i < t.tests.length; i += 2) {
			String exp = (String) t.tests[i];
			int arg1 = (Integer) t.tests[i + 1];
			String res = HW2Bases.itostrx(arg1);
			boolean cor = exp.equals(res);
			if(cor) {
				++score;
			}
			pw.printf("%s(%8X): Expected: %8s Result: %8s Correct: %5b\n", t.name, arg1, exp, res, cor);
			pw.flush();
		}
		
		// Set the score for this test case
		t.score = (double) score / (t.tests.length / 2);
	}

	/**
	 * Test the setByte function.
	 * @param pw A PrintWriter which will write a detailed log to an output file
	 * @param t A TestCase whose data will be used in this test.
	 */
	private static void test_setByte(TestCase t) {
		
		// Print the name of the function to the log
		pw.printf("\nTesting %s:\n", t.name);
		
		// Run each test for this test case
		int score = 0;
		for(int i = 0; i < t.tests.length; i += 4) {
			int exp = (Integer) t.tests[i];
			int arg1 = (Integer) t.tests[i + 1];
			int arg2 = (Integer) t.tests[i + 2];
			int arg3 = (Integer) t.tests[i + 3];
			int res = HW2Operations.setByte(arg1, arg2, arg3);
			boolean cor = exp == res;
			if(cor) {
				++score;
			}
			pw.printf("%s(%4X, %1X, %d): Expected: %4X Result: %4X Correct: %5b\n", t.name, arg1, arg2, arg3, exp, res, cor);
			pw.flush();
		}
		
		// Set the score for this test case
		t.score = (double) score / (t.tests.length / 4);
	}

	/**
	 * Test the getNibble function.
	 * @param pw A PrintWriter which will write a detailed log to an output file
	 * @param t A TestCase whose data will be used in this test.
	 */
	private static void test_getNibble(TestCase t) {
		
		// Print the name of the function to the log
		pw.printf("\nTesting %s:\n", t.name);
		
		// Run each test for this test case
		int score = 0;
		for(int i = 0; i < t.tests.length; i += 3) {
			int exp = (Integer) t.tests[i];
			int arg1 = (Integer) t.tests[i + 1];
			int arg2 = (Integer) t.tests[i + 2];
			int res = HW2Operations.getNibble(arg1, arg2);
			boolean cor = exp == res;
			if(cor) {
				++score;
			}
			pw.printf("%s(%8X, %d): Expected: %4X Result: %4X Correct: %5b\n", t.name, arg1, arg2, exp, res, cor);
			pw.flush();
		}
		
		// Set the score for this test case
		t.score = (double) score / (t.tests.length / 3);
	}

	/**
	 * Test the pack function.
	 * @param pw A PrintWriter which will write a detailed log to an output file
	 * @param t A TestCase whose data will be used in this test.
	 */
	private static void test_pack(TestCase t) {
		
		// Print the name of the function to the log
		pw.printf("\nTesting %s:\n", t.name);
		
		// Run each test for this test case
		int score = 0;
		for(int i = 0; i < t.tests.length; i += 5) {
			int exp = (Integer) t.tests[i];
			int arg1 = (Integer) t.tests[i + 1];
			int arg2 = (Integer) t.tests[i + 2];
			int arg3 = (Integer) t.tests[i + 3];
			int arg4 = (Integer) t.tests[i + 4];
			int res = HW2Operations.pack(arg1, arg2, arg3, arg4);
			boolean cor = exp == res;
			if(cor) {
				++score;
			}
			pw.printf("%s(%1X, %1X, %1X, %1X): Expected: %4X Result: %4X Correct: %5b\n", t.name, arg1, arg2, arg3, arg4, exp, res, cor);
			pw.flush();
		}
		
		// Set the score for this test case
		t.score = (double) score / (t.tests.length / 5);
	}

	/**
	 * Test the abs function.
	 * @param pw A PrintWriter which will write a detailed log to an output file
	 * @param t A TestCase whose data will be used in this test.
	 */
	private static void test_abs(TestCase t) {
		
		// Print the name of the function to the log
		pw.printf("\nTesting %s:\n", t.name);
		
		// Run each test for this test case
		int score = 0;
		for(int i = 0; i < t.tests.length; i += 3) {
			int exp = (Integer) t.tests[i];
			int arg1 = (Integer) t.tests[i + 1];
			int arg2 = (Integer) t.tests[i + 2];
			int res = HW2Operations.abs(arg1, arg2);
			boolean cor = exp == res;
			if(cor) {
				++score;
			}
			pw.printf("%s(%8X, %2d): Expected: %8X Result: %8X Correct: %5b\n", t.name, arg1, arg2, exp, res, cor);
			pw.flush();
		}
		
		// Set the score for this test case
		t.score = (double) score / (t.tests.length / 3);
	}

	/**
	 * Test the xor function.
	 * @param pw A PrintWriter which will write a detailed log to an output file
	 * @param t A TestCase whose data will be used in this test.
	 */
	private static void test_xor(TestCase t) {
		
		// Print the name of the function to the log
		pw.printf("\nTesting %s:\n", t.name);
		
		// Run each test for this test case
		int score = 0;
		for(int i = 0; i < t.tests.length; i += 3) {
			int exp = (Integer) t.tests[i];
			int arg1 = (Integer) t.tests[i + 1];
			int arg2 = (Integer) t.tests[i + 2];
			int res = HW2Operations.xor(arg1, arg2);
			boolean cor = exp == res;
			if(cor) {
				++score;
			}
			pw.printf("%s(%8X, %8X): Expected: %8X Result: %8X Correct: %5b\n", t.name, arg1, arg2, exp, res, cor);
			pw.flush();
		}
		
		// Set the score for this test case
		t.score = (double) score / (t.tests.length / 3);
	}

	/**
	 * Test the powerOf2 function.
	 * @param pw A PrintWriter which will write a detailed log to an output file
	 * @param t A TestCase whose data will be used in this test.
	 */
	private static void test_powerOf2(TestCase t) {
		
		// Print the name of the function to the log
		pw.printf("\nTesting %s:\n", t.name);
		
		// Run each test for this test case
		int score = 0;
		for(int i = 0; i < t.tests.length; i += 2) {
			boolean exp = (Boolean) t.tests[i];
			int arg1 = (Integer) t.tests[i + 1];
			boolean res = HW2Operations.powerOf2(arg1);
			boolean cor = exp == res;
			if(cor) {
				++score;
			}
			pw.printf("%s(%8X): Expected: %5b Result: %5b Correct: %5b\n", t.name, arg1, exp, res, cor);
			pw.flush();
		}
		
		// Set the score for this test case
		t.score = (double) score / (t.tests.length / 2);
	}

	/**
	 * Test the set function.
	 * @param pw A PrintWriter which will write a detailed log to an output file
	 * @param t A TestCase whose data will be used in this test.
	 */
	private static void test_set(TestCase t) {
		
		// Print the name of the function to the log
		pw.printf("\nTesting %s:\n", t.name);
		
		// Run each test for this test case
		int score = 0;
		for(int i = 0; i < t.tests.length; i += 3) {
			int bits = (Integer) t.tests[i];
			int exp = (Integer) t.tests[i + 1];
			int arg1 = (Integer) t.tests[i + 2];
			try {
				field.setInt(vec, bits);
			} catch(IllegalAccessException e) {
				e.printStackTrace();
			}
			vec.set(arg1);
			int res = 0x55555555;
			try {
				res = field.getInt(vec);
			} catch(IllegalAccessException e) {
				e.printStackTrace();
			}
			boolean cor = exp == res;
			if(cor) {
				++score;
			}
			pw.printf("%s(%2d): Bits: %8X Expected: %8X Result: %8X Correct: %5b\n", t.name, arg1, bits, exp, res, cor);
			pw.flush();
		}
		
		// Set the score for this test case
		t.score = (double) score / (t.tests.length / 3);
	}

	/**
	 * Test the clear function.
	 * @param pw A PrintWriter which will write a detailed log to an output file
	 * @param t A TestCase whose data will be used in this test.
	 */
	private static void test_clear(TestCase t) {
		
		// Print the name of the function to the log
		pw.printf("\nTesting %s:\n", t.name);
		
		// Run each test for this test case
		int score = 0;
		for(int i = 0; i < t.tests.length; i += 3) {
			int bits = (Integer) t.tests[i];
			int exp = (Integer) t.tests[i + 1];
			int arg1 = (Integer) t.tests[i + 2];
			try {
				field.setInt(vec, bits);
			} catch(IllegalAccessException e) {
				e.printStackTrace();
			}
			vec.clear(arg1);
			int res = 0x55555555;
			try {
				res = field.getInt(vec);
			} catch(IllegalAccessException e) {
				e.printStackTrace();
			}
			boolean cor = exp == res;
			if(cor) {
				++score;
			}
			pw.printf("%s(%2d): Bits: %8X Expected: %8X Result: %8X Correct: %5b\n", t.name, arg1, bits, exp, res, cor);
			pw.flush();
		}
		
		// Set the score for this test case
		t.score = (double) score / (t.tests.length / 3);
	}

	/**
	 * Test the toggle function.
	 * @param pw A PrintWriter which will write a detailed log to an output file
	 * @param t A TestCase whose data will be used in this test.
	 */
	private static void test_toggle(TestCase t) {
		
		// Print the name of the function to the log
		pw.printf("\nTesting %s:\n", t.name);
		
		// Run each test for this test case
		int score = 0;
		for(int i = 0; i < t.tests.length; i += 3) {
			int bits = (Integer) t.tests[i];
			int exp = (Integer) t.tests[i + 1];
			int arg1 = (Integer) t.tests[i + 2];
			try {
				field.setInt(vec, bits);
			} catch(IllegalAccessException e) {
				e.printStackTrace();
			}
			vec.toggle(arg1);
			int res = 0x55555555;
			try {
				res = field.getInt(vec);
			} catch(IllegalAccessException e) {
				e.printStackTrace();
			}
			boolean cor = exp == res;
			if(cor) {
				++score;
			}
			pw.printf("%s(%2d): Bits: %8X Expected: %8X Result: %8X Correct: %5b\n", t.name, arg1, bits, exp, res, cor);
			pw.flush();
		}
		
		// Set the score for this test case
		t.score = (double) score / (t.tests.length / 3);
	}

	/**
	 * Test the isSet function.
	 * @param pw A PrintWriter which will write a detailed log to an output file
	 * @param t A TestCase whose data will be used in this test.
	 */
	private static void test_isSet(TestCase t) {
		
		// Print the name of the function to the log
		pw.printf("\nTesting %s:\n", t.name);
		
		// Run each test for this test case
		int score = 0;
		for(int i = 0; i < t.tests.length; i += 3) {
			int bits = (Integer) t.tests[i];
			boolean exp = (Boolean) t.tests[i + 1];
			int arg1 = (Integer) t.tests[i + 2];
			try {
				field.setInt(vec, bits);
			} catch(IllegalAccessException e) {
				e.printStackTrace();
			}
			boolean res = vec.isSet(arg1);
			boolean cor = exp == res;
			if(cor) {
				++score;
			}
			pw.printf("%s(%2d): Bits: %8X Expected: %5b Result: %5b Correct: %5b\n", t.name, arg1, bits, exp, res, cor);
			pw.flush();
		}
		
		// Set the score for this test case
		t.score = (double) score / (t.tests.length / 3);
	}

	/**
	 * Test the isClear function.
	 * @param pw A PrintWriter which will write a detailed log to an output file
	 * @param t A TestCase whose data will be used in this test.
	 */
	private static void test_isClear(TestCase t) {
		
		// Print the name of the function to the log
		pw.printf("\nTesting %s:\n", t.name);
		
		// Run each test for this test case
		int score = 0;
		for(int i = 0; i < t.tests.length; i += 3) {
			int bits = (Integer) t.tests[i];
			boolean exp = (Boolean) t.tests[i + 1];
			int arg1 = (Integer) t.tests[i + 2];
			try {
				field.setInt(vec, bits);
			} catch(IllegalAccessException e) {
				e.printStackTrace();
			}
			boolean res = vec.isClear(arg1);
			boolean cor = exp == res;
			if(cor) {
				++score;
			}
			pw.printf("%s(%2d): Bits: %8X Expected: %5b Result: %5b Correct: %5b\n", t.name, arg1, bits, exp, res, cor);
			pw.flush();
		}
		
		// Set the score for this test case
		t.score = (double) score / (t.tests.length / 3);
	}

	/**
	 * Test the toString function.
	 * @param pw A PrintWriter which will write a detailed log to an output file
	 * @param t A TestCase whose data will be used in this test.
	 */
	private static void test_toString(TestCase t) {
		
		// Print the name of the function to the log
		pw.printf("\nTesting %s:\n", t.name);
		
		// Run each test for this test case
		int score = 0;
		for(int i = 0; i < t.tests.length; i += 2) {
			int bits = (Integer) t.tests[i];
			String exp = (String) t.tests[i + 1];
			try {
				field.setInt(vec, bits);
			} catch(IllegalAccessException e) {
				e.printStackTrace();
			}
			String res = vec.toString();
			boolean cor = exp.equals(res);
			if(cor) {
				++score;
			}
			pw.printf("%s(): Bits: %8X Expected: %32s Result: %32s Correct: %5b\n", t.name, bits, exp, res, cor);
			pw.flush();
		}
		
		// Set the score for this test case
		t.score = (double) score / (t.tests.length / 2);
	}

	/**
	 * Test the onesCount function.
	 * @param pw A PrintWriter which will write a detailed log to an output file
	 * @param t A TestCase whose data will be used in this test.
	 */
	private static void test_onesCount(TestCase t) {
		
		// Print the name of the function to the log
		pw.printf("\nTesting %s:\n", t.name);
		
		// Run each test for this test case
		int score = 0;
		for(int i = 0; i < t.tests.length; i += 2) {
			int bits = (Integer) t.tests[i];
			int exp = (Integer) t.tests[i + 1];
			try {
				field.setInt(vec, bits);
			} catch(IllegalAccessException e) {
				e.printStackTrace();
			}
			int res = vec.onesCount();
			boolean cor = exp == res;
			if(cor) {
				++score;
			}
			pw.printf("%s(): Bits: %8X Expected: %2d Result: %2d Correct: %5b\n", t.name, bits, exp, res, cor);
			pw.flush();
		}
		
		// Set the score for this test case
		t.score = (double) score / (t.tests.length / 2);
	}

	/**
	 * Test the zerosCount function.
	 * @param pw A PrintWriter which will write a detailed log to an output file
	 * @param t A TestCase whose data will be used in this test.
	 */
	private static void test_zerosCount(TestCase t) {
		
		// Print the name of the function to the log
		pw.printf("\nTesting %s:\n", t.name);
		
		// Run each test for this test case
		int score = 0;
		for(int i = 0; i < t.tests.length; i += 2) {
			int bits = (Integer) t.tests[i];
			int exp = (Integer) t.tests[i + 1];
			try {
				field.setInt(vec, bits);
			} catch(IllegalAccessException e) {
				e.printStackTrace();
			}
			int res = vec.zerosCount();
			boolean cor = exp == res;
			if(cor) {
				++score;
			}
			pw.printf("%s(): Bits: %8X Expected: %2d Result: %2d Correct: %5b\n", t.name, bits, exp, res, cor);
			pw.flush();
		}
		
		// Set the score for this test case
		t.score = (double) score / (t.tests.length / 2);
	}

	/**
	 * Test the size function.
	 * @param pw A PrintWriter which will write a detailed log to an output file
	 * @param t A TestCase whose data will be used in this test.
	 */
	private static void test_size(TestCase t) {
		
		// Print the name of the function to the log
		pw.printf("\nTesting %s:\n", t.name);
		
		// Run each test for this test case
		int score = 0;
		for(int i = 0; i < t.tests.length; i += 2) {
			int bits = (Integer) t.tests[i];
			int exp = (Integer) t.tests[i + 1];
			try {
				field.setInt(vec, bits);
			} catch(IllegalAccessException e) {
				e.printStackTrace();
			}
			int res = vec.size();
			boolean cor = exp == res;
			if(cor) {
				++score;
			}
			pw.printf("%s(): Bits: %8X Expected: %2d Result: %2d Correct: %5b\n", t.name, bits, exp, res, cor);
			pw.flush();
		}
		
		// Set the score for this test case
		t.score = (double) score / (t.tests.length / 2);
	}
}

