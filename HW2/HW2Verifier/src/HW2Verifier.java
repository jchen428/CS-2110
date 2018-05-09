import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.tree.*;

import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.regex.Pattern;

// Build   : java -cp .:antlr-4.4-complete.jar org.antlr.v4.Tool Java.g4
// Compile : javac -cp .:antlr-runtime-4.4.jar *.java
// Run     : java -cp .:antlr-runtime-4.4jar HW2Verifier
public class HW2Verifier {
	
	/** Files to parse */
	private static final String[] HW2_FILES = {
		"HW2Bases.java",
		"HW2BitVector.java",
		"HW2Operations.java"
	};
	
	/** Allowed method invocations */
	private static final String[] ALLOWED_METHODS = {
		"length",
		"charAt",
		"equals"
	};
	
	/** Enumeration of the banned operations */
	private static enum HW2OpType {
		Addition                      ("Addition"                                          ),
		Subtraction                   ("Subtraction"                                       ),
		Multiplication                ("Multiplication"                                    ),
		Division                      ("Division"                                          ),
		Modulus                       ("Modulus"                                           ),
		XOR                           ("Bitwise XOR"                                       ),
		LeftShift                     ("Left shift"                                        ),
		RightShift                    ("Right shift"                                       ),
		UnsignedRightShift            ("Unsigned right shift"                              ),
		Conditional                   ("Conditional"                                       ),
		ThreeConditionals             ("More than 2 conditionals"                          ),
		Iteration                     ("Iteration"                                         ),
		ThreeIterations               ("More than 2 loops"                                 ),
		NestedIteration               ("Nested loops"                                      ),
		ArrayDeclaration              ("Array declaration"                                 ),
		StringDeclaration             ("String declaration"                                ),
		TwoStringDeclaration          ("More than 1 String declared"                       ),
		Switch                        ("Switch statement"                                  ),
		Casting                       ("Casting"                                           ),
		IllegalFunctionCall           ("Disallowed method invocation"                      ),
		StringLiteralInvocation       ("Method invocation from a String literal"           ),
		MultiDigitStringConcatenation ("Concatenation of a string with more than 1 digit"  ),
		MultipleLines                 ("Implemented in more than 1 line"                   ),
		AdditionWithNumBesides1       ("Addition or subtraction with a number other than 1");
		private String msg;
		private HW2OpType(String msg) {
			this.msg = msg;
		}
		public String toString() {
			return msg;
		}
	};
	
	/** List of methods to verify */
	private static final String[] HW2_METHODS = {
		"setByte",
		"getNibble",
		"strbtoi",
		"strdtoi",
		"strxtoi",
		"itostrb",
		"itostrx",
		"set",
		"clear",
		"toggle",
		"isSet",
		"isClear",
		"toString",
		"onesCount",
		"zerosCount",
		"size",
		"pack",
		"abs",
		"xor",
		"powerOf2"
	};
	
