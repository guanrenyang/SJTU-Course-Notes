import copy
from tqdm import tqdm
import time


result_file_path = './astar_result.txt'
result_file = open(result_file_path, 'a')


class Queue(object):
    def __init__(self):
        self.items = []

    def make_queue(self, elements):
        self.items = elements

    def is_empty(self):
        return len(self.items) == 0

    def remove_front(self):
        return self.items.pop(0)

    def queue_front(self, elements):
        self.items.insert(0, elements)

    def queue_back(self, elements):
        self.items.append(elements)

    def show(self):
        print('queue', [i.state_hash for i in self.items])

class AStar_Node(object):
    def __init__(self, state, parent, action, depth, path_cost):
        self.state = state # `state` is a tuple. For pairwise align, e.g. ('abc','ab-')
        self.parent = parent # `parent` is the last state in the search tree, e.g. ('ab','ab')
        self.action = action  # The `action` taken by the parent node to generate the current node
        self.depth = depth # `depth` is the depth of the current node in the search tree
        self.path_cost = path_cost
        self.evaluation_cost = 0

    def state_hash(self):  # 这个要改一下的，结点状态不同了
        '''Print the state of the node'''
        try:
            return ''.join([str(item) for row in self.state for item in row]) + str(self.action if self.action is not None else -1)
        except:
            return ''.join([str(item) for item in self.state]) + self.action

class GeneralSearch(object):
    def __init__(self, initial_state, expand_node, goal_test, x:str, y: str, z: str, path_cost=0):
        self.nodes = [AStar_Node(initial_state, None, None, 1, path_cost)]  # Root Node
        self.expand_node = expand_node  # Function that returns expansion of inputted node
        self.goal_test = goal_test
        self.hashes = {self.nodes[0].state_hash: self.nodes[0]}
        self.x = x
        self.y = y
        self.z = z
        self.queue = Queue()
        
        if z=='':
            self.queue.make_queue(self.expand_node(self.nodes[0], self.x, self.y))
        else:
            self.queue.make_queue(self.expand_node(self.nodes[0], self.x, self.y, self.z))
        
    def queuing_function(self, nodes):
        raise NotImplementedError

    @staticmethod
    def solution(node):
        order = []

        while node.parent is not None:
            order.append(node.action)
            node = node.parent

        order = order[::-1]  # Reverse order to go from root to end

        return order

    def find_path(self, steps=None, search_cost=0):
        i = 1

        while not self.queue.is_empty():
            
            # # 调试
            # print('Before pop: ', end=' ')
            # for item in self.queue.items:
            #     print(item.path_cost,end=' ')  
            # print('\n')

            node = self.queue.remove_front()
            # print (i, "-", node.path_cost, node.depth, ":", len(self.queue.items))
            # print "Checking " + node.state_hash
            

            # 调试
        
            # print(node.state, len(node.state[0].replace('-','')), len(node.state[1].replace('-','')))
            # print('Path_cost: ',node.path_cost)
            # print('Evaluation_cost: ', node.evaluation_cost)
            # print('total_cost:' ,node.path_cost+node.evaluation_cost)
            # print('After pop: ', end=' ')
            # for item in self.queue.items:
            #     print(item.path_cost,end=' ') 
            # print('\n')
            if self.z=='':
                result_flag, n = self.goal_test(node, self.x, self.y)
            else:
                result_flag, n = self.goal_test(node, self.x, self.y, self.z)
            
            if result_flag:
                return self.solution(n), n

            # 检查重复状态
            '''在搜索算法扩展节点的过程中，我们可能会扩展一些重复的节点，这些节点是已经在搜索树中出现过的
            如果我们继续扩展这些节点，可能会导致搜索算法进入循环状态。因此我们需要将这些已经出现过的节点
            剔除出待扩展列表。'''
            if self.z=='':
                expanded_nodes = [new_node for new_node in self.expand_node(node, self.x, self.y) if new_node.state!=node.state]
            else:
                expanded_nodes = [new_node for new_node in self.expand_node(node, self.x, self.y, self.z) if new_node.state!=node.state]

            for new_node in expanded_nodes:
                new_node.path_cost += search_cost  # Add search cost
                self.hashes[new_node.state_hash] = new_node

            self.queuing_function(expanded_nodes)
            
            # # 调试
            # print('After expand: ', end=' ')
            # for item in self.queue.items:
            #     print(item.path_cost,end=' ')  
            # print('\n')
            

            if steps is not None and i >= steps:
                return None, None

            i += 1

        return False, None
