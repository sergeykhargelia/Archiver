# Archiver

A console application that can compress files by using the Huffman algorithm.

### Building

Run `cmake .` command at the root of the project and then run `make` command. It generates two executable files, `hw_02_test` and `hw_02`.
Run first executable, if you want to test the program on integration tests, and run second, if you want to execute the program on the custom test.    

### Usage

You can run the `hw_02` application with the following syntax:
```shell
./hw_02 -f <input_file> -o <output_file> <-c|-u>
``` 

#### Flags: 
* `-f`/`--file` needed to specify the name of the input file 
* `-o`/`--output` needed to specify the name of the output file
* `-c`/`-u` needed to specify the type of the operation: zip or unzip, respectively 
