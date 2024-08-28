#pragma once

//introducing my next crackhead idea:
//KNFNASM - A new assembly language/virtual computer written to specifically interact with the tasks that SDLTetris has, and do nothing more than that!
//Is it designed for speed? No.
//Is it designed for readability? Yes.
//Does it do everything that the eventual full lua engine can do? No.
//Does it support Strings, and have a weird memory layout that im sure people will hate me for? YES!
//
//KNFNASM is inspired by 6502 assembly (albiet a much simpler version) and is designed with user comfort in mind. 
//It's not really an assembly language
//It's assembly for babies.
//The language is read from xml files, with opcodes being defined as a char for the opcode, a int for the index and a 64bit unsigned int for the value
//Addresses are first 8bits, values are the remaining 56 bits. 
// This is autohandled by the compiler, so syntactically to use this you want to do it like this: The inc command can be used as a good example of this.
// commands are represented below, and their results are represented in binary in the []
// compare accumulator? cmp(A) == [03,10000000000000000000000000000000000000000,0] //
// compare address? cmp($16) == [03, 10000,0]
// compare value? cmp(16) == [03,1000000000000,0]
//The following opcodes are supported, and work how they sounds
// lda(index) : loads from either an address or a value, depending on syntax. 