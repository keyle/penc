##
## implementation
##

# Pure shunting-yard algo

Higher precedence operators (* in this case) must be resolved before lower precedence operators (+).

This means when processing b * c, the multiplication is resolved and placed in bytecode before handling the addition (+).

```

1. print(a * b + c)
   ["a", "b", "*", "c", "+", "print"]

2. print(a + b * c)
   ["a", "b", "c", "*", "+", "print"]


1. when we look at "+" we see we have "*" in the stack, we pop * and add it to bytecode before stacking +

2. when we look at "*" we see we have "+" in the stack, we stack * and then pop both later

higher precedence pops


```


Push Operators: Push an operator onto the stack if it has higher precedence than the operator on top of the stack.

Pop to Bytecode: If the current operator has lower or equal precedence, pop operators from the stack to the bytecode until the precedence rule is satisfied.

```

print(a(b(c + d)))

c d + b a print

the function "a" has higher priority, const literals and vars are the lowest, so a goes on the stack

if they're equal, e.g. two functions, they go on the stack

```

We DO NEED TO KNOW what a is, a LOAD var vs. a FUNC IDENT.


## extending for advanced blocks

```

if (a > b) c = d; else c = f;


["a", "b", ">", "jump_if_false L1", "d", "c", "=", "jump L2", "L1:", "f", "c", "=", "L2:"]

c = a > b ? d: f;

["a", "b", ">", "jump_if_false L1", "d", "jump L2", "L1:", "f", "L2:", "c", "assign ="]

```

Labels to track true and false paths.
Conditional jumps for ? and :.


## while

```
while(a > b) { b++; }

L1:, a, b, >, jump if false L2, b, 1, +, b, =, jump L1, L2:
                                -------------
                                b++
                                
do { b++; } while (a > b);

L1:, b, 1, +, b, =, a, b, > jump if true L1  #  note this is jump if true for do,while


```


object properties etc.

```

obj.property;

obj, property, resolve_member


obj.method(arg1, arg2);

obj, arg1, arg2, method_name, call_method

```

object recration etc.


```

new Something(arg1, arg2);

Something, arg1, arg2, new_instance

```


```

obj.attach(new Something(arg));

obj Something, arg, new_instance, attach, method_call

```


## Challenges in Object-Oriented Parsing

Dynamic Dispatch:
    For languages with polymorphism, call_method may need runtime resolution.

Nested Member Access:
    Syntax like obj.property.method() requires resolving multiple levels of member access.

Static vs. Instance Context:
    Parsing must distinguish between static methods (Class.method()) and instance methods (obj.method()).


```

f(x, y, z)

x y z f, call

```


##Tricky, lambdas:

Treat the Lambda as a Function:
    Capture the lambda's parameters (a, b) and body (a + b) as a block of code.
    Generate a label (L1) for the lambda body.

Emit Bytecode for the Lambda Body:
    Parse the body (a + b) and push the result.
    Emit a return instruction to signify the end of the lambda.

Assign the Lambda:
    Use the := operator to bind the lambda to the variable x.


```

x := (a, b) => a + b;

["L1:", "a", "b", "+", "return", "x", ":=", "L1"]


x := (a) => (b) => a + b;

["L2:", "b", "a", "+", "return", "L1:", "a", "L2", "return", "x", ":=", "L1"]


```


## try catch


```
try {
  something();
} catch(e) {
  print(e);
} finally {
  print("recovered");
}

```


```
["try_start1:", "call something", "jump finally_start3", 
                                                        "catch_start:", "e", "print", 
                                                                                    "finally_start3:", "'recovered'", "print", 
                                                                                                                                "try_end:"]
```

we might need placeholders and rewind the stack to set the labels after being named. we define finally_start3 after the jump to finally start3.

call it `__finally_start__` temporarily, go back up and set the label

it means we need to track we're in a try/catch, and that also needs to be in a stack.



