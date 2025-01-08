# Shunting-Yard stack based approach parser

- two stacks needed
    1. bytecode stack (OUTPUT)
    2. temporary stack (WORK) (e.g. RETURN <expr> pushes RETURN until 
       after expr has been resolved and pop() from temporary onto opcode stack.
       in case of an IF we also store the jump_if_false there with its temporary label

Demo

    `return x > 0 ? y.length() : 10;`

```
               OUTPUT        WORKBENCH 
    ------------------------------------------------------
    'return'                 RETURN  
    'x'        LOAD X
    '>'                      GT
    '0'        LOAD 0
    POP        GT
    '?'        JUMP_IF_FALSE label_else  ; start of THEN
    y          LOAD Y
    .length    CALL LENGTH
               JUMP label_end            ; end of THEN
    ':'        label_else:               ; start of ELSE
    '10'       LOAD 10
               label_end:                ; condition finished
    POP        RETURN

```

see `EDGE_CASE` at the bottom for operator precedence.


program scope
    type
        fields
        funcs
    enum
    func
    globals

func scope
    params
    locals
    outers
    assigns
    apply/call
    declarations
    statements
    expression_statements


# trying to avoid an AST this time

for precedence, shunting-yard algorithm, basically go from

```
x = 2 + 5 * 2 to, by ordering, stacking and popping 

['x', '=', 2, 5, 2, '*', '+']

```

push on `(`, pop until `)`


```
x = 2 + 5 * (3 + 1)

Execution Flow:

2 → Push to output
+ → Push to operators
5 → Push to output
* → Push to operators (higher precedence)
( → Push to operators
3 → Push to output
+ → Push to operators
1 → Push to output
) → Pop and apply + → Push result to output
Apply * → Push result to output. (5 * 4)
Apply + → Push result to output. (2 + 20)
Apply = → Assign value to x
```

if/else become special control flow operators
JUMP_IF_FALSE, otherwise continue

for loops etc. is a label,
    condition check every time
    jump if false -> out
    (otherwise continue within block and jump back to top unconditionally)


struct
    op_code, add.., load_const, load_var, store_var, jump, jump if false, call, return, break
    operand
    label
    constructor


```
for (i = 0; i < 10; i = i + 1) {
    print(i);
}
```

```
std::vector<Bytecode> bytecode = {
    // Initialization: i = 0
    Bytecode(OpCode::LOAD_CONST, {0}),
    Bytecode(OpCode::STORE_VAR, {"i"}),

    // Condition: i < 10
    Bytecode(OpCode::LOAD_VAR, {"i"}),
    Bytecode(OpCode::LOAD_CONST, {10}), // operand literal 10
    Bytecode(OpCode::SUB),
    Bytecode(OpCode::JUMP_IF_FALSE, "label_end"),

    // Loop body: print(i)
    Bytecode(OpCode::LOAD_VAR, {"i"}),
    Bytecode(OpCode::CALL, {"print"}),

    // Increment: i = i + 1
    Bytecode(OpCode::LOAD_VAR, {"i"}),
    Bytecode(OpCode::LOAD_CONST, {1}),
    Bytecode(OpCode::ADD),
    Bytecode(OpCode::STORE_VAR, {"i"}),

    // Jump back to condition
    Bytecode(OpCode::JUMP, "label_condition"),

    // End of loop
    Bytecode(OpCode::RETURN)
};
```

bytecode = 
    opcodes, ADD, MUL, JUMP,  CALL,                            NEW
    operands, 42, i, PI, {0}, @User::init, @myFunction, @puts, MyClass
    labels, loopstart, end, (no opcode linked)
    and constructors, ad-hoc func call (mem alloc, + custom for inheritance etc) (passed in calls)

+ lookup tables for additional metadata

labels:
    can act as identifier of list of bytecode
    switched by JUMP or CALL & returned
    can help reducing duplications



# if using emitc, consider a higher level bytecode

DEFINE_VAR	Declares a new variable	emitc.store
ASSIGN_VAR	Assigns a value to a variable	emitc.store or arith.addi
IF_COND	Conditional branching with THEN and ELSE blocks	scf.if
FOR	For-loop with initialization, condition, and increment	scf.while or scf.for
CALL	Function or system call	emitc.call
EXPR	Represents an expression like a + b	arith.addi, arith.cmpi, etc.

if x > 0 {print(1)} else {print(2)} 

DEFINE_VAR x
IF_COND x > 0
THEN_START
CALL print "1"
THEN_END
ELSE_START
CALL print "2"
ELSE_END
END_IF

# Final goal (also see below for two options)

use a simple flat list in an IR style of shunting yard.

## Flat Intermediate Representation (IR)

```
if (x > 0) {
    y = 1;
} else {
    y = -1;
}
```

becomes

```
LOAD_VAR x      # x
LOAD_CONST 0    # 0
GT              # >
JUMP_IF_FALSE label_else
LOAD_CONST 1
STORE_VAR y
JUMP label_end
label_else:
LOAD_CONST -1
STORE_VAR y
label_end:
```

# Function declaration

Either

- opcodes with metadata
- separate func from metadata

```
int a(int x, string y) {
    return x > 0 ? y.length() : 0;
}


FUNC a : int, params: [int, string]
PARAMETER_START
LOAD_PARAM 0 : int
LOAD_PARAM 1 : string
PARAMETER_END

LOAD_PARAM 0
LOAD_CONST 0
GT
JUMP_IF_FALSE label_else

LOAD_PARAM 1
CALL length
JUMP label_end

label_else:
LOAD_CONST 0

label_end:
RETURN
```


# Look ahead?

instead of a look ahead, we can:

### Use a Placeholder for Jumps (update the label later)

- Parse the Condition: Generate bytecode for evaluating x > 0.
- Emit JUMP_IF_FALSE label_??? with a placeholder.
- Parse on and then replace the label when the else or end is known

### Delay emission with backpatching (update the jump later) <--

- Parse the condition and generate bytecode
- record the location of the jump in false,
- when finished update the location of the jump to the appropriate label

### Reverse Emit using a stack

- not as flexible as the other two methods.


# Infix vs Postfix notation

infix:

1. load x
2. load 0
3. op >

postfix:

1. op >
2. load x
3. load 0

postfix may be more compact: 

1. > x 0 

(implicit loads)

harder to debug

# Processing code

### Store and delay 

`(a() > b())`

- encounter a
- CALL A
- encounter GT >
- store GT onto a stack or queue
- encounter b
- resolve b with CALL B
- pop GT and compare the result of a and b


We call A immediately as it fits our left-first model and keeps it simple.

if we stored OPCODE and Operand on a stack before resolving, we'd be resolving b() first.

```
CALL B <-- top (wrong)
CALL A
GT
```

We emit and evaluate first, before comparing

```
CALL a                ; Resolve a()
CALL b                ; Resolve b()
GT                    ; Compare results of a() and b()
JUMP_IF_FALSE label_else
CALL print "greater"  ; Execute if a() > b()
label_else:
...
```


# Typechecking

we can embed the type definition within the bytecode to stop at static analysis instead
of dynamic checking


```
CALL a : int
CALL b : string
GT : int, string -> bool   ; int,string are the types from above, clearly incompatible

```


## Analysis

static - syntactic correctness and type soundness (via symbol table and type system)
    do variables exist, are functions valid - linters / clangd
    
semantic - validation of program behaviour and compatibilities (rules of the lang)
    can type be divided, do func signature match type passed - compiling

dynamic checking - catches runtime error, div/0, nullptr access  (robustness/safety)

control flow - dead code, validity of flow paths

data flow - unused variables, use before set, replace vars with const if possible, 
    int x = 10; x = 5 ; x maybe is a const that is 5 from the start.

alias checks - pointers ownership, concurrency races, memory access order, borrow check

escape analysis - heap to stack conversion for local vars

type inference - x = 10 must be an int

mem safety - buffer over/underflow, uninit memory, dangling ptrs, leaks, prevent segfaults


# Code structure

e.g.

```
struct Point {
    int x;
    int y;
};

int add(int a, int b) {
    return a + b;
}

int main() {
    Point p;
    p.x = 5;
    p.y = 10;
    return add(p.x, p.y);
}
```

Two options


1. flat list representation

```
Program program {
    // Struct Definition
    Instruction(Opcode::STRUCT, { Operand("Point") }),
    Instruction(Opcode::FIELD, { Operand("x"), Operand("int") }),
    Instruction(Opcode::FIELD, { Operand("y"), Operand("int") }),
    Instruction(Opcode::END_SECTION),
    
    // Function add
    Instruction(Opcode::FUNC, { Operand("add"), Operand("int") }),
    Instruction(Opcode::PARAMETER_START),
    Instruction(Opcode::LOAD_PARAM, { Operand(0), Operand("int") }),
    Instruction(Opcode::LOAD_PARAM, { Operand(1), Operand("int") }),
    Instruction(Opcode::PARAMETER_END),
    Instruction(Opcode::LOAD_PARAM, { Operand(0) }),
    Instruction(Opcode::LOAD_PARAM, { Operand(1) }),
    Instruction(Opcode::ADD),
    Instruction(Opcode::RETURN),
    Instruction(Opcode::END_SECTION),
    
    // Function main
    Instruction(Opcode::FUNC, { Operand("main"), Operand("int") }),
    Instruction(Opcode::VAR, { Operand("p"), Operand("Point") }),
    Instruction(Opcode::LOAD_CONST, { Operand(5) }),
    Instruction(Opcode::STORE_FIELD, { Operand("p.x") }),
    Instruction(Opcode::LOAD_CONST, { Operand(10) }),
    Instruction(Opcode::STORE_FIELD, { Operand("p.y") }),
    Instruction(Opcode::LOAD_FIELD, { Operand("p.x") }),
    Instruction(Opcode::LOAD_FIELD, { Operand("p.y") }),
    Instruction(Opcode::CALL, { Operand("add") }),
    Instruction(Opcode::RETURN),
    Instruction(Opcode::END_SECTION)
};


STRUCT Point
    FIELD x : int
    FIELD y : int
END_STRUCT

FUNC add : int, params: [int, int]
PARAMETER_START
LOAD_PARAM 0 : int
LOAD_PARAM 1 : int
PARAMETER_END
LOAD_PARAM 0
LOAD_PARAM 1
ADD
RETURN
END_FUNC

FUNC main : int
VAR p : Point
LOAD_CONST 5
STORE_FIELD p.x
LOAD_CONST 10
STORE_FIELD p.y
LOAD_FIELD p.x
LOAD_FIELD p.y
CALL add
RETURN
END_FUNC

```

2. structured list representation

```
struct Program {
    std::vector<StructDefinition> structs;
    std::vector<Function> functions;
    std::vector<Instruction> globalInstructions; // For global imperative code
};

struct StructDefinition {
    std::string name;
    std::vector<std::pair<std::string, std::string>> fields; // Field name and type
};

struct Function {
    std::string name;
    std::string returnType;
    std::vector<std::pair<std::string, std::string>> parameters;
    std::vector<Instruction> bytecode; // Flat bytecode for the function body
};

struct Instruction {
    Opcode opcode;
    std::vector<Operand> operands;
};

...


Program program {
    // Struct Definitions
    {
        StructDefinition{
            "Point",
            { {"x", "int"}, {"y", "int"} }
        }
    },
    // Function Definitions
    {
        Function{
            "add",
            "int",
            { {"a", "int"}, {"b", "int"} },
            {
                Instruction(Opcode::LOAD_PARAM, { Operand(0), Operand("int") }),
                Instruction(Opcode::LOAD_PARAM, { Operand(1), Operand("int") }),
                Instruction(Opcode::ADD),
                Instruction(Opcode::RETURN)
            }
        },
        Function{
            "main",
            "int",
            {},
            {
                Instruction(Opcode::VAR, { Operand("p"), Operand("Point") }),
                Instruction(Opcode::LOAD_CONST, { Operand(5) }),
                Instruction(Opcode::STORE_FIELD, { Operand("p.x") }),
                Instruction(Opcode::LOAD_CONST, { Operand(10) }),
                Instruction(Opcode::STORE_FIELD, { Operand("p.y") }),
                Instruction(Opcode::LOAD_FIELD, { Operand("p.x") }),
                Instruction(Opcode::LOAD_FIELD, { Operand("p.y") }),
                Instruction(Opcode::CALL, { Operand("add") }),
                Instruction(Opcode::RETURN)
            }
        }
    },
    // Global Instructions
    {}
};

```

Not an AST: 

 - the body of each function is a flat list of instructions, not nested nodes
 - struct and function declarations are just metadata for organizing bytecode
 - no parent-child links between constructs

Pros/cons

flat list

- (+) simpler execution
- (+) lower memory usage
- (-) harder to optimize
- (-) force a simpler language

structured list

- (+) easier complex semantic analysis
- (+) easier optimizations
- (-) more memory


# EDGE_CASE: 2 * 3 + 4

                   OUTPUT           STASH

                   LOAD_CONST 2
'*'                                 MUL
                   LOAD_CONST 3
'+'                                       ;look at the + and check that there is not already a competing operation in the stash
                                          ; if so, resolve the priority MUL > PLUS, so POP MUL and stash PLUS
'4'                LOAD_CONST 4
POP                MUL
                                    ADD
POP                ADD

```
#include <cstdint>

enum class Precedence : uint8_t {
    Comma = 1,           // Lowest precedence
    Assignment,          // = += -= *= /= %= &= |= ^= <<= >>= >>>=
    Ternary,             // ?:
    LogicalOr,           // ||
    LogicalAnd,          // &&
    BitwiseOr,           // |
    BitwiseXor,          // ^
    BitwiseAnd,          // &
    Equality,            // == !=
    Relational,          // < <= > >= instanceof
    Shift,               // << >> >>>
    Additive,            // + -
    Multiplicative,      // * / %
    Unary,               // + - ++ -- ~ !
    Primary              // () [] . (Highest precedence)
};


constexpr Precedence getPrecedence(char op) {
    switch (op) {
        case '+': return Precedence::ADD;
        case '*': return Precedence::MUL;
        default: return Precedence::NONE;
    }
}

...

if (getPrecedence(workbench.top()) >= getPrecedence(currentOperator)) {
    // POP and emit
}
```




# Scopes

Use another stack, called the Scope stack, that tracks *active* scopes by tag name.

Var declarations get tagged with the scope, making it easy later to resolve correct or
incorrect scope access.

scopes must have a unique label (e.g. an increment counter++) and contain the `start`
`bytecode[index]` to jump there quickly(?)

alternatively we might want to keep a lookup table of all declarations within scopes
to avoid having to jump and browse through every line. A very long function might have
declarations way down the line, making it expensive to look up that variable.


`return x > 0 ? { int temp = a(); return temp + b(); } : temp;`

```
LOAD_VAR x                 ; Load x
LOAD_CONST 0               ; Load 0
GT                         ; Evaluate x > 0
JUMP_IF_FALSE else_label   ; Jump to the false block if x <= 0

; True Block
ENTER_SCOPE #zxc           ; Enter the block scope and tag it #zxc
CALL a                     ; Call a()
STORE_VAR temp #zxc        ; Store result in block-local variable 'temp' in tagged scope
LOAD_VAR temp #zxc         ; Load 'temp' in tagged scope
CALL b                     ; Call b()
ADD                        ; Add temp + b()
RETURN                     ; Return the result
EXIT_SCOPE #zxc            ; Exit the block scope
JUMP end_label             ; Skip the false block

else_label:
LOAD_VAR temp              ; Load 10

end_label:
RETURN                     ; Return the final result
```

So if the else block tries to access `temp`, we'd go looking up in the bytecode 
for temp, we find it with scope `#zxc`

we'd find `#zxc` isn't in the current stack of scopes, 
meaning the variable is not in this scope nor in a parent scope, so we deny it.


### further thoughts on scope

```
                # scope A

x = 10;

if (a > b) {    # scope B
    int x = 50; 
    print(x); 
} else {        # scope C
    print(x); 
}
```

The else prints 10 because the scope stack is `[C:50, A:1]` (scope:bytecode_index) 
`temp` doesn't exist in `C` so it looks in `A`.

However `x = 10;` could be declared way down the line and not be near `bytecode[1]`,

Storing a symbol table might make more sense


```
Variable   | Scope ID | Bytecode Location
-----------------------------------------
x          | A        | [1]  (global x)
x          | B        | [10] (local x in if block)
```

make it a hashtable `var_name:scope_id : bytecode_index` or more metadata


### another approach to scopes

Alternatively, we *could* maybe, enforce that our bytecode rewrites itself 
so declarations are always at the start of a block. 

Since symbols cannot be redefined within a block, 
that would be reducing the search space. 
The question becomes whether to prioritise memory or lookup speed

We could look at having a bytecode as linked list
