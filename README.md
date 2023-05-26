# 1837837
Başlığı anlayamadınmı ? O zaman birde tersten bak ;)


## 1 - Introduction
&ensp; The Advanced Calculator project is an interpreter which is based on some pre-designed assumptions. This interpreter implementation aims to calculate advanced infix operations and functions using the C programming language. 	

&ensp; Specifically, this project takes inputs as variables, functions, integers or assign statements, and after calculation, it returns an integer-valued answer.

## 2 - Program Interface 
&ensp; To use the Advanced Calculator project, you don't need to install anything. Once you install the folders properly, you can use them from the terminal just by writing make command. This command will create an executable file in the root folder which is named “advcalc”. Then you can execute it with ./advcalc command in the root directory. 

&ensp; When the program is started a line will open for you to enter input. Here you can enter the expression you want to know the result of and you can assign it to a variable if you want.

 &ensp; To terminate the program you can use Ctrl + D on Unix and Linux systems to exit a terminal or shell. More detail will be given in the following sections.
             
## 3 - Program Execution
&ensp; The advanced Calculator interpreter takes inputs as a string and calculates the input while paying attention to precedence. The interpreter is only capable of evaluating some specific infix operations. 

&ensp; Inputs are given using “Enter”, and the program is terminated using “CTRL-D”. While terminating also “CTRL-D” gives output for the last input just like “Enter”. The program continues taking inputs if it’s not terminated. Even if it’s given wrong inputs, it just gives an “Error !” output and continues to take inputs from the user. All possible errors and unexpected inputs are handled by “Error !” warning.

&ensp; After tokenizing lexemes, the program also memorizes the variable that you have assigned as long as you do not terminate it. (if it’s not assigned, the program takes it as zero) In the end, if the input is valid, the program gives the output.
