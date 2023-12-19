import numpy as np
import seaborn as sns
import matplotlib.pyplot as plt


def parse_range_query_txt(path_to_txt: str):
    all_times = []
    with open(path_to_txt, 'r') as f:
        for line in f:
            line = line.replace("\n", "")
            time = int(line)
            all_times.append(time)
    return all_times

def get_times(range_query_bst_path: str, range_query_sl_path: str):
    bst_times = parse_range_query_txt(range_query_bst_path)
    sl_times = parse_range_query_txt(range_query_sl_path)

    # Calculate means and standard deviations
    bst_mean = np.mean(bst_times)
    sl_mean = np.mean(sl_times)
    sl_std = np.std(sl_times)
    bst_std = np.std(bst_times)

    # 3-sigma rule
    bst_left, bst_right = bst_mean - bst_std * 3, bst_mean + bst_std * 3
    sl_left, sl_right = sl_mean - sl_std * 3, sl_mean + sl_std * 3

    # Filter out the values
    bst_times = [i / 1e9 for i in bst_times if i > bst_left and i < bst_right]
    sl_times = [i / 1e9 for i in sl_times if i > sl_left and i < sl_right]
    return bst_times, sl_times

def plot_times(bst_times, sl_times):
    sns.histplot(data=bst_times, kde=True, color="blue", label="BST")
    sns.histplot(data=sl_times, kde=True, color="green", label="SkipList")
    plt.title("SkipList vs BST Range Query(500-10000), 1M Array")
    plt.xlabel("Time taken to print all numbers in the range(seconds)")
    plt.ylabel("# Count")
    plt.legend()
    plt.show()

def main():
    (bst_times,
     sl_times,
     ) = get_times("statistical_comparisons/range_query_bst.txt", "statistical_comparisons/range_query_sl.txt")
    bst_times = np.array(bst_times)
    sl_times = np.array(sl_times)
    plot_times(bst_times, sl_times)

if __name__ == "__main__":
    main()
    