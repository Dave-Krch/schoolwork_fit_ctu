# stable_mariage_problem

Algorithm that creates stable pairs from list of preferences

Input:  
*N N*  
*N (list of all people)*  
.  
*2N lines*  
*.*  
*N (list of all people)*  
  

- First line: number of men and women (always same)
- Lines 1 -> N are mens preferences, lines N+1 -> 2N women preferences
= men have ids from 1 and women from N+1..
- First there is a size of a list, then list of preferences for given person
- First two given numbers, and number at the start of each line with preference list are equal (N), which is unnecessary. But I wrote this code as a school assignment, so i had to stick to the input form.

Output:
- 2N lines
- on line **i** is partner of person with id **i**

Example input:  
1 1  
1 2  
1 1  

Example output:  
2  
1