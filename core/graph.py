"""
AES

--- /8A2D9516A147FACA949A13CAC24A640A/20E0F0FF692EB2FFAC92166F91169935/6E35F4F5C129F67D235EC56BB29780A9 ping statistics ---
1000 packets transmitted, 1000 received, 0 nacked, 0% lost, 0% nacked, time 3652.22 ms
rtt min/avg/max/mdev = 0.869136/3.65222/9.08464/1.00157 ms

================================================================================================================================================================================

FPE


--- /4bpdy3qcw/iyusi/d6abiy22845el ping statistics ---
1000 packets transmitted, 1000 received, 0 nacked, 0% lost, 0% nacked, time 3613.59 ms
rtt min/avg/max/mdev = 1.00437/3.61359/7.93143/0.915741 ms

================================================================================================================================================================================

None

--- /sch.ac.kr/calab/research.file ping statistics ---
1000 packets transmitted, 1000 received, 0 nacked, 0% lost, 0% nacked, time 3658.37 ms
rtt min/avg/max/mdev = 1.05942/3.65837/7.4594/0.997677 ms

"""

import matplotlib.pyplot as plt
import numpy as np


def plot_aes(aes_data):
    result = []
    total = 0
    cnt = 0
    for i in aes_data:
        cnt += 1
        total += i
        if total >= 10:
            result.append(cnt)
            cnt = 0
            total = 0

    # zero_values = [0 for _ in range(302)]
    plt.title("AES Type")
    plt.xlabel('Time (ms)')
    plt.ylabel('Packets')
    plt.plot(result, linewidth="2.5", label="Received")
    # plt.plot(zero_values, linewidth="2.5", label="Nacked")
    plt.xticks(np.arange(0, 301, 60), labels=[0, 800, 1600, 2400, 3200, 4000])
    plt.yticks()
    for i in range(0, 7, 1):
        plt.axhline(i, 0, 1, color='lightgrey', linestyle='--')
    for i in range(0, 301, 60):
        plt.axvline(i, 0, 1, color='lightgrey', linestyle='--')

    plt.show()


def plot_fpe(fpe_data):
    result = []
    total = 0
    cnt = 0
    for i in fpe_data:
        cnt += 1
        total += i
        if total >= 10:
            result.append(cnt)
            cnt = 0
            total = 0

    # zero_values = [0 for _ in range(302)]
    plt.title("FPE Type")
    plt.xlabel('Time (ms)')
    plt.ylabel('Packets')
    plt.plot(result, linewidth="2.5", label="Received")
    # plt.plot(zero_values, linewidth="2.5", label="Nacked")
    plt.xticks(np.arange(0, 301, 60), labels=[0, 800, 1600, 2400, 3200, 4000])
    plt.yticks()
    for i in range(0, 7, 1):
        plt.axhline(i, 0, 1, color='lightgrey', linestyle='--')
    for i in range(0, 301, 60):
        plt.axvline(i, 0, 1, color='lightgrey', linestyle='--')

    plt.show()


def plot_none(none_data):
    result = []
    total = 0
    cnt = 0
    for i in none_data:
        cnt += 1
        total += i
        if total >= 10:
            result.append(cnt)
            cnt = 0
            total = 0

    # zero_values = [0 for _ in range(302)]
    plt.title("None Type")
    plt.xlabel('Time (ms)')
    plt.ylabel('Packets')
    plt.plot(result, linewidth="2.5", label="Received")
    # plt.plot(zero_values, linewidth="2.5", label="Nacked")
    plt.xticks(np.arange(0, 301, 60), labels=[0, 800, 1600, 2400, 3200, 4000])
    plt.yticks()
    for i in range(0, 10, 1):
        plt.axhline(i, 0, 1, color='lightgrey', linestyle='--')
    for i in range(0, 301, 60):
        plt.axvline(i, 0, 1, color='lightgrey', linestyle='--')

    plt.show()


if __name__ == '__main__':

    with open("backup/consumer/aes-result.txt") as aes_f:
        aes_data = aes_f.readlines()

    with open("backup/consumer/fpe-result.txt") as fpe_f:
        fpe_data = fpe_f.readlines()

    with open("backup/consumer/none-result.txt") as none_f:
        none_data = none_f.readlines()

    # AES
    count = 1
    for i in range(len(aes_data)):
        aes_data[i] = aes_data[i].replace("\n", "")
        aes_data[i] = aes_data[i].replace(f"content from /8A2D9516A147FACA949A13CAC24A640A/20E0F0FF692EB2FFAC92166F91169935/6E35F4F5C129F67D235EC56BB29780A9: seq={count} time=",
                                          "")
        aes_data[i] = aes_data[i].replace(" ms", "")
        aes_data[i] = float(aes_data[i])
        count = count + 1

    # FPE
    count = 1
    for i in range(len(fpe_data)):
        fpe_data[i] = fpe_data[i].replace("\n", "")
        fpe_data[i] = fpe_data[i].replace(f"content from /4bpdy3qcw/iyusi/d6abiy22845el: seq={count} time=", "")
        fpe_data[i] = fpe_data[i].replace(" ms", "")
        fpe_data[i] = float(fpe_data[i])
        count = count + 1

    # None
    count = 1
    for i in range(len(none_data)):
        none_data[i] = none_data[i].replace("\n", "")
        none_data[i] = none_data[i].replace(f"content from /sch.ac.kr/calab/research.file: seq={count} time=", "")
        none_data[i] = none_data[i].replace(" ms", "")
        none_data[i] = float(none_data[i])
        count = count + 1

    plot_none(none_data)
    plot_fpe(fpe_data)
    plot_aes(aes_data)
