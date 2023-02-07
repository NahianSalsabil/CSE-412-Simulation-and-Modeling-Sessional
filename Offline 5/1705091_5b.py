import numpy as np
import sys

prob_n = 4

if __name__ == '__main__':
    n_gen_list = [1, 2, 3]
    
    p = np.zeros((prob_n))
    p[0] = 0.4825
    for i in range(1, 4):
        p[i] = 0.2126 * pow(0.5893, i-1)
    
    diff = 1 - np.sum(p)
    p[0] += diff
    
    # cumulative probability
    cumulative_prob = np.zeros((4))
    cumulative_prob[0] = p[0]
    for i in range(1, prob_n):
        cumulative_prob[i] = cumulative_prob[i-1] + p[i]
    
    for n_gen in n_gen_list:
        count = np.zeros((5))
        for iteration in range(10000):
            next_gen_node_number = 1
            for i in range(n_gen):
                current_gen_node = next_gen_node_number
                next_gen_node_number = 0
                for j in range(current_gen_node):
                    prob = np.random.uniform(0, 1)
                    for j in range(prob_n):
                        if prob <= cumulative_prob[j]:
                            node_number = j
                            break
                    
                    next_gen_node_number += node_number
                    
                if next_gen_node_number == 0:
                    break
                
            if next_gen_node_number < 5:
                count[next_gen_node_number] += 1
        probability = np.zeros((5))
        probability = count / (iteration + 1)
        
        print("Number of generation: ", n_gen)
        for i in range(5):
            print("Probability of ", i, " nodes: ", probability[i])
        print("\n")
        
    
            
    
    
    