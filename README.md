Project : grep
Name : Asher Everen
MIS : 111608022

options implemented :

-r : reads all files from the directory recursively and prints the matches.
-i : ignores case while matching the pattern.
-v : prints all non matching lines.
-f : takes each line from file given with -f as a pattern and then prints the 
     matching lines.
-w : selects only matching lines that contain whole words. It must be at the 
     beginning of the line or preceded by a non word constituent or at the end
     of the line or followed by a non word constituent.
-c : prints the number of matching lines.
-m : prints only specified number of matched lines.
-b : prints the number of bytes read before the matched line.
-q : does not print anything to standard output, exits with zero status if any
     match was found, even if any error was detected.
-H : prints the filename for each match.
-h : does not print the file names.


