
**Description:**

There is a single server and 5 clients from client1 to client5.

1.Create files like " Details.txt  Grade.txt in server directory "

**Functionality:**

1.All the 5  clients and  Server will be running in different terminals.

2.User enters the filename at the client end.The client will send the filename to server through socket.The Server will fetch the data from the file and sends it to client through another queue.

**Execution steps:**

1.Create files as in the description.

2.All the 5  clients and  Server should be running in different terminals.

2.Enter the filename at the client side.The Server will send the data from the file if the file exists.It will throw a message as "File Not Found" if file not present at server side.


**2.Database Engine**
   
Time taken (ms)

|S.No:| 1 client | 2 clients | 3 clients | 5 clients | 
| --- | ---     |---|---|---|
| 1 | 1.685 | 2.625 | 2.912 | 3.550
| 2 | 1.245 | 2.017 | 2.492 | 4.131
| 3 | 1.825 | 2.744 | 3.287 | 3.624
| 4 | 1.565 | 3.091 | 3.165 | 3.814
