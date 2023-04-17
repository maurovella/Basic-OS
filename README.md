# Basic-OS
Basic System in C which calculates md5 hashes of files in a distributed manner.
For this purpose, the OS is split into three processes: application.c, view.c, and slaves

## Programming Considerations
* The code is written in "snake_case Standard"
* All functions are written in the following format:
```
returnType functionName (argumentType argumentName) {
    // code
} 
```
* All functions handle errors. Returning an error code and message named in the following format:
```
Message: "Error action."
Error Code: "ERR_ACTION"
```

## Project Structure

* application.c is the main process, it creates the slaves required in order to calculate all md5 hashes and it opens the shared memory and semaphores required in order to communicate with the vista process 
* slave.c is the function that control the slaves' actions. Receives multiple files and delegates individual files to subslaves for them to calculate md5 hashes.
* vista.c accesses the output of the application process in real time using the shared memory and semaphores created by the application process
* utils folder includes utility functions of files and file descriptors manipulations, shared memory and semaphore mechanisms
* include folder includes errors.h and the manager.h library with all the useful functions included in the utils folder to code application, slave, and vista

## Requirements
Programs have to be compiled and run using the following docker container:
```
user@linux:~$ docker pull agodio/itba-so:1.0
user@linux:~$ cd ./path/to/Basic-OS
user@linux:~$ docker run -v ${PWD}:/root --privileged -ti --name SO agodio/itba-so:1.0
user@docker:~$ cd root
```
## Compilation Instructions
In order to compile the program, inside the docker container run:
```
user@docker:~$ make
```
## Execution Instructions
There are 3 cases for executing the program

### Case 1
If you want to just calculate the md5 hashes of files run:
```
user@docker:~$ ./bin/application <files>
```

### Case 2
If you want to view the process in real time run:
```
user@docker:~$ ./bin/application <files> | ./bin/vista
```

### Case 3
If you want to have case 2 in two different terminals run:

#### Terminal 1
```
user@docker:~$ ./bin/application <files>
```
This program will output 2 names which *have to be used as arguments for vista* in the **same order** 
they appear.

#### Terminal 2
```
user@docker:~$ ./bin/vista <name1> <name2>
```

## Results
In any case of execution the results are stored in <code>respuesta.txt</code>
