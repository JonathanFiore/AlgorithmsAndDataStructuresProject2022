# Algorithms And Data Structures - Degree Project 2022
## ***Score: 30/30L***


### Programming Language and Constraints:
> - <p>C11 - variable length array available<p> 
> - <p>Only C Standard libraries are allowed<p>
> - <p>No multithreading<p>
> - <p>Input from: stdin<p>
> - <p>Output to: stdout<p>

<br/>

---
<br/>

### Data Structures used:
> - <p>Adjacency List<p>
> - <p>Red Black Trees<p>
> - <p>Hash Tables (Adjacency List implementation)<p>

<br/>

---
<br/>

### Tools used:
> - <p>Compiler: GCC<p>
> - <p>Valgrind<p>
> - <p>Address Sanitizer<p>
> - <p>IDE: I used a simple Text Editor: Sublime Text<p>


<br/>

---
<br/>

### Evaluation Method:
 #### ***3 Test batteries***:
> - <p>18 (pass or fail)<p>
> - <p>12 points to get from 18 to 30 (6 tests of 2 points each)<p>
> - <p>Cum laude<p>

All the batteries test the **Execution Time** and **Space occupied** during Execution

<br/>

---
<br/>

### Description: 
<p>The purpose of this project is to create a system whose main goal is to check the correspondence between the letters of two words with the same length<p>

<p>There is a group of permissible words, a specific one in this set of words is chosen at the beginning of the program, we will call di word "reference word"<p>

<p>There are 'n' comparisons between the reference word and 'n' words given in input; each comparisons produces constraints and allow to create a group of filtered words which is a subgroup of the permissible words.
The filtered group contains all the words which follow the constraints learned during the comparisons<p>

<p> You always have to be able to print the list of filtered words in lexicographic order<p>

<p>Example of comparison and constraints learned:<p>

**reference word = RF = a b c a b c a b c a b c a b c** <br>
**word to compare = b b a a b c c b c c b c a b c**

So we have: <br>
**a b c a b c a b c a b c a b c** <br>
**b b a a b c c b c c b c a b c** <br>
**--------------------------** <br>
**/ + | + + + | + + / + + + + +** **= Constraints**

**'/'** means that the specific letter is not present in the RF

**'|'** means that the specific letter is present in the RF but it's not in that specific position

**'+'** means that the specific letter is present in the RF and it's in that position as well
