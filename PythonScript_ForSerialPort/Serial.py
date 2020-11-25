import sys  
import glob
import serial
import time
import threading

###################################################
#List Serial Port
###################################################
def serial_ports():
    """ Lists serial port names

        :raises EnvironmentError:
            On unsupported or unknown platforms
        :returns:
            A list of the serial ports available on the system
    """
    if sys.platform.startswith('win'):
        ports = ['COM%s' % (i + 1) for i in range(256)]
    elif sys.platform.startswith('linux') or sys.platform.startswith('cygwin'):
        # this excludes your current terminal "/dev/tty"
        ports = glob.glob('/dev/tty[A-Za-z]*')
    elif sys.platform.startswith('darwin'):
        ports = glob.glob('/dev/tty.*')
    else:
        raise EnvironmentError('Unsupported platform')

    result = []
    for port in ports:
        try:
            s = serial.Serial(port)
            s.close()
            result.append(port)
        except (OSError, serial.SerialException):
            pass
    return result


if __name__ == '__main__':
    print(serial_ports())
###################################################
#Connect to port
###################################################
#name = input('RSI Port：')
Host_COM_PORT = input('\n\nType in host com port：')
RSI_COM_PORT = input('\n\nType in RSI  com port：')
#Host_COM_PORT ='COM20'
#RSI_COM_PORT = 'COM39'

BAUD_RATES = 115200
Host_COM = serial.Serial(Host_COM_PORT, BAUD_RATES)
print('\nHost port connect successful')

RSI_COM = serial.Serial(RSI_COM_PORT, BAUD_RATES)   
print('\nRSI port connect successful')

print('\nStart Data transform\n')
print('-----------------------')
########################################################
# Create  for Host_COM 
########################################################

def Host_handle_data(Host_Send_byte): 
    print(Host_Send_byte.decode('ascii','replace'),end='')
    #print(Host_Send_byte.hex())

    RSI_COM.write(Host_Send_byte)
def read_from_port(Host_COM):
    connected = False
    while True:
        Host_Send_byte  = Host_COM.read()
        Host_handle_data(Host_Send_byte)
#Host_running = threading.Event()
#Host_running.set()        
Host_thread = threading.Thread(target=read_from_port, args=(Host_COM,))
Host_thread.start()
########################################################
# Create a thread  for RSI_COM 
########################################################
def RSI_handle_data(RSI_Send_byte): 
    print(RSI_Send_byte.decode('ascii','replace'),end='')
    #print(RSI_Send_byte.hex())

    Host_COM.write(RSI_Send_byte)
def read_from_port(Host_COM):
    connected = False
    while True:
        RSI_Send_byte  = RSI_COM.read()
        RSI_handle_data(RSI_Send_byte)
#RSI_running = threading.Event()
#RSI_running.set()
RSI_thread = threading.Thread(target=read_from_port, args=(RSI_COM,))
RSI_thread.start()
########################################################
try:
    while True:
        #print(time.ctime())
        time.sleep(5)
# type in Crtl +C Will Close the project
except KeyboardInterrupt:
#    print('Event running.clear()')
#    RSI_running.clear()
#    Host_running.clear()
    Host_COM.close()    # Close Serial Port
    RSI_COM.close()    # Close Serial Port

    print('-----------------------')
    print('host port close goodbye')
    sys.exit()
