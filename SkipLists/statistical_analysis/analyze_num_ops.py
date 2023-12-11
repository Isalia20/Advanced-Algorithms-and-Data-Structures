from typing import Dict
import matplotlib.pyplot as plt
import numpy as np


def parse_num_ops_txt(num_ops_file_path: str):
    element_num_ops = {}
    
    with open(num_ops_file_path, 'r') as f:
        op_count_sum = 0
        for line in f:
            line = line.replace("\n", "")
            if "ELEMENT" in line:
                element_num = line.split("-")[1].replace(" ", "")
                element_num_ops[element_num] = 0
            if line == "------------------------------------": # End of skip list
                element_num_ops[element_num] = op_count_sum
                op_count_sum = 0
                continue
            op_count_sum += 1
    return element_num_ops

def get_counts(data_structure_dict: Dict[str, int], indices):
    counts = [data_structure_dict.get(str(i), 0) for i in indices]
    return counts

def bin_data(data, bin_edges):
    binned_data = np.empty(len(bin_edges) - 1)
    for i in range(len(bin_edges) - 1):
        bin_range = data[bin_edges[i]:bin_edges[i + 1]]
        binned_data[i] = np.mean(bin_range)  # Using mean for bin summary
    return binned_data

def plot_bst_skiplist(bin_centers, bst_binned, skiplist_binned):
    plt.figure(figsize=(10, 5)) 

    plt.plot(bin_centers, bst_binned, label='BST Operation Count', color='blue')
    plt.plot(bin_centers, skiplist_binned, label='SkipList Operation Count', color='red')

    # Labeling the plot
    plt.title('Binned Operation Count Comparison between BST and SkipList(Bins of 20)')
    plt.xlabel('Number to search')
    plt.ylabel('Binned Operation Count')
    plt.legend()
    plt.show()
    return

def main():
    indices = np.arange(0, 100002)
    bst = parse_num_ops_txt("statistical_comparisons/count_ops_bst.txt")
    skiplist = parse_num_ops_txt("statistical_comparisons/count_ops_skiplist.txt")
    bst_counts = get_counts(bst, indices)
    skiplist_counts = get_counts(skiplist, indices)

    bin_size = 20
    bin_edges = np.arange(0, len(bst_counts), bin_size)
    bin_centers = bin_edges[:-1] + np.diff(bin_edges) / 2
    bst_binned = bin_data(bst_counts, bin_edges)
    skiplist_binned = bin_data(skiplist_counts, bin_edges)
    plot_bst_skiplist(bin_centers, bst_binned, skiplist_binned)


if __name__ == "__main__":
    main()
    