class AStarSearch(GeneralSearch):
    def __init__(self, initial_state, expand_node, goal_test, heuristic, x: str, y: str, z=''):
        GeneralSearch.__init__(self, initial_state, expand_node, goal_test, x, y, z)
        self.heuristic = heuristic
        
        self.queue.items.sort(key=lambda x: (x.path_cost + x.evaluation_cost))

        # # DEBUG : print elements in queue
        # for item in self.queue.items:
        #     print(item.state)
        # input()

    def queuing_function(self, nodes):
        # Store evaluation cost to massively improve sort performance
        if self.z=='':
            for node in nodes:
                node.evaluation_cost = self.heuristic(node.state, self.x, self.y)
        else:
            for node in nodes:
                node.evaluation_cost = self.heuristic(node.state, self.x, self.y, self.z)
            
        self.queue.items.extend(nodes)
        self.queue.items.sort(key=lambda x: (x.path_cost+x.evaluation_cost))
        
class AStar():
    def __init__(self, two_seq_query, three_seq_query):

        self.two_seq_query = two_seq_query
        self.three_seq_query = three_seq_query
        
        
        self.data_base = []
        
        MSA_databse_file = open('MSA_database.txt')

        for line in MSA_databse_file:
            self.data_base.append(line.strip('\n'))

    
    @staticmethod
    def heuristic_pairwise(state, x: str, y:str):
        g_x, g_y = len(x), len(y)
        current_x = len(state[0].replace('-',''))
        current_y = len(state[1].replace('-',''))
        return 2*((g_x-current_x)-(g_y-current_y))


    @staticmethod
    def heuristic_triseq(state, x: str, y:str, z:str):
        g_x, g_y, g_z = len(x), len(y), len(z)
        current_x = len(state[0].replace('-',''))
        current_y = len(state[1].replace('-',''))
        current_z = len(state[2].replace('-',''))

        return 2*((g_x-current_x)-(g_y-current_y))+2*((g_x-current_x)-(g_z-current_z))+2*((g_z-current_z)-(g_y-current_y))

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
                    # print(f'key sequence index {index_key}')
                    current_result = self.__pairwiseAlign(seq_query, seq_key)
                    if(current_result<result):
                        result, index_result = current_result, index_key
                    pbar.update(1)
            
            print(f'cost: {result} ', f'index: {index_result}')
            print('target sequence: '+self.data_base[index_result])
            print(f'cost: {result} ', f'index: {index_result}', file=result_file)
            print('target sequence: '+self.data_base[index_result],file=result_file)

            end_time = time.time()
            print(f'Processing time: {end_time-start_time} s')
            print(f'Processing time: {end_time-start_time} s', file=result_file)
    def __pairwiseAlign(self, x: str, y: str):
        
        initial_state = ('','')
        AStarApproach = AStarSearch(initial_state, expand_node_pairwise, goal_test_pairwise, self.heuristic_pairwise, x, y) # 这里添加h函数
        # AStarApproach = AStarSearch(initial_state, expand_node, goal_test, lambda x,y,z:0, x, y)
        solution, node = AStarApproach.find_path(search_cost=0)

        if solution is False:
            print ("No Solution Found")
        else:
            print ("FOUND SOLUTION")
            print (node.state)
            print (node.path_cost)
            print (solution)
        return node.path_cost

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
    def __triSeqAlign(self, x: str, y:str, z: str):
        
        initial_state = ('','','')
        AStarApproach = AStarSearch(initial_state, expand_node_triseq, goal_test_triseq, self.heuristic_triseq, x, y, z) # 这里添加h函数
        # AStarApproach = AStarSearch(initial_state, expand_node, goal_test, lambda x,y,z:0, x, y)
        solution, node = AStarApproach.find_path(search_cost=0)

        if solution is False:
            print ("No Solution Found")
        else:
            print ("FOUND SOLUTION")
            print (node.state)
            print (node.path_cost)
            print (solution)
        return node.path_cost
    def correctnessValidation(self):

        test_result_file = open('./AStar_test_result.txt', 'w')
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

