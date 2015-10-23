Plethora Technical Exercise - Akshay Mittal - October 23, 2015:
--------------------------------------------------------------

How to compile the code:
> make

This will generate a binary file workpieceQuote

How to run the binary after compiling:
> workpieceQuote <filename>

Example:
> workpieceQuote Rectangle.json

What the binary will output:
If the file exists and is formatted properly, the output will be the cost of machining the workpiece.

Example:
> workpieceQuote Rectangle.json
The cost of the workpiece is $14.10

What I would have done additionally if I had more time:
The code currently does not perform a thorough error check of the input file and the geometry specified for the workpiece. For example, the code will crash if the input file is not formatted correctly. Also, the code does not check for unphysical geometry specifications like intersections of segments.

