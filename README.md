collectl info receiver GUI-Version

1.Install collectl
sudo apt-get install collectl

2.Make collectl info receiver GUI-Version
cd <collectl info receiver(GUI-Version) folder>
make clean
make

3.Start collectl info receiver GUI-Version
./main_release

4.run collectl in the system you want to monitor, for example:
collectl -A <ip you just got>:4000 -scm -P
don't forget -P option because we use plot format to commuicate.

5.this program can support 4 computers, for example:
computer1:
collectl -A <ip you just got>:4000 -scm -P
computer2:
collectl -A <ip you just got>:4001 -scm -P
computer3:
collectl -A <ip you just got>:4002 -scm -P
computer4:
collectl -A <ip you just got>:4003 -scm -P

6.you can use keyboard to control this program
'w' : select next diagram
's' : select last diagram
'd' : display data of next column in selected diagram 
'a' : display data of last column in selected diagram
'q' : decrease y-axis maxvalue by 10 times in selected diagram 
'e' : increase y-axis maxvalue by 10 times in selected diagram 