	/** Banned operations per function */
	private static final HW2OpType[][] HW2_BANLIST = {
		{   // setByte
			HW2OpType.Addition,
			HW2OpType.Subtraction,
			HW2OpType.Multiplication,
			HW2OpType.Division,
			HW2OpType.Modulus,
			HW2OpType.Iteration,
			HW2OpType.UnsignedRightShift,
			HW2OpType.Switch,
			HW2OpType.ArrayDeclaration,
			HW2OpType.IllegalFunctionCall,
			HW2OpType.Conditional,
			HW2OpType.ThreeConditionals,
			HW2OpType.ThreeIterations,
			HW2OpType.TwoStringDeclaration,
			HW2OpType.StringLiteralInvocation,
			HW2OpType.MultipleLines,
			HW2OpType.MultiDigitStringConcatenation,
			HW2OpType.AdditionWithNumBesides1,
			HW2OpType.Casting
		},{ // getNibble
			HW2OpType.Addition,
			HW2OpType.Subtraction,
			HW2OpType.Multiplication,
			HW2OpType.Division,
			HW2OpType.Modulus,
			HW2OpType.Iteration,
			HW2OpType.UnsignedRightShift,
			HW2OpType.Switch,
			HW2OpType.ArrayDeclaration,
			HW2OpType.IllegalFunctionCall,
			HW2OpType.Conditional,
			HW2OpType.ThreeConditionals,
			HW2OpType.ThreeIterations,
			HW2OpType.TwoStringDeclaration,
			HW2OpType.StringLiteralInvocation,
			HW2OpType.MultipleLines,
			HW2OpType.MultiDigitStringConcatenation,
			HW2OpType.AdditionWithNumBesides1,
			HW2OpType.Casting
		},{ // strbtoi
			HW2OpType.MultiDigitStringConcatenation,
			HW2OpType.IllegalFunctionCall,
			HW2OpType.ThreeConditionals,
			HW2OpType.ThreeIterations,
			HW2OpType.NestedIteration,
			HW2OpType.Switch,
			HW2OpType.ArrayDeclaration,
			HW2OpType.StringDeclaration,
			HW2OpType.StringLiteralInvocation,
			HW2OpType.UnsignedRightShift,
			HW2OpType.Multiplication,
			HW2OpType.Division,
			HW2OpType.Modulus,
			HW2OpType.TwoStringDeclaration
		},{ // strdtoi
			HW2OpType.MultiDigitStringConcatenation,
			HW2OpType.IllegalFunctionCall,
			HW2OpType.ThreeConditionals,
			HW2OpType.ThreeIterations,
			HW2OpType.NestedIteration,
			HW2OpType.Switch,
			HW2OpType.ArrayDeclaration,
			HW2OpType.StringDeclaration,
			HW2OpType.StringLiteralInvocation,
			HW2OpType.UnsignedRightShift,
			HW2OpType.TwoStringDeclaration
		},{ // strxtoi
			HW2OpType.MultiDigitStringConcatenation,
			HW2OpType.IllegalFunctionCall,
			HW2OpType.ThreeConditionals,
			HW2OpType.ThreeIterations,
			HW2OpType.NestedIteration,
			HW2OpType.Switch,
			HW2OpType.ArrayDeclaration,
			HW2OpType.StringDeclaration,
			HW2OpType.StringLiteralInvocation,
			HW2OpType.UnsignedRightShift,
			HW2OpType.Multiplication,
			HW2OpType.Division,
			HW2OpType.Modulus,
			HW2OpType.TwoStringDeclaration
		},{ // itostrb
			HW2OpType.MultiDigitStringConcatenation,
			HW2OpType.IllegalFunctionCall,
			HW2OpType.ThreeConditionals,
			HW2OpType.ThreeIterations,
			HW2OpType.NestedIteration,
			HW2OpType.Switch,
			HW2OpType.ArrayDeclaration,
			HW2OpType.StringLiteralInvocation,
			HW2OpType.UnsignedRightShift,
			HW2OpType.Multiplication,
			HW2OpType.Division,
			HW2OpType.Modulus,
			HW2OpType.TwoStringDeclaration
		},{ // itostrx
			HW2OpType.MultiDigitStringConcatenation,
			HW2OpType.IllegalFunctionCall,
			HW2OpType.ThreeConditionals,
			HW2OpType.ThreeIterations,
			HW2OpType.NestedIteration,
			HW2OpType.Switch,
			HW2OpType.ArrayDeclaration,
			HW2OpType.StringLiteralInvocation,
			HW2OpType.UnsignedRightShift,
			HW2OpType.Multiplication,
			HW2OpType.Division,
			HW2OpType.Modulus,
			HW2OpType.TwoStringDeclaration
		},{ // set
			HW2OpType.MultiDigitStringConcatenation,
			HW2OpType.IllegalFunctionCall,
			HW2OpType.ThreeConditionals,
			HW2OpType.ThreeIterations,
			HW2OpType.NestedIteration,
			HW2OpType.Switch,
			HW2OpType.ArrayDeclaration,
			HW2OpType.StringDeclaration,
			HW2OpType.StringLiteralInvocation,
			HW2OpType.UnsignedRightShift,
			HW2OpType.Multiplication,
			HW2OpType.Division,
			HW2OpType.Modulus,
			HW2OpType.TwoStringDeclaration,
			HW2OpType.Conditional,
			HW2OpType.Iteration,
			HW2OpType.AdditionWithNumBesides1,
			HW2OpType.Casting
		},{ // clear
			HW2OpType.MultiDigitStringConcatenation,
			HW2OpType.IllegalFunctionCall,
			HW2OpType.ThreeConditionals,
			HW2OpType.ThreeIterations,
			HW2OpType.NestedIteration,
			HW2OpType.Switch,
			HW2OpType.ArrayDeclaration,
			HW2OpType.StringDeclaration,
			HW2OpType.StringLiteralInvocation,
			HW2OpType.UnsignedRightShift,
			HW2OpType.Multiplication,
			HW2OpType.Division,
			HW2OpType.Modulus,
			HW2OpType.TwoStringDeclaration,
			HW2OpType.Conditional,
			HW2OpType.Iteration,
			HW2OpType.AdditionWithNumBesides1,
			HW2OpType.Casting
		},{ // toggle
			HW2OpType.MultiDigitStringConcatenation,
			HW2OpType.IllegalFunctionCall,
			HW2OpType.ThreeConditionals,
			HW2OpType.ThreeIterations,
			HW2OpType.NestedIteration,
			HW2OpType.Switch,
			HW2OpType.ArrayDeclaration,
			HW2OpType.StringDeclaration,
			HW2OpType.StringLiteralInvocation,
			HW2OpType.UnsignedRightShift,
			HW2OpType.Multiplication,
			HW2OpType.Division,
			HW2OpType.Modulus,
			HW2OpType.TwoStringDeclaration,
			HW2OpType.Conditional,
			HW2OpType.Iteration,
			HW2OpType.AdditionWithNumBesides1,
			HW2OpType.Casting
		},{ // isSet
			HW2OpType.MultiDigitStringConcatenation,
			HW2OpType.IllegalFunctionCall,
			HW2OpType.ThreeConditionals,
			HW2OpType.ThreeIterations,
			HW2OpType.NestedIteration,
			HW2OpType.Switch,
			HW2OpType.ArrayDeclaration,
			HW2OpType.StringDeclaration,
			HW2OpType.StringLiteralInvocation,
			HW2OpType.UnsignedRightShift,
			HW2OpType.Multiplication,
			HW2OpType.Division,
			HW2OpType.Modulus,
			HW2OpType.TwoStringDeclaration,
			HW2OpType.Iteration,
			HW2OpType.AdditionWithNumBesides1,
			HW2OpType.Casting
		},{ // isClear
			HW2OpType.MultiDigitStringConcatenation,
			HW2OpType.IllegalFunctionCall,
			HW2OpType.ThreeConditionals,
			HW2OpType.ThreeIterations,
			HW2OpType.NestedIteration,
			HW2OpType.Switch,
			HW2OpType.ArrayDeclaration,
			HW2OpType.StringDeclaration,
			HW2OpType.StringLiteralInvocation,
			HW2OpType.UnsignedRightShift,
			HW2OpType.Multiplication,
			HW2OpType.Division,
			HW2OpType.Modulus,
			HW2OpType.TwoStringDeclaration,
			HW2OpType.Iteration,
			HW2OpType.AdditionWithNumBesides1,
			HW2OpType.Casting
		},{ // toString
			HW2OpType.MultiDigitStringConcatenation,
			HW2OpType.IllegalFunctionCall,
			HW2OpType.ThreeConditionals,
			HW2OpType.ThreeIterations,
			HW2OpType.NestedIteration,
			HW2OpType.Switch,
			HW2OpType.ArrayDeclaration,
			HW2OpType.StringLiteralInvocation,
			HW2OpType.UnsignedRightShift,
			HW2OpType.Multiplication,
			HW2OpType.Division,
			HW2OpType.Modulus,
			HW2OpType.TwoStringDeclaration,
			HW2OpType.AdditionWithNumBesides1,
			HW2OpType.Casting
		},{ // onesCount
			HW2OpType.MultiDigitStringConcatenation,
			HW2OpType.IllegalFunctionCall,
			HW2OpType.ThreeConditionals,
			HW2OpType.ThreeIterations,
			HW2OpType.NestedIteration,
			HW2OpType.Switch,
			HW2OpType.ArrayDeclaration,
			HW2OpType.StringDeclaration,
			HW2OpType.StringLiteralInvocation,
			HW2OpType.UnsignedRightShift,
			HW2OpType.Multiplication,
			HW2OpType.Division,
			HW2OpType.Modulus,
			HW2OpType.TwoStringDeclaration,
			HW2OpType.AdditionWithNumBesides1,
			HW2OpType.Casting
		},{ // zerosCount
			HW2OpType.MultiDigitStringConcatenation,
			HW2OpType.IllegalFunctionCall,
			HW2OpType.ThreeConditionals,
			HW2OpType.ThreeIterations,
			HW2OpType.NestedIteration,
			HW2OpType.Switch,
			HW2OpType.ArrayDeclaration,
			HW2OpType.StringDeclaration,
			HW2OpType.StringLiteralInvocation,
			HW2OpType.UnsignedRightShift,
			HW2OpType.Multiplication,
			HW2OpType.Division,
			HW2OpType.Modulus,
			HW2OpType.TwoStringDeclaration,
			HW2OpType.AdditionWithNumBesides1,
			HW2OpType.Casting
		},{ // size
			HW2OpType.MultiDigitStringConcatenation,
			HW2OpType.IllegalFunctionCall,
			HW2OpType.ThreeConditionals,
			HW2OpType.ThreeIterations,
			HW2OpType.NestedIteration,
			HW2OpType.Switch,
			HW2OpType.ArrayDeclaration,
			HW2OpType.StringDeclaration,
			HW2OpType.StringLiteralInvocation,
			HW2OpType.UnsignedRightShift,
			HW2OpType.Multiplication,
			HW2OpType.Division,
			HW2OpType.Modulus,
			HW2OpType.TwoStringDeclaration,
			HW2OpType.AdditionWithNumBesides1,
			HW2OpType.Casting
		},{ // pack
			HW2OpType.MultiDigitStringConcatenation,
			HW2OpType.Addition,
			HW2OpType.Subtraction,
			HW2OpType.Multiplication,
			HW2OpType.Division,
			HW2OpType.Modulus,
			HW2OpType.Iteration,
			HW2OpType.ThreeIterations,
			HW2OpType.NestedIteration,
			HW2OpType.UnsignedRightShift,
			HW2OpType.Switch,
			HW2OpType.ArrayDeclaration,
			HW2OpType.StringDeclaration,
			HW2OpType.TwoStringDeclaration,
			HW2OpType.StringLiteralInvocation,
			HW2OpType.MultipleLines,
			HW2OpType.AdditionWithNumBesides1,
			HW2OpType.IllegalFunctionCall,
			HW2OpType.Conditional,
			HW2OpType.ThreeConditionals,
			HW2OpType.Casting
		},{ // abs
			HW2OpType.MultiDigitStringConcatenation,
			HW2OpType.Multiplication,
			HW2OpType.Division,
			HW2OpType.Modulus,
			HW2OpType.ThreeIterations,
			HW2OpType.NestedIteration,
			HW2OpType.UnsignedRightShift,
			HW2OpType.Switch,
			HW2OpType.ArrayDeclaration,
			HW2OpType.StringDeclaration,
			HW2OpType.TwoStringDeclaration,
			HW2OpType.StringLiteralInvocation,
			HW2OpType.AdditionWithNumBesides1,
			HW2OpType.IllegalFunctionCall,
			HW2OpType.Iteration,
			HW2OpType.ThreeConditionals,
			HW2OpType.Casting
		},{ // xor
			HW2OpType.MultiDigitStringConcatenation,
			HW2OpType.Addition,
			HW2OpType.Subtraction,
			HW2OpType.Multiplication,
			HW2OpType.Division,
			HW2OpType.Modulus,
			HW2OpType.Iteration,
			HW2OpType.ThreeIterations,
			HW2OpType.NestedIteration,
			HW2OpType.UnsignedRightShift,
			HW2OpType.Switch,
			HW2OpType.ArrayDeclaration,
			HW2OpType.StringDeclaration,
			HW2OpType.TwoStringDeclaration,
			HW2OpType.StringLiteralInvocation,
			HW2OpType.MultipleLines,
			HW2OpType.AdditionWithNumBesides1,
			HW2OpType.IllegalFunctionCall,
			HW2OpType.Conditional,
			HW2OpType.ThreeConditionals,
			HW2OpType.XOR,
			HW2OpType.MultipleLines,
			HW2OpType.LeftShift,
			HW2OpType.RightShift,
			HW2OpType.Casting
		},{ // powerOf2
			HW2OpType.MultiDigitStringConcatenation,
			HW2OpType.Multiplication,
			HW2OpType.Division,
			HW2OpType.Modulus,
			HW2OpType.ThreeIterations,
			HW2OpType.NestedIteration,
			HW2OpType.UnsignedRightShift,
			HW2OpType.Switch,
			HW2OpType.ArrayDeclaration,
			HW2OpType.StringDeclaration,
			HW2OpType.TwoStringDeclaration,
			HW2OpType.StringLiteralInvocation,
			HW2OpType.AdditionWithNumBesides1,
			HW2OpType.IllegalFunctionCall,
			HW2OpType.ThreeConditionals,
			HW2OpType.MultipleLines,
			HW2OpType.Iteration,
			HW2OpType.Casting
		}
	};
	