def expand_node_pairwise(node, x: str, y: str):
    x_index = (len(node.state[0].replace('-',''))-1)+1 # 统计得到当前node对应x的长度为lenX，那么其下标九尾lenX-1，要扩展的下一个结点的下标为lenX
    y_index = (len(node.state[1].replace('-',''))-1)+1
    
    expanded_nodes = []

    # gap-y
    if x_index!=len(x):
        new_node_1 = copy.copy(node)
        new_node_1.parent = node
        new_node_1.depth = node.depth + 1
        new_node_1.state = (node.state[0]+x[x_index], node.state[1]+'-')
        new_node_1.action = 'gap_y'
        new_node_1.path_cost = node.path_cost + 2
        expanded_nodes.append(new_node_1)

    # gap-x
    if y_index!=len(y):
        new_node_2 = copy.copy(node)
        new_node_2.parent = node
        new_node_2.depth = node.depth + 1
        new_node_2.state = (node.state[0]+'-', node.state[1]+y[y_index])
        new_node_2.action = 'gap_x'
        new_node_2.path_cost = node.path_cost + 2
        expanded_nodes.append(new_node_2)

    if x_index!=len(x) and y_index!=len(y):
        new_node_3 = copy.copy(node)
        new_node_3.parent = node
        new_node_3.depth = node.depth + 1
        new_node_3.state = (node.state[0]+x[x_index], node.state[1]+y[y_index])
        if x[x_index] == y[y_index]: # match
            new_node_3.action = 'match'
            new_node_3.path_cost = node.path_cost + 0
        else:
            new_node_3.action = 'mismatch'
            new_node_3.path_cost = node.path_cost + 3
        expanded_nodes.append(new_node_3)

    return expanded_nodes

def expand_node_triseq(node, x: str, y: str, z: str):

    
    x_index = (len(node.state[0].replace('-',''))-1)+1 # 统计得到当前node对应x的长度为lenX，那么其下标九尾lenX-1，要扩展的下一个结点的下标为lenX
    y_index = (len(node.state[1].replace('-',''))-1)+1
    z_index = (len(node.state[2].replace('-',''))-1)+1
    
    expanded_nodes = []

    # gap-y
    if x_index!=len(x) and z_index!=len(z):
        new_node_1 = copy.copy(node)
        new_node_1.parent = node
        new_node_1.depth = node.depth + 1
        new_node_1.state = (node.state[0]+x[x_index], node.state[1]+'-', node.state[2]+z[z_index])
        new_node_1.action = 'gap_y'
        new_node_1.path_cost = node.path_cost + (4 if x[x_index]==z[z_index] else 7)
        expanded_nodes.append(new_node_1)

    # gap-x
    if y_index!=len(y) and z_index!=len(z):
        new_node_2 = copy.copy(node)
        new_node_2.parent = node
        new_node_2.depth = node.depth + 1
        new_node_2.state = (node.state[0]+'-', node.state[1]+y[y_index], node.state[2]+z[z_index])
        new_node_2.action = 'gap_x'
        new_node_2.path_cost = node.path_cost + (4 if y[y_index]==z[z_index] else 7)
        expanded_nodes.append(new_node_2)

    # gap z
    if x_index!=len(x) and y_index!=len(y):
        new_node_3 = copy.copy(node)
        new_node_3.parent = node
        new_node_3.depth = node.depth + 1
        new_node_3.state = (node.state[0]+x[x_index], node.state[1]+y[y_index], node.state[2]+'-')
        new_node_3.action = 'gap_z'
        new_node_3.path_cost = node.path_cost + (4 if x[x_index]==y[y_index] else 7)
        expanded_nodes.append(new_node_3)
    
    # gap x,y
    if z_index!=len(z):
        new_node_4 = copy.copy(node)
        new_node_4.parent = node
        new_node_4.depth = node.depth + 1
        new_node_4.state = (node.state[0]+'-', node.state[1]+'-', node.state[2]+z[z_index])
        new_node_4.action = 'gap_x,y'
        new_node_4.path_cost = node.path_cost + 4
        expanded_nodes.append(new_node_4)

    # gap y,z
    if x_index!=len(x):
        new_node_5 = copy.copy(node)
        new_node_5.parent = node
        new_node_5.depth = node.depth + 1
        new_node_5.state = (node.state[0]+x[x_index], node.state[1]+'-', node.state[2]+'-')
        new_node_5.action = 'gap_y,z'
        new_node_5.path_cost = node.path_cost + 4
        expanded_nodes.append(new_node_5)
    # gap x,z
    if y_index!=len(y):
        new_node_6 = copy.copy(node)
        new_node_6.parent = node
        new_node_6.depth = node.depth + 1
        new_node_6.state = (node.state[0]+'-', node.state[1]+y[y_index], node.state[2]+'-')
        new_node_6.action = 'gap_y,z'
        new_node_6.path_cost = node.path_cost + 4
        expanded_nodes.append(new_node_6)

    # (mis)match
    if x_index!=len(x) and y_index!=len(y) and z_index!=len(z):
        new_node_7 = copy.copy(node)
        new_node_7.parent = node
        new_node_7.depth = node.depth + 1
        new_node_7.state = (node.state[0]+x[x_index], node.state[1]+y[y_index], node.state[2]+z[z_index])
        if x[x_index] == y[y_index] == z[z_index]: # match
            new_node_7.action = 'match'
            new_node_7.path_cost = node.path_cost + 0
        elif x[x_index]==y[y_index]:
            new_node_7.action = 'mismatch_z'
            new_node_7.path_cost = node.path_cost + 6
        elif x[x_index]==z[z_index]:
            new_node_7.action = 'mismatch_y'
            new_node_7.path_cost = node.path_cost + 6
        elif y[y_index]==z[z_index]:
            new_node_7.action = 'mismatch_x'
            new_node_7.path_cost = node.path_cost + 6
        else:
            new_node_7.action = 'mismatch_all'
            new_node_7.path_cost = node.path_cost + 9
        expanded_nodes.append(new_node_7)

    return expanded_nodes

