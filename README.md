## Small
### What is this?
An Intel 8086 microprocessor emulation created in C++ that can only execute simple bytecode instructions

### How does this work?
Small allocates 4MB (0x1000 bytes) of memory on the heap, and that serves as the main memory location wherein instructions are allocated using the small::alloc method. Small emulates the general purpose 16-bit registers in the Intel 8086 microprocessor AX, BX, CX, DX, all of which are interchangeable as, contrary to the actual hardware, they have no intended purpose. Additionally, Small mimics the Intel 8086 microprocessor's little endianness. Each register is split into the 8-bit lo and hi bytes - for example, AL and AH. Additionally, the IP register is used to execute bytecode from virtual memory using a simple byte parser I made. The IP register points to the instruction currently being executed.

Opcodes that are available include mov, jmp, inc, dec, int, and quit (which doesn't exist, but shoot, I made it anyway.) These pretend BIOS interrupts were used to interact with the terminal using ANSI escape codes. There are 4 interrupts available:

int  | description
---- | ------------------------------------------------------
0x00 | move terminal cursor to DH,DL
0x01 | set foreground terminal color to CL,DH,DL (rgb)
0x02 | set background terminal color to CL,DH,DL (rgb)
0x03 | print to std::out a byte at location DX as a character

Somewhere on my computer there exists a version of Small with add, sub, mul, and div opcodes. I never completed it as I was concurrently in the process of creating my own terminal emulator using OpenGL, which was more difficult than I had originally thought. I wouldn't have had to try to make my own terminal emulator if Mac zsh wasn't so finicky.

### How do I use it?
The file "main.cpp" shows how to use Small in C++ with comments.

```C++
init();
//------------ SOURCE --------------//
    alloc(  movcl,  (u8)0xFF    );  // red component set to 255
// loop                             //
    alloc(  movdx,  (u16)0x0000 );  // green, blue components set to 0
    alloc(  intr,   (u8)0x02    );  // change background color of text
    alloc(  movdx,  (u16)0x0014 );  // pass address of space character to dx register
    alloc(  intr,   (u8)0x03    );  // print char to stdout
    alloc(  deccl               );  // decrement cl register 5 times
    alloc(  deccl               );  //      because cl is the red component
    alloc(  deccl               );  //
    alloc(  deccl               );  //
    alloc(  deccl               );  //
    alloc(  jmp,    (u16)0x0002 );  // jump back to address of loop
//------------- DATA ---------------//
    alloc(          (u8)' '     );
exec();
```

The file has already been compiled for you and can be executed as follows:
```Bash
chmod +x main
./main
```

...producing the following infinite output, assuming you are using an ANSI-compatible terminal. You can press CTRL-C to stop it:
![Small Output](https://i.imgur.com/EEANFj5.jpg)

### What was the goal of this project?
Small was intended to refine my C++ skills and try something new. I've always been fascinated with lower level languages, virtual machines, and the inner workings of the main components of a computer.
