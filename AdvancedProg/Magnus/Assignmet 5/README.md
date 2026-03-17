# README assignment 2.



## ProgramTypeVisitor
The ProgramTypeVisitor is performing the type checking of the mini java AST. It visits each node of the program and checks the type of expressions and validates if the operations are used with floats or integers. It stores the types in a mapping and records errors if invalid types are used.
We also implemented type checking for while loops in the method: visit(Whileloop). Since mini java does not support boolean values the condition must have the type INT, otherwise an error is added to the list of problems

---

## ProgramExecutorVisitor
The ProgramExecutorVisitor executes our desired operations, and calculates and maps our expressions into a value of type int or float. In the ProgramExecutorVisitor, we have implemented different functions for operators. we made sure that each operator worked for both types float and int. The operators require the same types. the implemented operators are the following: “+,-,*,/,%” as well as the unary operations “+,-”. We also made a map which pairs the different types with the operators and their respective methods.

The printStatement method recursively visits each subexpression and evaluates each expression, to compute the value and storing it in the values map. it then prints each prefix and the respective value.

The while Loop implementation evaluates the “condition” of the while loop, it is implemented in a way, where the while loops condition acts as follows:
while(condition >= 0). it then proceeds to execute the body of the whileloop, and then re-evaluates the condition of the loop - determining whether or not the loop should continue to be executed.
 
---

## TestMiniJava
We implemented some extra tests, in order to make sure the implemented methods worked as they should. We also wrote some tests to make sure the program handled errors in the intended way.

---

## MiniJavaRun
The MiniJavaRun program is used to test the calculations (not JUnit tests) to see if our lambda functions are programmed properly. When given a correctly assembled AST using implemented operators it will then calculate using both
javas default and out custom implemented calculations, displaying them both.

---

## EXTRA:
We implemented some tests in the MiniJavaRun, in order to test out some unary operators, as well as the while loop. This was done to make sure we understood the implementations, and the recursive “flows” that followed.  



