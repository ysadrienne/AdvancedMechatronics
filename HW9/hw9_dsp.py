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

x = 50
weight_a = 0.95 # higher for noisy data but takes longer
weight_b = 1 - weight_a

for key in data:
    t = data[key]['t']
    s = data[key]['s']
    Fs = data[key]['sr']

    new_data_list = []
    # MAF
    # for i in range(len(s)):
    #     if i < x:
    #         group = s[0 : i + 1]
    #         avg = sum(group) / x
    #     else:
    #         group = s[i - x + 1 : i + 1]
    #         avg = sum(group) / x

    #     new_data_list.append(avg)

    # LFP with IIR
    current_avg = s[0]
    new_data_list.append(current_avg)
    for i in range(1, len(s)):
        current_avg = weight_a * current_avg + weight_b * s[i]
        new_data_list.append(current_avg)

    # FFT on filtered signal
    Ts = 1.0/Fs; # sampling interval
    ts = np.arange(0,t[-1],Ts) # time vector

    y = new_data_list # the data to make the fft from
    n = len(y) # length of the signal
    k = np.arange(n)
    T = n/Fs
    frq = k/T # two sides frequency range
    frq = frq[range(int(n/2))] # one side frequency range
    Y = np.fft.fft(y)/n # fft computing and normalization
    Y = Y[range(int(n/2))]

    Y_unfiltered = np.fft.fft(s)/n
    Y_unfiltered = Y_unfiltered[range(int(n/2))]

    fig, (ax1, ax2) = plt.subplots(2, 1)
    # fig.suptitle(f'MAF FFT for {filename} with {x} Averaged Datapoints')
    # fig.suptitle(f'LPF with IIR FFT for {filename} with A: {weight_a:.2f}, B: {weight_b:.2f}')
    fig.suptitle(f'FIR FFT for {filename} with A: {weight_a:.2f}, B: {weight_b:.2f}')
    ax1.plot(t,s,'k',label='Original')
    ax1.plot(t,new_data_list,'r', label='Filtered')
    ax1.set_xlabel('Time (s)')
    ax1.set_ylabel('Amplitude')
    ax1.legend()

    ax2.loglog(frq,abs(Y_unfiltered),'k') # plotting the fft
    ax2.loglog(frq,abs(Y),'r') # plotting the fft
    ax2.set_xlabel('Freq (Hz)')
    ax2.set_ylabel('|Y(freq)|')

    # plt.savefig(f'MAF_{key}.png')
    # plt.savefig(f'LPF_{key}.png')
    plt.savefig(f'FIR_{key}.png')

plt.show()

# for i in range(len(at)):
#     # print the data to verify it was read
#     print(str(at[i]) + ", " + str(a1[i]))

# plt.plot(data['sigA']['t'], data['sigA']['s'], 'b-*')
# plt.xlabel('Time [s]')
# plt.ylabel('Signal')
# plt.title('Signal vs Time')
# plt.show()