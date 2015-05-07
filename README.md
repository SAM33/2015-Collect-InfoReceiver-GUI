collectl info receiver GUI-Version<br/>
For windows users : https://github.com/SAM33/infoReceiver-guiversion-windows <br/>
Base on library: <br/>
Linux : freeglut3-dev<br/>
Mac   : xcode<br/>

1.Install collectl in the system you want to keep watch on <br/>
sudo apt-get install collectl  <br/>
2.Make collectl info receiver GUI-Version in The monitor's computer 
<br/>
cd <collectl info receiver(GUI-Version) folder>
<br/>
make clean
<br/>
make
<br/><br/>
3.Start collectl info receiver GUI-Version
<br/>
./main_release
<br/><br/>
4.run collectl in the system you want to keep watch on <br/>, 
for example:
<br/>
collectl -A [ip you just got]:4000 -scmn -P
<br/>
don't forget -P option because we use plot format to commuicate.
<br/><br/>
5.this program can support 4 computers, for example:
<br/>
computer1:
<br/>
collectl -A [ip you just got]:4000 -scmn -P
<br/>
computer2:
<br/>
collectl -A [ip you just got]:4001 -scmn -P
<br/>
computer3:
<br/>
collectl -A [ip you just got]:4002 -scmn -P
<br/>
computer4:
<br/>
collectl -A [ip you just got]:4003 -scmn -P
<br/><br/>
6.look at terminal to control this program
<br/><br/>

