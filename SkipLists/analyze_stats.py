import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
import seaborn as sns
from typing import List, Dict
from visualization_parameters import VISUALIZATION_PARAMETERS


def initialize_dict_from_txt(file_txt: str):
    # Initialize an empty dictionary
    data_dict = {}
    
    # Open the file and read line by line
    with open(file_txt, 'r') as f:
        for line in f:
            # Split each line into name and number
            name, num = line.strip().split(' - ')
            
            # If the name is already a key in the dictionary, add the number to the existing value
            if name in data_dict:
                data_dict[name].append(int(num))
            # Otherwise, add the name to the dictionary with the number as its value
            else:
                data_dict[name] = [int(num)]
    return data_dict

def remove_outliers(data):
    q25, q75 = np.percentile(data, 25), np.percentile(data, 75)
    iqr = q75 - q25
    cut_off = iqr * 1.5
    lower, upper = q25 - cut_off, q75 + cut_off
    outliers_removed = [x for x in data if x >= lower and x <= upper]
    return outliers_removed

def remove_outliers_all(data_dict):
    for key in data_dict:
        data_dict[key] = remove_outliers(data_dict[key])
    return data_dict

def init_subplots():
    _, axs = plt.subplots(3, 2, figsize=(20, 10))  # Create a figure with two subplots
    return axs

def plot_skip_list_and_bst(data: Dict[str, List[int]], axs : plt.axes, row: int, col: int, title: str):
    sns.histplot(data["SkipList"], kde=True, color="blue", label="SkipList", ax=axs[row, col])
    sns.histplot(data["BST"], kde=True, color="green", label="BST", ax=axs[row, col])
    axs[row, col].set_title(title)
    axs[row, col].legend()
    axs[row, col].set_xlabel("Nano seconds")
    plt.tight_layout()

def visualize_pipeline(file_txt: str, axs, axs_index: int, title: str):
    data = initialize_dict_from_txt(file_txt)
    data = remove_outliers_all(data)
    row = axs_index // 2
    col = axs_index % 2
    data = initialize_dict_from_txt(file_txt)
    data = remove_outliers_all(data)
    plot_skip_list_and_bst(data, axs, row, col, title)
    

if __name__ == "__main__":
    for element_position in VISUALIZATION_PARAMETERS:
        axs = init_subplots()
        for parameters in VISUALIZATION_PARAMETERS[element_position]:
            visualize_pipeline(parameters['file_name'], axs, parameters['index'], parameters['title'])
        plt.savefig(f"BST_SkipList_comparison_{element_position}.png")