	/**
	 * Parse each file, determine banned operators, tell the user what was banned
	 * @param args Unused
	 */
	private static boolean violation = false;
	public static void main(String[] args) {
		
		// Create mapping of functions onto banned ops and found violations
		HashMap<String, HW2Pair<HW2OpType[], HashSet<HW2OpType>>> banlist =
				new HashMap<String, HW2Pair<HW2OpType[], HashSet<HW2OpType>>>();
		for(int i = 0; i < HW2_METHODS.length; ++i) {
			banlist.put(HW2_METHODS[i], new HW2Pair<HW2OpType[], HashSet<HW2OpType>>(
					HW2_BANLIST[i], new HashSet<HW2OpType>()));
		}
		
		// Analyze each file
		for(String currFile : HW2_FILES) {
			
			// Tokenize and generate parse tree
			ANTLRFileStream input;
			try {
				input = new ANTLRFileStream(currFile);
				JavaLexer lexer = new JavaLexer(input);
				CommonTokenStream tokens = new CommonTokenStream(lexer);
				JavaParser parser = new JavaParser(tokens);
				ParserRuleContext tree = parser.compilationUnit();
				
				// Walk the tree
				ParseTreeWalker walker = new ParseTreeWalker();
				JavaListener listener = new HW2Listener(currFile, banlist);
				walker.walk(listener, tree);
				
			} catch (IOException e) {
				System.err.printf("Could not find file: %s\n", currFile);
				System.exit(1);
			}
		}
		
		// Print out the operations found
		ArrayList<String> funcs = new ArrayList<String>();
		for(String s : HW2_METHODS) {
			funcs.add(s);
		}

		// Illegal operations
		boolean f = true;
		for(String s : banlist.keySet()) {
			if(funcs.contains(s)) {
				ArrayList<HW2OpType> ops = new ArrayList<HW2OpType>();
				for(HW2OpType op : banlist.get(s).t1) {
					ops.add(op);
				}
				boolean b = true;
				for(HW2OpType op : banlist.get(s).t2) {
					if(ops.contains(op)) {
						if(f) {
							f = false;
							System.out.println("Illegal operations:");
						}
						if(b) {
							System.out.println("\t" + s + ": ");
							b = false;
						}
						System.out.println("\t\t" + op);
						violation = true;
					}
				}
			}
		}
		
		// Congrats if there were no violations!
		if(!violation) {
			System.out.println("No violations.");
		}
	}

