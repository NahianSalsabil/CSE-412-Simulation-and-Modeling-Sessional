import sys
import random
import numpy as np
import matplotlib.pyplot as plt


if __name__ == '__main__':
    n = int(sys.argv[1])
    select = [1, 3, 5, 10]

    for s in select:
        success_rate_list = []
        for m in range(n):
            success_count = 0
            for iteration in range(10000):
                # generate n random uniform numbers between (0,1)
                n_list = [random.uniform(0,1) for i in range(n)]
                
                m_index = random.sample(list(range(n)), m)
                m_list = [n_list[i] for i in m_index]
                    
                criteria = 0
                if m_list != []:
                    criteria = np.max(m_list)
                
                # Interview
                selected_index = -1
                for i in range(n):
                    if i not in m_index and n_list[i] > criteria:
                        selected_index = i
                        break
                    
                # if no one is selected, then select the one with the last index
                if selected_index == -1:
                    selected_index = n - 1
                    
                # Check Success
                sorted_n_list = np.argsort(n_list)
                sorted_n = np.array(n_list)[sorted_n_list]
                top_scorers = sorted_n[-s : ]
                
                if n_list[selected_index] in top_scorers:
                    success_count += 1
                 
            success_rate = success_count / (iteration + 1)
            success_rate_list.append(success_rate * 100)
        
        plt.figure(figsize=(6, 6))
        plt.title('Success Rate vs. m\ns = ' + str(s) + ', n = ' + str(n) + ', 10000 iterations')
        plt.plot(range(n), success_rate_list)
        plt.xlabel('m')
        plt.ylabel('Success Rate')
        plt.show()