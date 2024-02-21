import matplotlib.pyplot as plt
import seaborn as sns 
import subprocess
import pandas as pd
import numpy as np

n_array = np.array([1,2,3,4]) #array containing the number of processors to run on. 
n_trials = 50 #number of trials to run for each processor count. 

subprocess.run(["mpicxx","par_pi_calc.cpp","-o","par_pi.exe"]) #compile the code

runtime_array = np.array([]);

for n in n_array:
     #Compile the code 
    print(n)
    pi_temp = np.array([]);
    time_temp = np.array([]);
    
    print("Running with",n,"processor(s)")
    
    for tr in range(n_trials):
        # Run a command and capture its output
        result = subprocess.run(["mpiexec","-n",str(n) ,"par_pi.exe"], stdout=subprocess.PIPE, stderr=subprocess.PIPE)

        # Decode the output to text
        stdout_text = result.stdout.decode('utf-8')
        stderr_text = result.stderr.decode('utf-8') #error text for debugging
        
        # print(stdout_text)
        # print(stderr_text)
        
        # avg_pi = float(stdout_text[36:44])

        
        search_text = "Time elapsed"

        index = stdout_text.find(search_text)

        avg_time = float(stdout_text[index+12:])
        
        
        
        #store the pi and runtime value for each trial in temporary arrays
        time_temp = np.append(time_temp,avg_time)
        # pi_temp = np.append(pi_temp,avg_pi) #not using this for question 2, but is there if we need it.
    print("Average runtime for ",n,"processors = ",np.mean(time_temp))
    
    runtime_array = np.append(runtime_array,np.mean(time_temp))
    
print('runtime array',runtime_array)
print('n array',n_array)

plt.figure()
plt.plot(n_array,runtime_array,'--r', marker='o')
plt.xlabel("Number of Processors")
plt.ylabel("Runtime (s)")
plt.xticks(np.arange(1,5,1))
plt.show()
plt.savefig("part4_p2.png")  