	/**
	 * Interrupts parse tree walk of HW2 files to detect banned operators
	 */
	private static class HW2Listener extends JavaBaseListener {
		
		/** Variables to describe where we are in the parsing */
		private String currFile,
		               currMethod = null;
		private HashMap<String, HW2Pair<HW2OpType[], HashSet<HW2OpType>>> banlist;
		private int iterationLevel         = 0,
				    iterationCount         = 0,
				    conditionalCount       = 0,
				    stringDeclarationCount = 0;
		
		/**
		 * Initialize this HW2Listener with a filename that's being looked at
		 * @param currFile The name of the file being parsed
		 */
		public HW2Listener(String currFile, HashMap<String, HW2Pair<HW2OpType[],
				HashSet<HW2OpType>>> banlist) {
			this.currFile = currFile;
			this.banlist = banlist;
		}
		
		/**
		 * Add a violation to the current function (may not be an actual violation
		 * in the banlist, just an operation being checked for)
		 * @param operation The operation to add to list of potential violations
		 */
		private void addViolation(HW2OpType operation) {
			banlist.get(currMethod).t2.add(operation);
		}
		
		/**
		 * No imports allowed
		 * @param ctx Contains the list of ParseTrees with symbols to check
		 */
		public void enterImportDeclaration(JavaParser.ImportDeclarationContext ctx) {
			System.err.println("Java API import detected in " + currFile + ": " +
					ctx.children.get(ctx.children.size() - 2).getText());
			violation = true;
		}
		
