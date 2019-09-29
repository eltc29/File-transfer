Download the repo to your computer. To run the program:
- Open two Linux terminals(one for the client and the other for the server) and change to directory you downloaded the files.
- First, run "gcc client.c -o client', then 'gcc server.c -o server" to compile the files.
- In one of the terminal, run "./server PORTNUMBER" (could be any port number,for e.g. 5000).
- Then, in the other terminal, run "./client 127.0.0.1 PORTNUMBER FILE1 FILE2".
    - 127.0.0.1 is used as the loopback address here but it could be any other server.
    - FILE1 is existing path on server and FILE2 is targeted path on the client's side which in this case is the current working directory 
