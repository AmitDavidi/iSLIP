import matplotlib.pyplot as plt
import numpy as np 

# xaxis = [0.9, 0.95, 0.99] # for runs 9-11, 12 - 14, 15 - 17
xaxis = [0.6, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 0.99] # for runs 1 - 8
from_log_file = 1
to_log_file = 8

mean_times = []
max_times = []
std_devs = []
average_times = []

for i in range(from_log_file, to_log_file + 1):
    file_name = f"out_script{i}.txt"
    time_stays = []
    with open(file_name, 'r') as file:
        for line in file:
            data = line.strip().split()
            time_sent = int(data[3])
            time_stay = int(data[3]) - int(data[0])
            time_stays.append(time_stay)
    

    average_times.append(np.average(time_stays))
    mean_times.append(np.mean(time_stays))
    max_times.append(np.max(time_stays))
    std_devs.append(np.std(time_stays))


# Plotting the graph
plt.plot(xaxis, average_times, marker='o', label='Average Time')
plt.plot(xaxis, mean_times, marker='o', label='Mean Time')
plt.plot(xaxis, max_times, marker='o', label='Max Time')
plt.plot(xaxis, std_devs, marker='o', label='Standard Deviation')
plt.xlabel('Packet arrival probability')
plt.ylabel('Time units')
plt.title('Packet Time Statistics')
plt.xticks(xaxis)
plt.legend()
plt.show()


# -- Handle buffer logs --

mean_sizes = []
max_sizes = []
std_devs = []

for i in range(from_log_file, to_log_file + 1):
    file_name = f"{i}.log.txt"
    sizes = []
    with open(file_name, 'r') as file:
        for line in file:
            data = line.strip().split()
            size = int(data[3])
            sizes.append(size)
    
    mean_size = np.mean(sizes)
    max_size = np.max(sizes)
    std_dev = np.std(sizes)
    
    mean_sizes.append(mean_size)
    max_sizes.append(max_size)
    std_devs.append(std_dev)

# Plotting the graph

# Plotting Mean, Max, and Std Dev
plt.plot(xaxis, mean_sizes, marker='o', label='Mean Size')
plt.plot(xaxis, max_sizes, marker='o', label='Max Size')
plt.plot(xaxis, std_devs, marker='o', label='Standard Deviation')

plt.xlabel('Packet arrival probability')
plt.ylabel('Queue Size')
plt.title('Queue Size Statistics')
plt.xticks(xaxis)
plt.legend()
plt.show()