		/**
		 * No file level vars allowed, except "bits" in HW2BitVector.java
		 * @param ctx Contains the list of ParseTrees with symbols to check
		 */
		public void enterFieldDeclaration(JavaParser.FieldDeclarationContext ctx) {
			if(!"bits".equals(ctx.children.get(1).getText()) || !"HW2BitVector.java".equals(currFile)) {
				System.err.println("Class-level variable declared in " + currFile + ": " +
						ctx.children.get(ctx.children.size() - 2).getText());
				violation = true;
			}
		}
		
		/**
		 * When a method declaration is entered, set the name of the function
		 * being traversed so that potential violations found can be added to
		 * the appropriate entry 
		 * @param ctx Contains the list of ParseTrees with symbols to check
		 */
		public void enterMethodDeclaration(JavaParser.MethodDeclarationContext ctx) {
			
			// Name of the function being entered
			String thisFunction = ctx.Identifier().toString();
			
			// No helper functions allowed
			if(!banlist.containsKey(thisFunction)) {
				banlist.put(thisFunction, new HW2Pair<HW2OpType[], HashSet<HW2OpType>>(
						null, new HashSet<HW2OpType>()));
				System.err.println("Helper function detected in " + currFile + ": " +
						thisFunction);
				violation = true;
			}
			
			// Set current function variables
			currMethod = thisFunction;
			iterationCount = iterationLevel = conditionalCount = stringDeclarationCount = 0;
		}
		