def goal_test_pairwise(node, x: str, y: str):
    # expanded_nodes = expand_node(node, x, y)

    # for n in expanded_nodes:
    #     if n.state[0].replace('-','')==x and n.state[1].replace('-','')==y:
    #         print(n.state[0].replace('-',''), x)
    #         print(n.state[1].replace('-',''), y)
    #         return True, n
    # return False, n
    if node.state[0].replace('-','')==x and node.state[1].replace('-','')==y:
        return True, node
    return False, node

def goal_test_triseq(node, x: str, y: str, z: str):
    # expanded_nodes = expand_node(node, x, y)

    # for n in expanded_nodes:
    #     if n.state[0].replace('-','')==x and n.state[1].replace('-','')==y:
    #         print(n.state[0].replace('-',''), x)
    #         print(n.state[1].replace('-',''), y)
    #         return True, n
    # return False, n
    if node.state[0].replace('-','')==x and node.state[1].replace('-','')==y and node.state[2].replace('-','')==z:
        return True, node
    return False, node

if __name__ == '__main__':
    two_seq_query = ['KJXXJAJKPXKJJXJKPXKJXXJAJKPXKJJXJKPXKJXXJAJKPXKJXXJAJKHXKJXXJAJKPXKJXXJAJKHXKJXX',
                    'ILOTGJJLABWTSTGGONXJMUTUXSJHKWJHCTOQHWGAGIWLZHWPKZULJTZWAKBWHXMIKLZJGLXBPAHOHVOLZWOSJJLPO',
                    'IHKKKRKKKKKKXGWGKKKPKSKKKKKBKKKPKHKKXKKBSKKPKWKKLKSKRKKWXKPKKBKKKPKTSKHKKKKLADKKYPKKKOPHKKBWWLPPWKK',
                    'MPPPJPXPGPJPPPXPPPJPJPPPXPPPPSPPJJJPPXXPPPPPJPPPXPPXIPJMMMXPKPSVGULMHHZPAWHTHKAAHHUPAONAPJSWPPJGA',
                    'IPPVKBKXWXKHSAPHVXXVOJMRAKKPJVLLJBWKOLLJKXHGXLLCPAJOBKPGXBATGXMPOMCVZTAXVPAGKXGOMJQOLJGWGKXLQ']
    three_seq_query = ['IPZJJLMLTKJULOSTKTJOGLKJOBLTXGKTPLUWWKOMOYJBGALJUKLGLOSVHWBPGWSLUKOBSOPLOOKUKSARPPJ',
                    'IWTJBGTJGJTWGBJTPKHAXHAGJJSJJPPJAPJHJHJHJHJHJHJHJHJPKSTJJUWXHGPHGALKLPJTPJPGVXPLBJHHJPKWPPDJSG']
    
    Msa = AStar(two_seq_query, three_seq_query)


    Msa.correctnessValidation()
    # Msa.pairwiseTask()
    # Msa.triSeqTask()
    

    result_file.close()