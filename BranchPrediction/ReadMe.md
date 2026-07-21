# Branch prediction


Introduction:
    Branch prediction is a CPU feature that tries to guess which path your code 
will take so the processor can stay busy instead of waiting. When the CPU guesses 
correctly, execution is fast. When it guesses wrong, the pipeline must be flushed 
and restarted, which costs 10–20 cycles per failure.

  These penalties matter in tight loops, interpreters, game engines, and any 
system where predictable timing is important. Developers use techniques such as 
lookup‑table dispatch, bitwise masking, and data‑oriented design to avoid 
unpredictable branches and keep execution deterministic.


Branch Example:

  This project demonstrates how branch predictability affects performance by 
comparing two tiny “virtual machines.” Both versions:

  1. Share the same Context struct

  2. Use the same handler functions

  3. Process the same randomly generated opcode list

The only difference is how each VM selects a handler:

  Bad Version — Switch Dispatch

  Uses a switch(opcode) to choose a handler.
  Switches are normally fast, but only when the input pattern is predictable.
  Random opcodes make the branch predictor fail constantly.
    
  Good Version — Lookup‑Table Dispatch

  Uses handlers[opcode](ctx) to call the handler directly.
  This removes the branch entirely, so the CPU has nothing to predict.
  Execution stays stable even with random input.


Important Note:
    Switch statements are excellent when the input follows a pattern the CPU can 
learn. They often compile into efficient jump tables and are widely used in 
interpreters, state machines, and protocol parsers.

In this demo, the switch performs poorly only because the input is intentionally 
random, giving the branch predictor no pattern to learn. The lookup‑table version 
succeeds because it removes the branch entirely.

This distinction is crucial:

  The issue is not “switch bad.”
  The issue is unpredictable branching.


Compiling Instructions:

  Compile using the following line:
      g++ Test.cpp -03 -o Test.exe

  Run in terminal to see results in nanoseconds, using following command:
      ./Test.exe


# EOF