		/**
		 * Detection of operations at the statement level, which includes looping
		 * constructs and switch statements
		 * @param ctx Contains the list of ParseTrees with symbols to check
		 */
		public void enterStatement(JavaParser.StatementContext ctx) {

			// Obtain String representations of the tokens
			ArrayList<String> statementTokens = new ArrayList<String>();
			for(ParseTree t : ctx.children) {
				statementTokens.add(t.getText());
			}
			
			// Detect iteration
			if(statementTokens.contains("for") || statementTokens.contains("while")) {
				addViolation(HW2OpType.Iteration);
				// Nested iteration
				if(iterationLevel++ == 1) {
					addViolation(HW2OpType.NestedIteration);
				}
				// 3+ Loops
				if(++iterationCount == 3) {
					addViolation(HW2OpType.ThreeIterations);
				}
			}
			
			// Detect conditionals
			if(statementTokens.contains("if")) {
				addViolation(HW2OpType.Conditional);
				// 3+ Conditionals
				if(++conditionalCount == 3) {
					addViolation(HW2OpType.ThreeConditionals);
				}
			}
			
			// Detect switch statements
			if(statementTokens.contains("switch")) {
				addViolation(HW2OpType.Switch);
			}
		}
		
		/**
		 * The only thing to do here is decrease the iteration level when exiting
		 * a loop construct
		 * @param ctx Contains the list of ParseTrees with symbols to check
		 */
		public void exitStatement(JavaParser.StatementContext ctx) {
			
			// Obtain String representations of the tokens
			ArrayList<String> statementTokens = new ArrayList<String>();
			for(ParseTree t : ctx.children) {
				statementTokens.add(t.getText());
			}
			
			// Detect iteration
			if(statementTokens.contains("for") || statementTokens.contains("while")) {
				--iterationLevel;
			}
		}
		
