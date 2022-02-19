

Configuration:
  The configuration is a text file.
  Format:
  line 0: Type <Xbox, PSwhatever>
  line 1: Axis 0
  line 2+: Axis 1
  ....
  last line: last Axis

ser2net is used on the raspberry so the controller service
only needs to connect to the tcp socket on the raspberry pi.

/dev/ttyUSB0 is located on port 5050
