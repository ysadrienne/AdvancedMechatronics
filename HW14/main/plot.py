#!/usr/bin/env python3
import sys
import serial
import matplotlib.pyplot as plt
import numpy as np

if len(sys.argv) < 2:
    print("Usage: plot.py <serial_port> [baudrate]")
    sys.exit(1)

port = sys.argv[1]
baud = int(sys.argv[2]) if len(sys.argv) > 2 else 115200

NUM_LINES = 5 * 80 

def read_lines(ser, n):
    lines = []
    while len(lines) < n:
        line = ser.readline().decode('utf-8').strip()
        if line:
            lines.append(line)
    return lines


with serial.Serial(port, baud, timeout=10) as ser:
    ser.reset_input_buffer()
    ser.reset_output_buffer()
    print("Collection requested...")
    
    ser.write(f"{NUM_LINES}\n".encode('utf-8'))
    ser.flush()
    
    raw_lines = read_lines(ser, NUM_LINES)
    print(f"Collected {len(raw_lines)} lines")

times = []
filtered_values = []

for ln in raw_lines:
    parts = ln.split()
    if len(parts) < 3:
        continue
    try:
        t_sec = float(parts[1]) / 1000.0
        v_val = float(parts[2])
        
        times.append(t_sec)
        filtered_values.append(v_val)
    except ValueError:
        continue


raw_values = [filtered_values[0]]
for i in range(1, len(filtered_values)):
    raw_val = (filtered_values[i] - 0.9 * filtered_values[i-1]) / 0.1
    raw_values.append(raw_val)

sample_rate = len(times) / (times[-1] - times[0]) if len(times) > 1 else 80.0

data = {
    'hx711_sensor': {
        't': times,
        's': raw_values,
        'filtered': filtered_values,
        'sr': sample_rate
    }
}

for key in data:
    t = data[key]['t']
    s = data[key]['s']
    new_data_list = data[key]['filtered']
    Fs = data[key]['sr']

    Ts = 1.0 / Fs 
    n = len(new_data_list)
    k = np.arange(n)
    T = n / Fs
    frq = (k / T)[range(int(n / 2))]

    Y = np.fft.fft(new_data_list) / n 
    Y = Y[range(int(n / 2))]

    Y_unfiltered = np.fft.fft(s) / n
    Y_unfiltered = Y_unfiltered[range(int(n / 2))]

    fig, (ax1, ax2) = plt.subplots(2, 1)
    fig.suptitle(f'IIR LPF FFT for {key} (A: 0.90, B: 0.10)')
    
    ax1.plot(t, s, 'k', label='Original (Raw)')
    ax1.plot(t, new_data_list, 'r', label='Filtered')
    ax1.set_xlabel('Time (s)')
    ax1.set_ylabel('Amplitude')
    ax1.legend()

    ax2.loglog(frq, abs(Y_unfiltered), 'k', label='Original FFT') 
    ax2.loglog(frq, abs(Y), 'r', label='Filtered FFT') 
    ax2.set_xlabel('Freq (Hz)')
    ax2.set_ylabel('|Y(freq)|')
    ax2.legend()

    plt.savefig(f'LPF_{key}.png')

plt.show()