		/**
		 * Detect basic operators
		 * @param ctx Contains the list of ParseTrees with symbols to check
		 */
		public void enterExpression(JavaParser.ExpressionContext ctx) {
			
			// Obtain String representations of the tokens
			ArrayList<String> expressionTokens = new ArrayList<String>();
			for(ParseTree t : ctx.children) {
				expressionTokens.add(t.getText());
			}
			
			/* Detect shift operators
			 * This is done with these loops because of the way the Java grammar
			 * is written for ANTLR 4; the individual characters are separated
			 * into different tokens in the grammar to allow for nested generics.
			 */
			int c = 0;
			for(String s : expressionTokens) {
				if("<".equals(s)) {
					if(++c == 2) {
						addViolation(HW2OpType.LeftShift);
					}
				} else {
					c = 0;
				}
			}
			c = 0;
			for(String s : expressionTokens) {
				if(">".equals(s)) {
					if(++c == 2) {
						addViolation(HW2OpType.RightShift);
					} else if(c == 3) {
						addViolation(HW2OpType.UnsignedRightShift);
					}
				} else {
					c = 0;
				}
			}
			if(expressionTokens.contains("<<=")) {
				addViolation(HW2OpType.LeftShift);
			}
			if(expressionTokens.contains(">>=")) {
				addViolation(HW2OpType.RightShift);
			}
			if(expressionTokens.contains(">>>=")) {
				addViolation(HW2OpType.UnsignedRightShift);
			}
			
			// Detect basic operations
			if(expressionTokens.contains("+") || expressionTokens.contains("+=")) {
				addViolation(HW2OpType.Addition);
				// Detect illegal String concatenation
				if(ctx.children.size() > 2) {
					String left  = ctx.getChild(0).getText(),
					       right = ctx.getChild(2).getText();
					if(Pattern.matches("\"..+\"", left) || Pattern.matches("\"..+\"", right) ||
							Pattern.matches("\\d\\d+", left) || Pattern.matches("\\d\\d+", right)) {
						//addViolation(HW2OpType.MultiDigitStringConcatenation);
					}
				}
			}
			if(expressionTokens.contains("-") || expressionTokens.contains("-=")) {
				addViolation(HW2OpType.Subtraction);
			}
			if(expressionTokens.contains("*") || expressionTokens.contains("*=")) {
				addViolation(HW2OpType.Multiplication);
			}
			if(expressionTokens.contains("/") || expressionTokens.contains("/=")) {
				addViolation(HW2OpType.Division);
			}
			if(expressionTokens.contains("%") || expressionTokens.contains("%=")) {
				addViolation(HW2OpType.Modulus);
			}
			if(expressionTokens.contains("^") || expressionTokens.contains("^=")) {
				addViolation(HW2OpType.XOR);
			}
			
			// Addition or subtraction with values other than 1
			if(expressionTokens.contains("+") || expressionTokens.contains("-")) {
				if(ctx.children.size() > 2) {
					String[] allowedNumbers = {"1", "0x1"};
					ArrayList<String> allowedList = new ArrayList<String>();
					for(String s : allowedNumbers) {
						allowedList.add(s);
					}
					String left  = ctx.getChild(0).getText(),
					       right = ctx.getChild(2).getText();
					if(Pattern.matches("(0x)?\\d+", left)  && !allowedList.contains(left) ||
					   Pattern.matches("(0x)?\\d+", right) && !allowedList.contains(right)) {
						addViolation(HW2OpType.AdditionWithNumBesides1);
					}
				}
			}
			
			// Increment conditional count on ternary operator
			if(expressionTokens.contains("?")) {
				addViolation(HW2OpType.Conditional);
				if(++conditionalCount == 3) {
					addViolation(HW2OpType.ThreeConditionals);
				}
			}
			
			// Detect function invocations
			if(ctx.children.size() > 2 && ".".equals(ctx.getChild(1).getText())) {
				ArrayList<String> allowed = new ArrayList<String>();
				for(String s : ALLOWED_METHODS) {
					allowed.add(s);
				}
				// String literal invocation
				if(ctx.getChild(0).getText().charAt(0) == '"') {
					addViolation(HW2OpType.StringLiteralInvocation);
				} else if(!allowed.contains(ctx.getChild(2).getText())) {
					addViolation(HW2OpType.IllegalFunctionCall);
				}
			} else if(ctx.children.size() > 2 && "(".equals(ctx.getChild(1).getText())) {
				ArrayList<String> hw2Methods = new ArrayList<String>();
				for(String s : HW2_METHODS) {
					hw2Methods.add(s);
				}
				if(hw2Methods.contains(ctx.getChild(0).getText())) {
					addViolation(HW2OpType.IllegalFunctionCall);
				}
			}
			
			// Detect casting
			if(ctx.children.size() > 3 && "(".equals(ctx.getChild(0).getText()) &&
			                              ")".equals(ctx.getChild(2).getText())) {
				addViolation(HW2OpType.Casting);
			}
		}
		
