from random import random, randint
from copy import copy
import time
from tqdm import tqdm
class Individual():
    def __init__(self, raw_sequence): # TESTED
        self.__generation = 0
        self.__fitness = 0
        self.__maxOffset = 10
        
        # initialize chromosome including a random offset
        self.chromosome = []
        self.alignLength = 0
        for seq in raw_sequence:
            offset = randint(0, self.__maxOffset)
            seq = '-'*offset + seq
            self.chromosome.append(seq)

            if len(seq) > self.alignLength:
                self.alignLength = len(seq) 
        
        for i, seq in enumerate(self.chromosome):
            if len(seq) == self.alignLength:
                continue
            bias = self.alignLength - len(seq)
            self.chromosome[i] = seq + bias*'-'
        # Debug
        if len(self.chromosome[0])!=len(self.chromosome[1]):
            print('ERROR')
    def setGeneration(self, generation:int): # TESTED
        self.__generation = generation
    def computeFitness(self): # TESTED
        '''compute sum of pairwise cost by Dynamic Programming '''
        self.__fitness = 0
        num_seq = len(self.chromosome)
        for seq1_index in range(num_seq):
            for seq2_index in range(seq1_index+1, num_seq):
                seq_1 = self.chromosome[seq1_index]
                seq_2 = self.chromosome[seq2_index]
                DP_table = [[0 for j in range(len(seq_2) + 1)] for i in range(len(seq_1) + 1)] # create a table for DP
                for i in range(0,len(seq_1)+1):
                    for j in range(0, len(seq_2)+1):
                        if i==0 and j==0:
                            continue
                        elif i==0:
                            DP_table[i][j]=DP_table[i][j-1]+2
                        elif j==0:
                            DP_table[i][j]=DP_table[i-1][j]+2
                        else:
                            if seq_1[i-1]==seq_2[j-1]:
                                DP_table[i][j]=min(DP_table[i-1][j-1], DP_table[i-1][j]+2, DP_table[i][j-1]+2) # x_i and y_j match
                            else:
                                DP_table[i][j]=min(DP_table[i-1][j-1]+3, DP_table[i-1][j]+2, DP_table[i][j-1]+2) # x_i and y_j mismatch
                self.__fitness += DP_table[len(seq_1)][len(seq_2)]

    def getGeneration(self):
        return self.__generation
    def getFitness(self):
        return self.__fitness

class Genetic():
    def __init__(self, population_size:int, mutation_rate, crossover_rate, max_iteration, raw_sequence):
        self.population = [] # A list of individuals
        self.population_size = population_size
        self.mutation_rate = mutation_rate
        self.raw_sequence = raw_sequence # ['12','123,'3213']
        self.generation = 0 # The newest generation
        self.crossover_rate = crossover_rate
        self.max_iteration = max_iteration

        self.rand_gap_interval = (0, 5) # randomly insert some number of gaps (in `rand_gap_interval`) to each sequence of the alignment
    def init_population(self): # TESTED
        for _ in range(self.population_size):
            self.population.append(Individual(self.raw_sequence))
    def evaluation(self):
        for individual in self.population:
            if individual.getGeneration() < self.generation:
                continue
            individual.computeFitness()
    def selection(self):
        i = 0
        new_population = []
        sum_fitness = 0
        for individual in self.population:
            sum_fitness += individual.getFitness()

        while(len(new_population)<self.population_size):
            current_individual = self.population[i]
            probability = current_individual.getFitness() / sum_fitness
            if(is_choosen(probability)):
                new_population.append(current_individual)
            i = (i+1) // self.population_size
        
    def cross_over(self):
        for i in range(self.population_size):
            for j in range(i+1, self.population_size):
                if(random()>self.crossover_rate): # no crossover
                    continue
                self.one_point_crossover(i, j)
                
    def one_point_crossover(self, index_1:int, index_2:int):
        individual_1 = self.population[index_1]
        individual_2 = self.population[index_2]
        newIndividual = copy(individual_1)

        max_right_seq = 0
        left_block = []
        right_block = []
        position_1 = randint(0, individual_1.alignLength-1)
        for i, seq_1 in enumerate(individual_1.chromosome):
            left_seq = seq_1[0:position_1+1]
            left_len = len(left_seq.replace('-',''))
            left_block.append(left_seq)

            right_index = 0
            seq_2 = individual_2.chromosome[i]
            for j, item in enumerate(seq_2):
                if item!='-':
                    right_index += 1
                if right_index == left_len:
                    right_index = j + 1
                    break
            right_block.append(seq_2[right_index:])
            if(len(seq_2[right_index:])>max_right_seq):
                max_right_seq = len(seq_2[right_index:])

        # add - in right block
        for i, seq in enumerate(right_block):
            bias = max_right_seq - len(seq)
            right_block[i] = bias*'-' + seq

        for i, seq in enumerate(newIndividual.chromosome):
            newIndividual.chromosome[i] = left_block[i]+right_block[i]
        newIndividual.computeFitness()
        self.population.append(newIndividual)

    def gap_insertion(self):
        num_gap = randint(self.rand_gap_interval[0], self.rand_gap_interval[1])
        for i, individual in enumerate(self.population):
            if(random()>self.mutation_rate): # no mutation
                    continue
            for j, seq in enumerate(individual.chromosome):
                for k in range(0, num_gap):
                    pos = randint(0, len(seq)-1)
                    self.population[i].chromosome[j] = seq[0:pos]+'-'+seq[pos:]

    def result(self):
        ordered_population = sorted(self.population, key=lambda individual: individual.getFitness(), reverse=True)
        return ordered_population[0].getFitness()

    def Iteration(self):

        self.init_population()
        for _ in tqdm(range(self.max_iteration)):
            
            self.evaluation()

            self.selection()

            self.cross_over()

            self.gap_insertion()

        return self.result()
            
