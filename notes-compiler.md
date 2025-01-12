
# Notes

Jai

- lexing
- parsing
- type checking
- sizing
- message pipe
- bytecode gen
- compile time execution
- machine code generator

Zig

per file (following is very parallelisable)
- tokenizer -> tokens
- parser -> ast
- astgen -> ZIR (IR that has no type analysis yet)
per function
- sema -> AIR (has semantic analysis done)
per ISA (arm/x86)
- codegen -> MIR (WASM, llvm, C)
- emit -> machine code

Zig tokens: enum of what it is + start integer, that's all (derive the length by the type)

Zig ast: 
struct AstNode {
    enum NodeType type;
    bool already_traced_this_node;
    size_t line; size_t column;
    ZigType *owner;
    union {
        AstNodeFnDef fn_def;
        AstNodeFnProto fn_proto;
        AstNodeParamDecl param_decl;
        AstNodeBlock block;
        AstNode * grouped_expr;
        AstNodeReturnExpr return_expr;
        AstNodeDefer defer;
        AstNodeTestDecl test_decl;
        AstNodeBinOpExpr bin_op_expr;
        AstNodeCatchExpr unwrap_err_expr;
        AstNodeUnwrapOptional unwrap_optional;
        AstNodePrefixOpExpr prefix_op_expr;
        AstNodePointerType pointer_type;
        AstNodeFnCallExpr fn_call_expr;
    }
}

ZIR 4 arrays
- tags
- common data
- extra array
- string hashtable

fast to produce, easy to save


Compiler Stages Explained:
Lexing (Lexical Analysis)
What it does: Splits the raw code into tokens (basic building blocks like keywords, identifiers, numbers).
Practical example: Turns int x = 10; into a series of tokens like int, x, =, 10, ;.
Parsing (Syntax Analysis)
What it does: Organizes tokens into a structured representation, like a syntax tree, based on grammar rules.
Practical example: Ensures int x = 10; is syntactically valid and produces a tree where x is a variable of type int assigned the value 10.
Type Checking (Semantic Analysis)
What it does: Validates that operations in the code adhere to type rules (e.g., you can't add a string to an integer).
Practical example: Checks that int x = "hello"; is invalid because "hello" isn’t an int.
Sizing
What it does: Calculates memory requirements for variables, objects, and data structures based on their type and usage.
Practical example: Allocates 4 bytes for an int and computes layout offsets in structs or arrays.
Message Pipe
What it does: Manages how diagnostic messages (errors/warnings) flow from different compiler stages to the user.
Practical example: Reports an error like "missing semicolon" at the correct code location after lexing or parsing.
Bytecode Generation
What it does: Translates the syntax tree or intermediate representation into bytecode (an intermediate, low-level representation).
Practical example: Converts x = x + 1 into bytecode instructions like LOAD x, ADD 1, STORE x for a virtual machine.
Compile-Time Execution
What it does: Executes parts of the code during compilation, often for optimizations or constant folding.
Practical example: Replaces 2 + 2 with 4 in the compiled code because it can evaluate it statically.
Machine Code Generation
What it does: Produces actual machine instructions tailored to the target CPU architecture.
Practical example: Converts a high-level addition operation to ADD R1, R2 for an x86 processor.




Sizing is not strictly required if targeting the EmitC dialect, as EmitC focuses on producing C-like intermediate code rather than dealing with low-level machine-specific memory layouts.

Why Sizing Isn't Necessary:
EmitC is High-Level:
It generates a C-like abstraction where variables and memory sizes are determined by the C compiler during its compilation stage, not during EmitC generation.
Deferral to C Compiler:
The responsibility for memory layout, alignment, and sizing is deferred to the C compiler (e.g., gcc, clang) that processes the emitted code.
EmitC's Role:
EmitC acts as a bridge between MLIR (Multi-Level Intermediate Representation) and C, focusing on preserving program semantics rather than hardware-specific details.
When Sizing Would Be Needed:
If the EmitC dialect is part of a pipeline where:

Lower-level dialects (e.g., LLVM) are used later in the process.
Memory layout or offsets must be explicitly defined before reaching the C backend.
In those cases, sizing might still occur earlier in the compilation pipeline before lowering to EmitC.





If you target LLVM, sizing is required because LLVM operates at a lower level where explicit memory layout and type sizes must be defined. LLVM expects precise details about the size and alignment of types to generate correct machine code.

Why Sizing is Necessary for LLVM:
Explicit Memory Management:
LLVM's intermediate representation (IR) requires knowledge of type sizes to correctly handle memory allocation (alloca), pointer arithmetic, and data structure layouts.
Example: If you declare an i32 in LLVM IR, the compiler must know it takes up 4 bytes.
Struct Layouts and Alignments:
When generating LLVM IR for complex data structures like structs or arrays, the sizes and offsets of each field must be computed to generate proper memory access instructions.
Example:
A struct like struct { int a; char b; } needs to account for alignment and padding rules in LLVM.
Pointer Arithmetic:
LLVM IR uses explicit instructions for operations like getelementptr, which require the exact size of types to calculate memory addresses.
Target-Specific Optimization:
LLVM's backend optimizations, such as loop unrolling or vectorization, depend on type sizes and alignments to generate efficient code.
Sizing in LLVM IR:
In practical terms, sizing is resolved during earlier compilation stages (in a frontend like Clang or through lowering from MLIR) before emitting LLVM IR. These details are embedded as part of the type system and alignment metadata in LLVM IR.

Example of LLVM IR with type sizes:

%struct.MyStruct = type { i32, i8 }
%1 = alloca %struct.MyStruct, align 4
%2 = getelementptr %struct.MyStruct, %struct.MyStruct* %1, i32 0, i32 1
Here, the struct's size and alignment are baked into the IR, making it essential for the compilation process.



Also consider

Cranelift
QBE
MLIR LLVM
MLIR EmitC
LLVM IR

MLIR SPMD
MIR (GCC)
Rust's MIR
JS Bytecode


When to Consider Each Option:

Goal	                    Recommendation
Portability	                WASM, EmitC
Runtime Performance	        LLVM IR, Cranelift, SPIR-V
Compilation Speed	        Cranelift, MIR
Heterogeneous Hardware	    MLIR, SPIR-V
Functional Semantics	    Core/Grin
Dynamic Language	        Sea of Nodes, Truffle/Graal
Ease of Prototyping	MLIR    Toy Dialect, EmitC


@google LLVM IR cheatsheet

https://mukulrathi.com/create-your-own-programming-language/llvm-ir-cpp-api-tutorial/

> So instead of a fixed number of physical registers, in LLVM IR we have an unlimited set of virtual registers 
> (labelled %0, %1, %2, %3… we can write and read from. It’s the backend’s job to map from virtual to physical registers.

> And rather than allocating specific sizes of datatypes, we retain types in LLVM IR. 
> Again, the backend will take this type information and map it to the size of the datatype. 
> LLVM has types for different sizes of ints and floats, e.g. int32, int8, int1 etc. 
> It also has derived types: like pointer types, array types, struct types, function types. To find out more, check out the Type documentation.

MIR looks pretty easy to write though

func add(i32 x, i32 y) i32
local i32 result

result = x + y
ret result
endfunc

MIR is experimental and requires mir-tools, mir-gen.h and https://github.com/vnmakarov/mir

It might be better to bite the bullet and do LLVM IR from the start.

