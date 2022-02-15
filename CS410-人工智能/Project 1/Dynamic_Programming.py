from tqdm import tqdm
import time

result_file_path = './result.txt'
result_file = open(result_file_path, 'w')

class DynamicProgramming():
    def __init__(self, two_seq_query, three_seq_query):

        self.two_seq_query = two_seq_query
        self.three_seq_query = three_seq_query
        
        self.data_base = []
        
        MSA_databse_file = open('MSA_database.txt')

        for line in MSA_databse_file:
            self.data_base.append(line.strip('\n'))
    def pairwiseTask(self):
        print('==========Pair-wise alignment task==========')
        print('==========Pair-wise alignment task==========', file=result_file)

        for index_query, seq_query in enumerate(self.two_seq_query):
            print(f'=====searching sequence {index_query}======')
            print(f'=====searching sequence {index_query}======', file=result_file)
            start_time = time.time()

            index_result, result = 0, float('inf')

            with tqdm(total=len(self.data_base)) as pbar: # 进度条装饰
                for index_key, seq_key in enumerate(self.data_base):
                    current_result = self.__pairwiseAlign(seq_query, seq_key)
                    if(current_result<result):
                        result, index_result = current_result, index_key
                    pbar.update(1)
            
            print(f'cost: {result} ', f'index: {index_result}')
            print(f'cost: {result} ', f'index: {index_result}', file=result_file)
            print('target sequence: '+self.data_base[index_result])
            print('target sequence: '+self.data_base[index_result], file=result_file)
            end_time = time.time()
            print(f'Processing time: {end_time-start_time} s')
            print(f'Processing time: {end_time-start_time} s', file=result_file)

    def __pairwiseAlign(self, x: str, y: str):
        
        def forward(DP_table, i, j):
            if i==0 and j==0:
                return
            elif i==0:
                DP_table[i][j]=DP_table[i][j-1]+2
            elif j==0:
                DP_table[i][j]=DP_table[i-1][j]+2
            else:
                if x[i-1]==y[j-1]:
                    DP_table[i][j]=min(DP_table[i-1][j-1], min(DP_table[i-1][j]+2, DP_table[i][j-1]+2)) # x_i and y_j match
                else:
                    DP_table[i][j]=min(DP_table[i-1][j-1]+3, min(DP_table[i-1][j]+2, DP_table[i][j-1]+2)) # x_i and y_j mismatch

        row = len(x)
        col = len(y)

        Cost = [[0 for j in range(col + 1)] for i in range(row + 1)] # create a table for DP

        for i in range(0,row+1):
            for j in range(0, col+1):
                forward(Cost, i, j)
        return Cost[row][col]
        '''Baseline只考虑比较,暂时不考虑打印optimal solution'''
        # path = trace_back(DP_table)

        # for line in DP_table:
        #     print(line)

        # str1, str2 = str(), str()
        # for match_condition in path[-1]:
        #     str1+=match_condition[0]
        #     str2+=match_condition[1]
        # print(str1)
        # print(str2)
        # print(DP_table[row][col])

    def triSeqTask(self):
        print('\n\n\n==========Three-sequence alignment task==========')
        print('\n\n\n==========Three-sequence alignment task==========', file=result_file)
        for index_query, seq_query in enumerate(self.three_seq_query):
            print(f'\n=====searching query pair {index_query}======')
            print(f'\n=====searching query pair {index_query}======', file=result_file)
            start_time = time.time()

            index_result_1, index_result_2, result = 0, 0, float('inf')

            visited = [] # tuple of indices

            with tqdm(total=len(self.data_base)**2) as pbar: # 进度条装饰
                for index_key_1, seq_key_1 in enumerate(self.data_base):
                    for index_key_2, seq_key_2 in enumerate(self.data_base):
                        if index_key_1==index_key_2:
                            pbar.update(1)
                            continue
                        # print(f"processing sequence{index_key_1} and {index_key_2}")
                        if (index_key_1, index_key_2) in visited or (index_key_2, index_key_1) in visited:
                            pbar.update(1)
                            continue
            
                        current_result = self.__triSeqAlign(seq_query, seq_key_1, seq_key_2)
                        if(current_result<result):
                            result, index_result_1, index_result_2 = current_result, index_key_1, index_key_2
                        
                        visited.append((index_key_1, index_key_2))
                        pbar.update(1)

            print(f'cost: {result}')
            print(f'cost: {result}', file = result_file)
            
            print(f'index: {index_result_1}')
            print(f'index: {index_result_1}', file=result_file)

            print('target sequence 1: '+self.data_base[index_result_1])
            print('target sequence 1: '+self.data_base[index_result_1], file=result_file)

            print(f'index: {index_result_2}')
            print(f'index: {index_result_2}', file=result_file)

            print('target sequence 2: '+self.data_base[index_result_2])
            print('target sequence 2: '+self.data_base[index_result_2], file=result_file)

            end_time = time.time()
            print(f'Processing time: {end_time-start_time} s')
            print(f'Processing time: {end_time-start_time} s', file=result_file)

    def __triSeqAlign(self,x: str, y: str, z: str):
        def forward(DP_table, i, j, k):
            if i==0 and j==0 and k==0:
                return
            elif (i==0 and j==0):
                DP_table[i][j][k] = DP_table[i][j][k-1] + 4 
            elif (i==0 and k==0):
                DP_table[i][j][k] = DP_table[i][j-1][k] + 4
            elif (j==0 and k==0):
                DP_table[i][j][k] = DP_table[i-1][j][k] + 4
            elif i==0:
                DP_table[i][j][k] = min(DP_table[i][j-1][k-1] + (4 if y[j-1]==z[k-1] else 7),
                                            min(DP_table[i][j][k-1] + 4, DP_table[i][j-1][k] + 4))
            elif j==0:
                DP_table[i][j][k] = min(DP_table[i-1][j][k-1] + (4 if x[i-1]==z[k-1] else 7),
                                            DP_table[i][j][k-1] + 4, DP_table[i-1][j][k] + 4)
            elif k==0:
                DP_table[i][j][k] = min(DP_table[i-1][j-1][k] + (4 if x[i-1]==y[j-1] else 7),
                                            DP_table[i-1][j][k] + 4, DP_table[i][j-1][k] + 4)
            else:
                if x[i-1]==y[j-1]==z[k-1]:
                    min_value = [ DP_table[i-1][j-1][k-1],
                            DP_table[i-1][j-1][k] + 4, DP_table[i-1][j][k-1] + 4, DP_table[i][j-1][k-1] + 4,
                            DP_table[i][j][k-1] + 4, DP_table[i][j-1][k] + 4, DP_table[i-1][j][k] + 4 ]
                    min_value.sort()
                    DP_table[i][j][k] = min_value[0]
                elif x[i-1]==y[j-1] and x[i-1]!=z[k-1]:
                    min_value = [ DP_table[i-1][j-1][k-1] + 6,
                            DP_table[i-1][j-1][k] + 4, DP_table[i-1][j][k-1] + 7, DP_table[i][j-1][k-1] + 7,
                            DP_table[i][j][k-1] + 4, DP_table[i][j-1][k] + 4, DP_table[i-1][j][k] + 4 ]
                    min_value.sort()
                    DP_table[i][j][k] = min_value[0]
                elif x[i-1]==z[k-1] and x[i-1]!=y[j-1]:
                    min_value = [ DP_table[i-1][j-1][k-1] + 6,
                            DP_table[i-1][j-1][k] + 7, DP_table[i-1][j][k-1] + 4, DP_table[i][j-1][k-1] + 7,
                            DP_table[i][j][k-1] + 4, DP_table[i][j-1][k] + 4, DP_table[i-1][j][k] + 4 ]
                    min_value.sort()
                    DP_table[i][j][k] = min_value[0]
                elif y[j-1]==z[k-1] and x[i-1]!=y[j-1]:
                    min_value = [ DP_table[i-1][j-1][k-1] + 6,
                            DP_table[i-1][j-1][k] + 7, DP_table[i-1][j][k-1] + 7, DP_table[i][j-1][k-1] + 4,
                            DP_table[i][j][k-1] + 4, DP_table[i][j-1][k] + 4, DP_table[i-1][j][k] + 4 ]
                    min_value.sort()
                    DP_table[i][j][k] = min_value[0]
                else:
                    min_value = [ DP_table[i-1][j-1][k-1] + 9,
                            DP_table[i-1][j-1][k] + 7, DP_table[i-1][j][k-1] + 7, DP_table[i][j-1][k-1] + 7,
                            DP_table[i][j][k-1] + 4, DP_table[i][j-1][k] + 4, DP_table[i-1][j][k] + 4 ]
                    min_value.sort()
                    DP_table[i][j][k] = min_value[0]

        axis_0, axis_1, axis_2 = len(x), len(y), len(z)

        DP_table =[[[0 for k in range(axis_2+1)] for j in range(axis_1 + 1)]  for i in range(axis_0 + 1)] # create a table for DP
        
        # with tqdm(total=(axis_0+1)*(axis_1+1)*(axis_2+1)) as pbar:
        for i in range(0,axis_0+1):
            for j in range(0, axis_1+1):
                for k in range(0, axis_2+1):
                    forward(DP_table, i, j, k)
                        # pbar.update(1)

        return DP_table[axis_0][axis_1][axis_2]
    def correctnessValidation(self):

        test_result_file = open('./DP_test_result.txt', 'w')
        test_databse_file = open('./Test_database.txt')
        test_query_file = open('./Test_query.txt')
        
        two_seq_query = []
        data_base = []
        
        for line in test_databse_file:
            data_base.append(line.strip('\n'))

        for line in test_query_file:
            two_seq_query.append(line.strip('\n'))

        for query in two_seq_query:
            for key in data_base:
                print(self.__pairwiseAlign(query, key), file=test_result_file)


    
    
        