def is_choosen(prob):
    p = random()
    return 0 if p<prob else 1
    
if __name__ == '__main__':

    mutation_rate = 0.4
    population_size = 10
    max_iteration = 15
    crossover_rate = 0.5

    two_seq_query = ['KJXXJAJKPXKJJXJKPXKJXXJAJKPXKJJXJKPXKJXXJAJKPXKJXXJAJKHXKJXXJAJKPXKJXXJAJKHXKJXX',
                    'ILOTGJJLABWTSTGGONXJMUTUXSJHKWJHCTOQHWGAGIWLZHWPKZULJTZWAKBWHXMIKLZJGLXBPAHOHVOLZWOSJJLPO',
                    'IHKKKRKKKKKKXGWGKKKPKSKKKKKBKKKPKHKKXKKBSKKPKWKKLKSKRKKWXKPKKBKKKPKTSKHKKKKLADKKYPKKKOPHKKBWWLPPWKK',
                    'MPPPJPXPGPJPPPXPPPJPJPPPXPPPPSPPJJJPPXXPPPPPJPPPXPPXIPJMMMXPKPSVGULMHHZPAWHTHKAAHHUPAONAPJSWPPJGA',
                    'IPPVKBKXWXKHSAPHVXXVOJMRAKKPJVLLJBWKOLLJKXHGXLLCPAJOBKPGXBATGXMPOMCVZTAXVPAGKXGOMJQOLJGWGKXLQ']
    three_seq_query = ['IPZJJLMLTKJULOSTKTJOGLKJOBLTXGKTPLUWWKOMOYJBGALJUKLGLOSVHWBPGWSLUKOBSOPLOOKUKSARPPJ',
                    'IWTJBGTJGJTWGBJTPKHAXHAGJJSJJPPJAPJHJHJHJHJHJHJHJHJPKSTJJUWXHGPHGALKLPJTPJPGVXPLBJHHJPKWPPDJSG']
    
    result_file_path = './genetic_result.txt'
    result_file = open(result_file_path, 'w')
    
    data_base = []
    MSA_databse_file = open('MSA_database.txt')
    for line in MSA_databse_file:
        data_base.append(line.strip('\n'))


    print('==========Pair-wise alignment task==========')
    print('==========Pair-wise alignment task==========', file=result_file)

    for index_query, seq_query in enumerate(two_seq_query):
        print(f'=====searching sequence {index_query}======')
        print(f'=====searching sequence {index_query}======', file=result_file)
        start_time = time.time()

        index_result, result = 0, float('inf')

        for index_key, seq_key in enumerate(data_base):
            print(f'=====sequence {index_key} in database')
            genetic = Genetic(population_size, mutation_rate, crossover_rate, max_iteration, raw_sequence=[seq_query,seq_key])
            temp_result = genetic.Iteration()

            if(temp_result<result):
                result = temp_result
                index_result =index_key
            
        
        print(f'cost: {result} ', f'index: {index_result}')
        print(f'cost: {result} ', f'index: {index_result}', file=result_file)
        print('target sequence: '+ data_base[index_result])
        print('target sequence: '+ data_base[index_result], file=result_file)
        end_time = time.time()
        print(f'Processing time: {end_time-start_time} s')
        print(f'Processing time: {end_time-start_time} s', file=result_file)

    print('\n\n\n==========Three-sequence alignment task==========')
    print('\n\n\n==========Three-sequence alignment task==========', file=result_file)
    for index_query, seq_query in enumerate(three_seq_query):
        print(f'\n=====searching query pair {index_query}======')
        print(f'\n=====searching query pair {index_query}======', file=result_file)
        start_time = time.time()

        index_result_1, index_result_2, result = 0, 0, float('inf')

        visited = [] # tuple of indices

        with tqdm(total=len(data_base)**2) as pbar: # 进度条装饰
            for index_key_1, seq_key_1 in enumerate(data_base):
                for index_key_2, seq_key_2 in enumerate(data_base):

                    print(f'=====sequence {index_key_1, index_key_2} in database')
                    genetic = Genetic(population_size, mutation_rate, crossover_rate, max_iteration, raw_sequence=[seq_query, seq_key_1, seq_key_2])
                    temp_result = genetic.Iteration()

                    if(temp_result<result):
                        result = temp_result
                        index_result =index_key
                    pbar.update(1)

        print(f'cost: {result}')
        print(f'cost: {result}', file = result_file)
        
        print(f'index: {index_result_1}')
        print(f'index: {index_result_1}', file=result_file)

        print('target sequence 1: '+data_base[index_result_1])
        print('target sequence 1: '+data_base[index_result_1], file=result_file)

        print(f'index: {index_result_2}')
        print(f'index: {index_result_2}', file=result_file)

        print('target sequence 2: '+data_base[index_result_2])
        print('target sequence 2: '+data_base[index_result_2], file=result_file)

        end_time = time.time()
        print(f'Processing time: {end_time-start_time} s')
        print(f'Processing time: {end_time-start_time} s', file=result_file)



    

    

    # for item in genetic_approach.population:
    #     print(item.chromosome)

