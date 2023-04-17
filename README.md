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

# Requirements

# Compilation Instructions

# Ejecution Instructions