if __name__ == '__main__':

    two_seq_query = ['KJXXJAJKPXKJJXJKPXKJXXJAJKPXKJJXJKPXKJXXJAJKPXKJXXJAJKHXKJXXJAJKPXKJXXJAJKHXKJXX',
                    'ILOTGJJLABWTSTGGONXJMUTUXSJHKWJHCTOQHWGAGIWLZHWPKZULJTZWAKBWHXMIKLZJGLXBPAHOHVOLZWOSJJLPO',
                    'IHKKKRKKKKKKXGWGKKKPKSKKKKKBKKKPKHKKXKKBSKKPKWKKLKSKRKKWXKPKKBKKKPKTSKHKKKKLADKKYPKKKOPHKKBWWLPPWKK',
                    'MPPPJPXPGPJPPPXPPPJPJPPPXPPPPSPPJJJPPXXPPPPPJPPPXPPXIPJMMMXPKPSVGULMHHZPAWHTHKAAHHUPAONAPJSWPPJGA',
                    'IPPVKBKXWXKHSAPHVXXVOJMRAKKPJVLLJBWKOLLJKXHGXLLCPAJOBKPGXBATGXMPOMCVZTAXVPAGKXGOMJQOLJGWGKXLQ']
    three_seq_query = ['IPZJJLMLTKJULOSTKTJOGLKJOBLTXGKTPLUWWKOMOYJBGALJUKLGLOSVHWBPGWSLUKOBSOPLOOKUKSARPPJ',
                    'IWTJBGTJGJTWGBJTPKHAXHAGJJSJJPPJAPJHJHJHJHJHJHJHJHJPKSTJJUWXHGPHGALKLPJTPJPGVXPLBJHHJPKWPPDJSG']


    DP_approach = DynamicProgramming(two_seq_query, three_seq_query)
    # DP_approach.correctnessValidation()
    DP_approach.pairwiseTask()
    DP_approach.triSeqTask()
    # DP_approach.test()
    
    

        

