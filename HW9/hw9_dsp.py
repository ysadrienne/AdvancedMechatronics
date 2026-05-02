import csv
import matplotlib.pyplot as plt
import numpy as np

files = ['sigA.csv', 'sigB.csv', 'sigC.csv', 'sigD.csv']

data = {}

for filename in files:
    t = []
    s = []
    with open(filename) as f:
        # open the csv file
        reader = csv.reader(f)
        for row in reader:
            # read the rows 1 one by one
            t.append(float(row[0])) # leftmost column
            s.append(float(row[1])) # second column
    
    key = filename.replace('.csv', '')
    sample_rate = len(t) / t[-1]
    data[key] = {'t': t, 's': s, 'sr': sample_rate}

# for i in range(len(at)):
#     # print the data to verify it was read
#     print(str(at[i]) + ", " + str(a1[i]))

# plt.plot(data['sigA']['t'], data['sigA']['s'], 'b-*')
# plt.xlabel('Time [s]')
# plt.ylabel('Signal')
# plt.title('Signal vs Time')
# plt.show()

for key in data:
    t = data[key]['t']
    s = data[key]['s']
    Fs = data[key]['sr']

    Ts = 1.0/Fs; # sampling interval
    ts = np.arange(0,t[-1],Ts) # time vector
    y = s # the data to make the fft from
    n = len(y) # length of the signal
    k = np.arange(n)
    T = n/Fs
    frq = k/T # two sides frequency range
    frq = frq[range(int(n/2))] # one side frequency range
    Y = np.fft.fft(y)/n # fft computing and normalization
    Y = Y[range(int(n/2))]

    fig, (ax1, ax2) = plt.subplots(2, 1)
    fig.suptitle(f'FFT for {filename}')
    ax1.plot(t,y,'b')
    ax1.set_xlabel('Time (s)')
    ax1.set_ylabel('Amplitude')
    ax2.loglog(frq,abs(Y),'b') # plotting the fft
    ax2.set_xlabel('Freq (Hz)')
    ax2.set_ylabel('|Y(freq)|')

plt.show()