# SMU CS106 Embedded Systems Lab 2

**Members**: 

* Bryan Lee

* Charlie Angriawan

* Emmanuel Oh

* Zhi An Yuen

  

#### Challenge 1. You don’t know our pain

A charity caring for homeless persons in Singapore is keen to adopt technology for their work. You
decide to help this charity deploy a monitoring system at a deserted place where a homeless person
sleeps. The system comprises several sensors that send data to a gateway. In a real system (but not
required for this challenge), the gateway sends data to the cloud, where it can be further processed.



#### Challenge 2. I don’t stop when I’m tired, I stop when I’m done

You can use up to 8 micro:bits for this challenge. Suppose the micro:bits are spaced equally along the
circumference of a circle. Develop an application that causes exactly 1 micro:bit to be lit when one
person inside the circle walks continuously, and causes all micro:bits to be unlit, when the person
stops walking.



#### How to Run

1. Install [`yotta`](http://docs.yottabuild.org/#installing) and `srecord` [[windows]](http://srecord.sourceforge.net/) [[ubuntu]](https://packages.ubuntu.com/bionic/srecord)

2. Run project:

Run `./flash.sh challenge{num} {module} {microbit_path}`

Example (Ubuntu):

```bash
$ ./flash.sh challenge2 walker /media/charlie/MICROBIT1
$ ./flash.sh challenge1 gateway /media/charlie/MICROBIT1
$ ./flash.sh challenge1 lightsensor /media/charlie/MICROBIT2
```