		/**
		 * Detect array or String declarations
		 * @param ctx Contains the list of ParseTrees with symbols to check
		 */
		public void exitLocalVariableDeclaration(JavaParser.LocalVariableDeclarationContext ctx) {
			
			// Obtain String representations of the tokens
			ArrayList<String> varDeclarationTokens = new ArrayList<String>();
			for(ParseTree t : ctx.children) {
				varDeclarationTokens.add(t.getText());
			}
			
			// Detect String declaration
			if(varDeclarationTokens.contains("String")) {
				addViolation(HW2OpType.StringDeclaration);
				// Multiple String declarations
				if(++stringDeclarationCount == 2) {
					addViolation(HW2OpType.TwoStringDeclaration);
				}
			}
			
			// Detect array declaration
			for(String s : varDeclarationTokens) {
				if(s.endsWith("[]")) {
					addViolation(HW2OpType.ArrayDeclaration);
				}
			}
		}
		
		/**
		 * Used to detect use of more than 1 line.
		 * @param ctx Contains the list of ParseTrees with symbols to check
		 */
		public void enterMethodBody(JavaParser.MethodBodyContext ctx) {
			if(ctx.getChild(0).getChildCount() > 3) {
				addViolation(HW2OpType.MultipleLines);
			}
		}
	}
	
	/**
	 * Allows for simple pairing of objects for the HashMap
	 */
	private static class HW2Pair<T1, T2> {
		public T1 t1;
		public T2 t2;
		public HW2Pair(T1 t1, T2 t2) {
			this.t1 = t1;
			this.t2 = t2;
		}
	}
}
