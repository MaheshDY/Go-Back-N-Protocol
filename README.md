The objective of this project is to model, simulate and analyze the Go-Back-N protocol. As explained in the lecture slides, Go-Back-N protocol is based on sliding window flow control method.
The project can be implemented by extending the simple tic-toc model. The following features should be demonstrated. 
Station A (e.g., TIC) sending packets at specified rate towards Station B (e.g., TOC)
The following variables should be configurable via the omnetpp.ini file,
1.	The data rate. That is, the rate at which TIC sends packets to TOC
2.	The window size for TOC. That is, the number of packets TIC can send towards TOC without receiving ACKs from TOC.
3.	The size of N. That is the number of packets that the receiver receives before sending an acknowledgment. 
4.	The packet loss/error rate. This should be random. HINT: use exponential function.
Keep the size of sequence number field in the packets to 8 bits, to allow maximum number of 256 sequence numbers before the sequence number wraps around.
At the start of the simulation, the TIC should send a control message to TOC querying its window size, which is configured in the TOC at the time of initialization via the omnetpp.ini file. The TOC then replies to the query from TIC indicating the window size. i.e., the receive (TOC) buffer size. This implies that you have to implement a buffer in the TOC (i.e., receiver side). HINT: Use the omnet class cQueue for implementing the buffer. 
