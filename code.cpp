//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 2003-2015 Andras Varga
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//
#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include <unistd.h>
int count;
int load;
int self;
int lose;
int Window_size=5;
int b[10];
int flag=0;
int count1;
int count2;
int seq; // message sequence number
using namespace omnetpp;
/**
* In the previous model we just created another packet if we needed to
* retransmit. This is OK because the packet didn't contain much, but
* in real life it's usually more practical to keep a copy of the original
* packet so that we can re-send it without the need to build it again.
*/
class Tic9 : public cSimpleModule
{
 public:
 simtime_t timeout; // timeout
 cMessage *timeoutEvent; // holds pointer to the timeout self-message
 //int seq; // message sequence number
 cMessage *message[50]; // message that has to be re-sent on timeout
 public:
 Tic9();
 virtual ~Tic9();
 virtual cMessage *generateNewMessage();
 virtual cMessage *generateNewMessage1();
 virtual void sendCopyOf(cMessage *msg);
 virtual void sendCopyOf1(cMessage *msg);
 virtual void initialize() override;
 virtual void handleMessage(cMessage *msg) override;
};
Define_Module(Tic9);
Tic9::Tic9()
{
 timeoutEvent = message[0] = nullptr;
 }
Tic9::~Tic9()
{
 cancelAndDelete(timeoutEvent);
 delete message[0];
 }
void Tic9::initialize()
{
 // Initialize variables.
 seq = 0;
 int i;
 timeout = 1.0;
 timeoutEvent = new cMessage("timeoutEvent");
 // Generate and send initial message.
 EV << "Sending Control message\n";
 self=1;
 if(self == 1){
 message[0]= generateNewMessage1();
 sendCopyOf1(message[0]);
 // load=getName();
 }
 scheduleAt(simTime()+timeout, timeoutEvent);
}
void Tic9::handleMessage(cMessage *msg)
{
 if (msg == timeoutEvent) {
 int i;
 // If we receive the timeout event, that means the packet hasn't
 // arrived in time and we have to re-send it.
 EV << "Timeout expired, resending message and restarting timer\n";
 i=b[0];
 count1=b[0];
 while(i < Window_size){
 flag=8;
 sendCopyOf(message[i]);
 i++;
}
 scheduleAt(simTime()+timeout, timeoutEvent);
 }
 else { // message arrived
 // Acknowledgement received!
 EV << getName();
 EV << "Received window size: " << msg->getName() << "\n";
 if (strcmp("Window_size", msg->getName()) == 0){
 load=Window_size;
 EV << "window size is: " <<Window_size << "\n";
 }
 //delete msg;
 // Also delete the stored message and cancel the timeout event.
 EV << "Timer cancelled.\n";
 cancelEvent(timeoutEvent);
 //delete message;
 for(int i=0;i<load;i++){
 void wait(simtime_t time);
 // Ready to send another one.
 message[i] = generateNewMessage();
 sendCopyOf(message[i]);
 }
 scheduleAt(simTime()+timeout, timeoutEvent);
 }
}
cMessage *Tic9::generateNewMessage1()
{
 // Generate a message with a different name every time.
 char msgname1[20];
 sprintf(msgname1, "Control Message");
 cMessage *msg = new cMessage(msgname1);
 return msg;
}
cMessage *Tic9::generateNewMessage()
{
 // Generate a message with a different name every time.
 char msgname[20];
 sprintf(msgname, "tic-%d", ++seq);
 seq=seq%256;
 cMessage *msg = new cMessage(msgname);
 return msg;
}
void Tic9::sendCopyOf(cMessage *msg)
{
 int counter = 0;
 // Duplicate message and send the copy.}
 cMessage *copy = (cMessage *)msg->dup();
 send(copy, "out");
}
void Tic9::sendCopyOf1(cMessage *msg)
{
 int counter = 0;
 lose = 5;
 // Duplicate message and send the copy.}
 cMessage *copy = (cMessage *)msg->dup();
 send(copy, "out");
}
/**
* Sends back an acknowledgement -- or not.
*/
class Toc9 : public cSimpleModule
{
public:
 virtual void handleMessage(cMessage *msg) override;
 };
Define_Module(Toc9);
void Toc9::handleMessage(cMessage *msg)
{
 char a[20];
 char ack_num[20];
 int window = Window_size;
 if (uniform(0, 1) < 0.06) {
 EV << "\"Losing\" message " << msg << endl;
 bubble("message lost");
 if(flag == 0){
 b[0]=count;
 }
 else if(flag == 8){
 b[0]=count1;
 flag=50;
 }
 flag++;
 delete msg;
 }
 else if(self == 1){
 sprintf(a,"Window_size");
 cMessage *msg1= new cMessage(a);
 send(msg1,"out");
 bubble("Window Size = 4");
 self=0;
 }
 else if(flag == 8){
 count1++;
 if(count1 > (Window_size - 1)){
 delete msg,
 count=0;
 flag=0;
 EV << msg << " lost, sending back an acknowledgement.\n";
 sprintf(ack_num,"ack-%d",seq);
 cMessage *msg2 = new cMessage(ack_num);
 send(msg2, "out");
 }
 }
 else {
 count += 1;
 if (count == Window_size)
 {
 if(flag == 0){
 EV << msg << " received, sending back an acknowledgement.\n";
 delete msg;
 //send(new cMessage("ack"), "out");
 sprintf(ack_num,"ack-%d",seq);
 cMessage *msg2 = new cMessage(ack_num);
 send(msg2, "out");
 count = 0;
 flag=0;
 }
 }
 }
}
